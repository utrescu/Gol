/***************************************************************************
 *   Copyright (C) 2006 by Xavier Sala                                     *
 *   utrescu@xaviersala.net                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "PorterEstats.h"
#include "Porter.h"
#include "JugadorMissatges.h"
#include "missatger.h"
#include "general.h"
#include "Pilota.h"
#include "Regio.h"

//uncomment to send state info to debug window
// #define PORTER_STATE_INFO_ON

//------------------------   ESTAT GLOBAL ------------------------------
PorterEstatGlobal* PorterEstatGlobal::Instance()
{
	static PorterEstatGlobal instance;
	return &instance;
}

void PorterEstatGlobal::Enter(Porter* player)
{};

void PorterEstatGlobal::Execute(Porter* player)
{};

void PorterEstatGlobal::Exit(Porter* player)
{};

bool PorterEstatGlobal::OnMessage(Porter* keeper, const Missatge& mis)
{
	switch(mis.Msg)
	{
		case Msg_GoHome:
		{
#ifdef PORTER_STATE_INFO_ON
    	printf("..........Porter ha rebut peticio de tornar a casa %d\n", keeper->getID());
#endif	
			keeper->setTornaARegioInicial();
			keeper->getFSM()->canviaEstat(PorterEstatTornaACasa::Instance());
		}
		break;

		case Msg_ReceiveBall:
		{
#ifdef PORTER_STATE_INFO_ON
    	printf("..........Porter %d ha rebut peticio d'anar a per la pilota\n", keeper->getID());
#endif				
			keeper->getFSM()->canviaEstat(PorterEstatIntercepta::Instance());
		}
		break;
	}//end switch
	return false;
};
//---------------------   SEGUEIX PILOTA ------------------------------
// Es l'estat per defecte del porter, va seguint la pilota sense 
// moure's de la porteria. 
// --------------------------------------------------------------------
PorterEstatCobrirPorteria* PorterEstatCobrirPorteria::Instance()
{
	static PorterEstatCobrirPorteria instance;
	return &instance;
}

void PorterEstatCobrirPorteria::Enter(Porter* keeper)
{
  	//turn interpose on
	keeper->getComportament()->InterposeOn(keeper->Porter_Seguiment());
	// Aquest sera l'objectiu
	keeper->getComportament()->setTarget(keeper->GetRearInterposeTarget());
#ifdef PORTER_STATE_INFO_ON
     	printf("Porter %d vegila la pilota\n", keeper->getID());
#endif
};

void PorterEstatCobrirPorteria::Execute(Porter* keeper)
{
  	// Actualitzar l'objectiu perque la piloteta es mou.
	keeper->getComportament()->setTarget(keeper->GetRearInterposeTarget());
	// Si la pilota queda a prop, l'agafa i es prepara per xutar-la
	if (keeper->isPilotaAbastPorter()) // isPilotaAbastDeRecepcio())
	{
		keeper->getPilota()->Agafada(); 
		keeper->getCamp()->setLaTeElPorter(true);
		keeper->getFSM()->canviaEstat(PorterEstatXuta::Instance());
		return;
	}
	// #### Falta determinar si s'ha d'estirar 
	//    a) Si està prou a prop
	//    b) No ve cap a mi

  	// si la pilota esta en una distacia massa propera es mou per interceptar-la 
	// if (keeper->isPotCapturarLaPilota() 
	if (keeper->isAbastDeIntercepcio()) // && !keeper->getEquip()->TincPilota())
	{
		keeper->getFSM()->canviaEstat(PorterEstatIntercepta::Instance());
	}

  	// Si el porter ha sortit massa, hauria de recular cap a la porteria 
	if (keeper->isMassaLlunyDeLaPorteria() && keeper->getEquip()->TincPilota())
	{
		keeper->getFSM()->canviaEstat(PorterEstatTornaACasa::Instance());
		return;
	}
};

void PorterEstatCobrirPorteria::Exit(Porter* keeper)
{
	keeper->getComportament()->InterposeOff();
};
//---------------------   TORNAR A CASA  -------------------------------
PorterEstatTornaACasa* PorterEstatTornaACasa::Instance()
{
	static PorterEstatTornaACasa instance;
	return &instance;
}

void PorterEstatTornaACasa::Enter(Porter* keeper)
{
	keeper->getComportament()->ArriveOn();	
#ifdef PORTER_STATE_INFO_ON
     	printf("Porter %d torna a casa\n", keeper->getID());
#endif
};

void PorterEstatTornaACasa::Execute(Porter* keeper)
{
	keeper->getComportament()->setTarget(keeper->getRegioNormal()->getPuntMig());
  	// Quan sigui prou a prop del desti, es posa a seguir la pilota 
	// Si no han fet un gol (si han fet un gol no cal fer res)
	if ((keeper->isACasa() || !keeper->getEquip()->TincPilota())
		 && keeper->getCamp()->isGol()==false)
	{
		keeper->getFSM()->canviaEstat(PorterEstatCobrirPorteria::Instance());
#ifdef PORTER_STATE_INFO_ON
     		printf("... PORTER %d Passa a cobrir per: ( Casa(%d) o NoPossessio(%d) ) i gol(%d)==false\n", keeper->getID(), keeper->isACasa(), !keeper->getEquip()->TincPilota(),keeper->getCamp()->isGol() );
#endif
	}	
};

void PorterEstatTornaACasa::Exit(Porter* keeper)
{
	keeper->getComportament()->ArriveOff();	
};

//------------------  INTERCEPTA PILOTA -------------------------
PorterEstatIntercepta* PorterEstatIntercepta::Instance()
{
	static PorterEstatIntercepta instance;
	return &instance;
}

void PorterEstatIntercepta::Enter(Porter* keeper)
{
	keeper->getComportament()->PursuitOn();
#ifdef PORTER_STATE_INFO_ON
     		printf("PORTER %d vol interceptar la pilota\n", keeper->getID());
#endif
};

void PorterEstatIntercepta::Execute(Porter* keeper)
{
	//Si el porter se'n va massa lluny, ha de tornar a la porteria a 
	// menys que sigui el que esta mes a prop de la pilota
	if ((keeper->isMassaLlunyDeLaPorteria() && !keeper->isElMesProperDelCampAPilota()) || keeper->isForaDelCamp())
	{
#ifdef PORTER_STATE_INFO_ON
     		printf("... PORTER %d Torna per: lluny(%d) i NoMesProper(%d) o fora(%d)\n", keeper->getID(), keeper->isMassaLlunyDeLaPorteria(), !keeper->isElMesProperDelCampAPilota(),keeper->isForaDelCamp() );
#endif
		keeper->getFSM()->canviaEstat(PorterEstatTornaACasa::Instance());
		return;
	}
	// #### Falta determinar si s'ha d'estirar 

  	// Si la pilota esta a l'abast l'agafa i la torna al joc 
	if (keeper->isPilotaAbastPorter())
	{
		keeper->getPilota()->Agafada(); 
		keeper->getCamp()->setLaTeElPorter(true);
		keeper->getFSM()->canviaEstat(PorterEstatXuta::Instance());
		return;
	}
};

void PorterEstatIntercepta::Exit(Porter* keeper)
{
	keeper->getComportament()->PursuitOff();  
};

//------------------  XUTA PILOTA -------------------------
PorterEstatXuta* PorterEstatXuta::Instance()
{
	static PorterEstatXuta instance;
	return &instance;
}

void PorterEstatXuta::Enter(Porter* keeper)
{
	//let the team know that the keeper is in control
	keeper->getEquip()->setJugadorAmbLaPilota(keeper);

	//envia tothom cap a casa 
	keeper->getEquip()->getContraris()->TothomACasa();
	keeper->getEquip()->TothomACasa();
#ifdef PORTER_STATE_INFO_ON
     	printf("Porter %d vol xutar\n", keeper->getID());
#endif
};

void PorterEstatXuta::Execute(Porter* keeper)
{
	JugadorBase* receiver = NULL;
	Punt3 BallTarget;
    
  	//Buscar un jugador mes avançat per fer-li la passada
	if (keeper->getEquip()->BuscaPassada(keeper, receiver, BallTarget, keeper->Jugador_ForcaPassada(),
	                                     keeper->Jugador_DistanciaMinimaPassada()))
	{
#ifdef PORTER_STATE_INFO_ON
    	printf("..........Porter %d passa pilota a (%f,%f)\n", keeper->getID(), BallTarget.x, BallTarget.y);
#endif	
    	// Fer la passada
		double power = keeper->getPilota()->ForcaPerMoure(BallTarget, keeper->Jugador_ForcaPassada());
		
		// ---- Per fer que xuti enlaire poso valor a 'h'
		Punt3 dir = VectorNormalitza(BallTarget-keeper->getPilota()->getPosicio());
		dir.h = power;

		keeper->getPilota()->Xuta(dir, power);
		// FORCAPASSADAMAXIMA);
		// El porter ja no te la pilota 
		keeper->getCamp()->setLaTeElPorter(false); 
    		// Avisem el receptor que li ve la pilota 
		Carter->DispatchMsg(SEND_MSG_IMMEDIATELY, keeper->getID(), receiver->getID(), Msg_ReceiveBall, &BallTarget);
    		// torna a seguir la pilota des de la porteria 
		keeper->getFSM()->canviaEstat(PorterEstatCobrirPorteria::Instance());
		return;
	}
	else
	{
		// FALTA:
		// 1. Demana als defenses que baixin perquè són massa lluny
		// 2. Xuta endavant i que sigui el que déu vulgui...
		// keeper->getEquip()->CridaElsDefenses();
#ifdef PORTER_STATE_INFO_ON
    	printf("..........PORTER %d allunya pilota\n", keeper->getID());
#endif	

		keeper->getCamp()->setLaTeElPorter(false); 
		// ---- Per fer que xuti enlaire poso valor a 'h'
		Punt3 PortContraria = VectorNormalitza(keeper->getEquip()->getPorteriaContraria()->getMig()- keeper->getPilota()->getPosicio());
		PortContraria.h=keeper->Jugador_ForcaXut();

		keeper->getPilota()->Xuta(PortContraria,keeper->Jugador_ForcaXut());
		keeper->getEquip()->setJugadorAmbLaPilota(NULL);
		keeper->getFSM()->canviaEstat(PorterEstatCobrirPorteria::Instance());

	}
	keeper->setVelocitat(Punt3());
};

void PorterEstatXuta::Exit(Porter* keeper)
{
	// Per quan el porter tira la pilota d'un xut que ha sortit fora
	if (!keeper->getCamp()->isPartitEnJoc())
	{
		// ... Però s'ha de vegilar que no sigui ell el que la tira des de fora (25-8-2006)
		if (keeper->getCamp()->getTerrenyDeJoc()->isDins(keeper->getPilota()->getPosicio()))
		{
			keeper->getCamp()->setPartitEnJoc(true);
		}
	}
};

//---------------------   S'ESTIRA ------------------------------
// El porter s'estira per atrapar la pilota ... 
// 
// --------------------------------------------------------------------
PorterEstatEstirat* PorterEstatEstirat::Instance()
{
	static PorterEstatEstirat instance;
	return &instance;
}

void PorterEstatEstirat::Enter(Porter* keeper)
{
	Punt3 XXX;
	// S'estira el porter
	keeper->getComportament()->SeekOn();
	// Dir-li cap a on s'estirarà: On és XXX?????
	XXX.x = keeper->getEquip()->getPorteriaNostra()->getPosicioMig().x;
	// #### Falta determinar cap a on s'estirarà
	// He de saber on anirà la pilota sobre la ratlla per estirar-me cap allà


	keeper->getComportament()->setTarget(XXX);
	keeper->setEstirat(true);
	// Canvi entre h i w ...
	// --> Fer que ara les imatges són les de l'estirada...
	// --> Fer-lo més veloç???
};

void PorterEstatEstirat::Execute(Porter* keeper)
{
 	// Si la pilota esta a l'abast l'agafa i la torna al joc 
	if (keeper->isPilotaAbastPorter())
	{
		keeper->getPilota()->Agafada(); 
		keeper->getCamp()->setLaTeElPorter(true);
		keeper->getFSM()->canviaEstat(PorterEstatCaigut::Instance());
		return;
	}
	// Si ens hem aturat definitivament, hem de passar a caigut
	if (keeper->getVelocitat()==0)
	{
		keeper->getFSM()->canviaEstat(PorterEstatCaigut::Instance());
		return;
	}

};

void PorterEstatEstirat::Exit(Porter* keeper)
{
	// S'atura de cop!
	keeper->getComportament()->SeekOff();
	// Per si surt perquè han fet un gol he de desactivar estirat ... 
	keeper->setEstirat(false);
};

//---------------------   CAIGUT  ------------------------------
// El porter s'ha estirat i s'ha de quedar a terra una estona
// 
// --------------------------------------------------------------------
PorterEstatCaigut* PorterEstatCaigut::Instance()
{
	static PorterEstatCaigut instance;
	return &instance;
}

void PorterEstatCaigut::Enter(Porter* keeper)
{
	// Hauria de posar un valor de 'reacció' individual de cada porter
	// Això ens permetrà tenir porters més bons
	m_pRegulator = new Regulator(keeper->Porter_Reaccio());
	// Ara hauria d'estar totalment aturat
	keeper->setVelocitat(Punt3());

	// Les imatges sempre són el porter a terra...
	// ## -- Com ho munto? 

	// El porter continua estirat a terra (perquè estirat l'ha deshabilitat ...)
	keeper->setEstirat(true);
};

void PorterEstatCaigut::Execute(Porter* keeper)
{
	// Potser hauria de posar que si per casualitat la pilota li cau a sobre l'agafi
	if (keeper->isPilotaAbastPorter())
	{
		keeper->getPilota()->Agafada(); 
		keeper->getCamp()->setLaTeElPorter(true);
		// keeper->getFSM()->canviaEstat(PorterEstatCaigut::Instance());
		return;
	}	
	// Deixar passar un temps amb el jugador a terra (més que res per putejar)
	if (m_pRegulator->isReady())
	{
		if (keeper->getCamp()->isEnMansDelPorter())
		{
			// Quan ha passat el temps, si tinc la pilota Xutar
			keeper->getFSM()->canviaEstat(PorterEstatXuta::Instance());
		}
		else
		{
			// Si no la tinc Aixecar-se i tornar a porteria o cobrir pilota ... 
			keeper->getFSM()->canviaEstat(PorterEstatTornaACasa::Instance());
		}
	}
};

void PorterEstatCaigut::Exit(Porter* keeper)
{
	delete m_pRegulator;
	keeper->setEstirat(false);
};
