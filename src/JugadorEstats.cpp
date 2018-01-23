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
#include "JugadorEstats.h"
#include "JugadorMissatges.h"
#include "missatger.h"
#include "Jugador.h"
#include "general.h"
#include "Pilota.h"
#include "Regio.h"
#include "C2DMatrix.h"

// #define PLAYER_STATE_INFO_ON

//------------------------   ESTAT GLOBAL ------------------------------
//  Basicament es fa servir per recepcionar missatges. Es tracta d'un
//  estat permanent.
//----------------------------------------------------------------------
JugadorEstatGlobal* JugadorEstatGlobal::Instance()
{
	static JugadorEstatGlobal instance;

	return &instance;
}

void JugadorEstatGlobal::Enter(Jugador* player)
{};

void JugadorEstatGlobal::Execute(Jugador* player)
{
	// Si esta a prop de la pilota i te la possessio, redueix la velocitat
	if((player->isPilotaAbastDeRecepcio()) && (player->isControlantPilota()))
	{
		player->setVelocitatMaxima(player->Jugador_VelocitatAmb());
	}

	else
	{
		player->setVelocitatMaxima(player->Jugador_VelocitatSense());
	}
};

void JugadorEstatGlobal::Exit(Jugador* player)
{};

bool JugadorEstatGlobal::OnMessage(Jugador* player, const Missatge& mis)
{
	
	switch(mis.Msg)
	{
		case Msg_ReceiveBall:
		{
      		// Ens han enviat la pilota, per tant hem de preparar-nos per rebre-la al 
			// lloc que ens diu 
			player->getComportament()->setTarget(*(static_cast<Punt3*>(mis.ExtraInfo)));
		    // Ens passem a l'estat de rebre 
			player->getFSM()->canviaEstat(JugadorEstatRebrePasse::Instance());
			return true;
		}
		break;
		
		// L'equip ha fet un gol, han de tornar a casa fent una bonica celebracio	
		case Msg_Gol:
		{
			player->setTornaARegioInicial();
			player->getFSM()->canviaEstat(JugadorEstatTornaACasa::Instance());
		}
		break;
		
		case Msg_SupportAttacker:
		{
      		// Si ja està donant suport, no cal fer res
			if (player->getFSM()->isEnEstat(*JugadorEstatAjudaAtac::Instance()))
			{
				return true;
			}
      
			/*
			// Fora de joc!
			if( player->isElMesProperDelCampAPorteriaContraria() )
			{
				PlayerBase *kicker = player->getEquip()->getJugadorPerID(telegram.Sender);
   				// Comprovar si el xutador està més a prop que els altres
				if( kicker->DistanciaPorteriaContraria() > player->DistanciaPorteriaContraria() )
				{
					printf("Player %d està en fora de joc"), kicker->getID());
				}
			} 			
			*/
      		//Li haig de dir on ha d'anar .... ## FALTA FER @@@ 
			player->getComportament()->setTarget(player->getEquip()->getPuntDeSuport());
			player->getFSM()->canviaEstat(JugadorEstatAjudaAtac::Instance());
			return true;
		}

		break;

		case Msg_Wait:
		{
			player->getFSM()->canviaEstat(JugadorEstatEsperant::Instance());
			return true;
		}

		break;

		case Msg_GoHome:
		{
			// player->SetDefaultHomeRegion(); ## FALTA ##
			player->getFSM()->canviaEstat(JugadorEstatTornaACasa::Instance());
			return true;
		}

		break;

		case Msg_AnarASaque:
		{
			player->getFSM()->canviaEstat(JugadorEstatAnarASaque::Instance());
			return true;
		}
		break;

		case Msg_PassToMe:
		{  
      		// obtenir la posicio del que demana la passada
			Jugador* receiver = static_cast<Jugador*>(mis.ExtraInfo);

#ifdef PLAYER_STATE_INFO_ON
      		printf("Jugador %d rep peticio de %d de passada\n", player->getID(),receiver->getID());
#endif
			// Si ja ha decidit la passada o no té la pilota, s'ha de rebutjar la petició
			if (player->getEquip()->getJugadorDeRecepcio() != NULL ||
		            !player->isPilotaAbastDeXut() )
		    {
			      // Rebutjat perque es impossible o no conve 
#ifdef PLAYER_STATE_INFO_ON
        	printf("... Jugador %d No pot fer la passada <no pot xutar>\n",player->getID());
#endif
				return true;
            }
		    // fer la passada. Se l'ha d'avisar que li enviem perque la esperi...
			// ---- XAVIER: Calcular la força necessaria per xutar
			double power = player->getPilota()->ForcaPerMoure(receiver->getPosicio(), player->Jugador_ForcaPassada());
			player->getPilota()->Xuta(receiver->getPosicio() - player->getPilota()->getPosicio(), power);
			
#ifdef PLAYER_STATE_INFO_ON
   		printf("... Jugador %d prova un passe a (%f,%f)\n", player->getID(),receiver->getPosicio().x, receiver->getPosicio().y);
#endif
		    	//Avisar al receptor 
      			Carter->DispatchMsg(SEND_MSG_IMMEDIATELY,
			                  player->getID(),
			                  receiver->getID(),
			                  Msg_ReceiveBall,
			                  &receiver->getPosicio());
				// Esperar noves ordres
		    	player->getFSM()->canviaEstat(JugadorEstatEsperant::Instance());
            		player->BuscaAjuda();
            		return true;
		}
		break;

	}//end switch

	return false;
};

//--------------------   PERSEGUIR PILOTA ------------------------------
JugadorEstatPerseguirPilota* JugadorEstatPerseguirPilota::Instance()
{
	static JugadorEstatPerseguirPilota instance;

	return &instance;
}

void JugadorEstatPerseguirPilota::Enter(Jugador* player)
{
	// Activem el comportament de perseguir la pilota 
	player->getComportament()->SeekOn();
	// player->getComportament()->AvoidObstacleOn();
	
#ifdef PLAYER_STATE_INFO_ON
  	printf("Jugador %d comença a perseguir la pilota i mira a (%f,%f,%f)\n", player->getID(), player->getMirant().x, player->getMirant().y, player->getMirant().h);
#endif
};

void JugadorEstatPerseguirPilota::Execute(Jugador* player)
{
	// La pilota esta molt a prop de nosaltres, per tant la xutem
	if (player->isPilotaAbastDeXut())
	{
#ifdef PLAYER_STATE_INFO_ON
     		printf("... Jugador %d intentara un xut\n", player->getID());
#endif
		player->getFSM()->canviaEstat(JugadorEstatXuta::Instance());
		return;
	}
  	//si es el jugador del nostre equip mes proper a la pilota, l'hem de perseguir 
	if (player->isMesProperAPilota())
	{
		// Aqui es on li dic que ha de perseguir 
		// Si la pilota està enrere, també podriem fer Avoid() i de passada esquivarà contraris
		double dot = player->getEquip()->getPorteriaNostra()->getMirant().Dot(player->getMirant());

  		// si la pilota esta entre nosaltres i la nostra porteria, si hi ha algun
		// contrari se l'ha d'envoltar --> AvoidObstacle
		if (dot < 0)
		{
			player->getComportament()->AvoidObstacleOn();
		}
		// No sé si tornar-ho a deixar quan no passi ... 
		player->getComportament()->setTarget(player->getPilota()->getPosicio());
		return;
	}
	
	// Estrategia defensiva mes agressiva
	if( player->getDeQueFaig() == JugadorBase::defensa)
	{
		if( player->isPilotaAbastDeRecepcio() &&
		    isSecondInFOVOfFirst(player->getPosicio(), player->getMirant(), player->getPilota()->getPosicio(), 
		    (PI/180)*210.0) && player->isACasa() )
		{
#ifdef PLAYER_STATE_INFO_ON
     		printf("... Defensa %d agressiva\n", player->getID());
#endif
			player->getComportament()->setTarget(player->getPilota()->getPosicio());
			return;
		}
	}

#ifdef PLAYER_STATE_INFO_ON
     		printf("... Jugador %d torna\n", player->getID());
#endif
  	// Si no es el mes proper a la pilota, retorna a casa o ...
	player->getFSM()->canviaEstat(JugadorEstatTornaACasa::Instance());
};

void JugadorEstatPerseguirPilota::Exit(Jugador* player)
{
	// Desactivem el comportament perseguir 
	player->getComportament()->SeekOff();
	player->getComportament()->AvoidObstacleOff();
};

//-----------------   DRIBBLAR EL CONTRARI  ----------------------------
JugadorEstatDribbla* JugadorEstatDribbla::Instance()
{
	static JugadorEstatDribbla instance;
	return &instance;
}

void JugadorEstatDribbla::Enter(Jugador* player)
{
  	// Recordar que controlem la pilota 
	player->getEquip()->setJugadorAmbLaPilota(player);

#ifdef PLAYER_STATE_INFO_ON
  	printf("Jugador %d dribblant\n", player->getID());
#endif	
};

void JugadorEstatDribbla::Execute(Jugador* player)
{
	double dot = player->getEquip()->getPorteriaNostra()->getMirant().Dot(player->getMirant());

  	// si la pilota esta entre nosaltres i la nostra porteria, l'hem de voltar ... 
	// Ho fa amb petits xuts fins que estigui en la direccio correcta 
	if (dot < 0)
	{
		//El jugador ha de rodar perquè està mirant cap a l'altre costat (Pi/4) 
		Punt3 direction = player->getMirant();

    		//calculate the sign (+/-) of the angle between the player heading and the 
    		//facing direction of the goal so that the player rotates around in the 
    		//correct direction
		double angle = PIQUARTS * -1 * player->getEquip()->getPorteriaNostra()->getMirant().Signe(player->getMirant());
		VectorRodaPerOrigen(direction, angle);

		// Ha de controlar la pilota i girar alhora ... 
		// const double KickingForce = 0.8;
		const double KickingForce = player->Jugador_ForcaDribbling()*0.5;

		player->getPilota()->Xuta(direction, KickingForce);
	}
  	// Xuta la pilota cap al camp contrari
	else
	{   if( player->isEsticMarcat() )
		{
			// En comptes d'anar-hi de dret, fem ziga-zaga
			Punt3 BallTarget = AfegirFressaAlXut(player->getPilota()->getPosicio(), 
				                             player->getEquip()->getPorteriaContraria()->getMirant(),
                                                             player->Jugador_Punteria());
			player->getPilota()->Xuta(BallTarget, player->Jugador_ForcaDribbling());
		}
		else
		{
			// Ara segons en quin lloc del camp estem ... 
			if (fabs(player->getEquip()->getPorteriaContraria()->getMig().x - player->getPilota()->getPosicio().x)
                                 > DISTANCIAATACANTSGIREN)
			{
				// Anem endavant (cap al camp contrari)
				player->getPilota()->Xuta(player->getEquip()->getPorteriaNostra()->getMirant(), 
                                player->Jugador_ForcaDribbling());  
			}
			else
			{
				// Girem cap a la porteria perquè hi som a prop 
				Punt3 BallTarget = VectorNormalitza(player->getEquip()->getPorteriaContraria()->getMig() - 
                                                                    player->getPilota()->getPosicio());
				player->getPilota()->Xuta(BallTarget, player->Jugador_ForcaDribbling());  
			}
		}
	}
   	// el jugador ha xutat la pilota pel que l'ha de perseguir 
   	player->getFSM()->canviaEstat(JugadorEstatPerseguirPilota::Instance());
   	return;  
};

void JugadorEstatDribbla::Exit(Jugador* player)
{};

//---------------------   TORNAR A CASA  -------------------------------
JugadorEstatTornaACasa* JugadorEstatTornaACasa::Instance()
{
	static JugadorEstatTornaACasa instance;

	return &instance;
}

void JugadorEstatTornaACasa::Enter(Jugador* player)
{
	player->getComportament()->ArriveOn();

	if (!player->getRegioNormal()->isDins(player->getComportament()->getTarget(), Regio::halfsize))
	{
		player->getComportament()->setTarget(player->getRegioNormal()->getPuntMig());
	}

#ifdef PLAYER_STATE_INFO_ON
  	printf("Jugador %d Tornant a casa (%f,%f)\n", player->getID(), player->getComportament()->getTarget().x, player->getComportament()->getTarget().y);
#endif

};

void JugadorEstatTornaACasa::Execute(Jugador* player)
{
	if (player->getCamp()->isPartitEnJoc())
	{
		// Defensa agressiva: si s'acosta la pilota, el defensa s'hi tira!
		if( player->isPilotaAbastDeRecepcio() && 
	            isSecondInFOVOfFirst(player->getPosicio(), player->getMirant(), player->getPilota()->getPosicio(), 
                    PI/180*210.0) && player->getDeQueFaig()== JugadorBase::defensa)
		{
				// attack ball
				player->getFSM()->canviaEstat(JugadorEstatPerseguirPilota::Instance());
				return;
		}

    		// Si mentre tornem resulta que som els mes propers a la pilota, 
		// ning espera una passada i el porter no te la pilota ... A per ella!
		if ( player->isMesProperAPilota() &&
		   (player->getEquip()->getJugadorDeRecepcio()==NULL) &&
		    !player->getCamp()->isEnMansDelPorter())
		{
			player->getFSM()->canviaEstat(JugadorEstatPerseguirPilota::Instance());
			return;
		}
	}

	// Si el partit està en joc i estem prou a prop del destí ens podem aturar 
	if (player->getCamp()->isPartitEnJoc() &&
            player->getRegioNormal()->isDins(player->getPosicio(),Regio::halfsize))
	{
		player->getComportament()->setTarget( player->getPosicio());
		player->getFSM()->canviaEstat(JugadorEstatEsperant::Instance()); 
	}
	// si el partit no està en joc, ens hem d'acostar al centre de la nostra posició
	else if(!player->getCamp()->isPartitEnJoc() && player->isADesti())
	{
		player->getFSM()->canviaEstat(JugadorEstatEsperant::Instance());
	}
};

void JugadorEstatTornaACasa::Exit(Jugador* player)
{
	player->getComportament()->ArriveOff();
};

//------------------------ ESPERANT  ----------------------------------
JugadorEstatEsperant* JugadorEstatEsperant::Instance()
{
	static JugadorEstatEsperant instance;

	return &instance;
}

void JugadorEstatEsperant::Enter(Jugador* player)
{
#ifdef PLAYER_STATE_INFO_ON
  	printf("Jugador %d entra en espera a (%f,%f)\n", player->getID(), player->getPosicio().x, player->getPosicio().y);
#endif
	if (!player->getCamp()->isPartitEnJoc())
	{
		player->getComportament()->setTarget(player->getRegioNormal()->getPuntMig());
	}
};

void JugadorEstatEsperant::Execute(Jugador* player)
{
  	//si no està allà volia anar, hi acaba d'arribar 
	if (!player->isADesti())
	{
		player->getComportament()->ArriveOn();
		return;
	}
	else
	{
		// Si estem al lloc, només ens posem a mirar la pilota... 
		player->getComportament()->ArriveOff();
		player->setVelocitat(Punt3(0,0,0));
		player->MiraPilota();
		
	}
	// Si no te la pilota pero la tenim nosaltres i aquest jugador
	// esta mes endavant que el que la te demana passada 
	if ( player->getEquip()->TincPilota() &&
		    (!player->isControlantPilota()) &&
		    player->isMesEndavantQueLAtacant() )
	{
#ifdef PLAYER_STATE_INFO_ON
  		printf("... Jugador %d està més endavant --> Demana passada\n", player->getID());
#endif
		player->getEquip()->DemanaPassada(player);
		return;
	}
	
	if (player->getCamp()->isPartitEnJoc())
	{
		// Defensa agressiva: si s'acosta la pilota, el defensa s'hi tira!
		if( player->isPilotaAbastDeRecepcio() && 
		    isSecondInFOVOfFirst(player->getPosicio(), player->getMirant(), player->getPilota()->getPosicio(), PI/180*210.0) && player->getDeQueFaig()== JugadorBase::defensa)
		{
			// attack ball
			player->getFSM()->canviaEstat(JugadorEstatPerseguirPilota::Instance());
			return;
		}

		// Si la pilota està a prop d'aquest jugador I 
		// no espera una passada I el porter no té la pilota 
		// l'anirà a buscar 
		if (player->isMesProperAPilota() &&
		    player->getEquip()->getJugadorDeRecepcio() == NULL  &&
		    !player->getCamp()->isEnMansDelPorter())
		{
			player->getFSM()->canviaEstat(JugadorEstatPerseguirPilota::Instance());
#ifdef PLAYER_STATE_INFO_ON
  		printf("...Jugador %d canvia a perseguir pilota\n", player->getID());
#endif			
			return;
		}
	} 
};

void JugadorEstatEsperant::Exit(Jugador* player)
{};

//-------------------------- XUTA --------------------------------------
JugadorEstatXuta* JugadorEstatXuta::Instance()
{
	static JugadorEstatXuta instance;
	return &instance;
}

void JugadorEstatXuta::Enter(Jugador* player)
{
	// Passar a la imatge de xutar
	// player->setMovimentActual(JUGADORXUTA);

 	// L'equip ha de tenir-nos com a el que controla la pilota 
	player->getEquip()->setJugadorAmbLaPilota(player);
   
        //només podem xutar si no fa poc que ho he fet, altrament persegueix la pilota 
	if (!player->PreparatPerTornarAXutar()) 
	{
		player->getFSM()->canviaEstat(JugadorEstatPerseguirPilota::Instance());
	}
#ifdef PLAYER_STATE_INFO_ON
  	printf("Jugador %d vol xutar\n", player->getID());
#endif	
};

void JugadorEstatXuta::Execute(Jugador* player)
{
	// Calcula el producte vectorial entre la pilota i el vector a on mira el
	// jugador per saber si podem xutar
	Punt3 ToBall = player->getPilota()->getPosicio() - player->getPosicio();
	double dot = player->getMirant().Dot(VectorNormalitza(ToBall));
	
	// No es pot xutar si el porter té la pilota o si està darrera del jugador o 
	// ja s'ha decidit que es farà una passada. Si passa això seguim la pilota 

	if (dot<0)
	{
		if (player->getEquip()->getJugadorDeRecepcio() != NULL ||
			player->getCamp()->isEnMansDelPorter() ) 
		{
#ifdef PLAYER_STATE_INFO_ON
    		printf("... El porter té la pilota / pilota darrere jugador\n");
#endif
		player->getFSM()->canviaEstat(JugadorEstatPerseguirPilota::Instance());
		return;
		}
		// El meu afegitó:
		if (player->isInZonaCalenta() && !player->isEsticMarcat())
		{
#ifdef PLAYER_STATE_INFO_ON
    		printf("...*** Nou Dribbling \n");
#endif
			player->getFSM()->canviaEstat(JugadorEstatDribbla::Instance());
			return;
		}
	}
	/* ---------------  Intent de xutar a porteria ------------------------- */
	// Si es pot xutar, el vector tindrà la posció a on es pot provar el xut 
	Punt3 BallTarget;
  	//Com més davant nostre estigui la pilota més fort xutarem 
	double power = player->Jugador_ForcaXut() * dot;
	// Si el jugador pot fer un gol des d'aquí O 
	// si decideix xutar de totes formes intentarem xutar 
	if (player->getEquip()->PucXutar(player->getPilota()->getPosicio(), power, BallTarget)
		|| (RandFloat() < player->Jugador_XutaIgualment()))
	{
		// S'afegeix fressa al xut perquè no interessa qeu siguin massa bons xutant 
		// la quantitat de fressa es pot ajustar amb player->Jugador_Punteria() 
		BallTarget = AfegirFressaAlXut(player->getPilota()->getPosicio(), BallTarget, player->Jugador_Punteria());

#ifdef PLAYER_STATE_INFO_ON
   		printf("... Jugador %d XUTA a (%f,%f)\n",player->getID(),BallTarget.x, BallTarget.y);
#endif
		
   		//this is the direction the ball will be kicked in
   		Punt3 KickDirection = BallTarget - player->getPilota()->getPosicio();
 		player->getPilota()->Xuta(KickDirection, power);
		//canvia d'estat
		player->getFSM()->canviaEstat(JugadorEstatEsperant::Instance());
   		player->BuscaAjuda();
		return;
	}


	/* ---------------  Intent de passar a un jugador --------------- */
  	//Aquí hi haurà el receptor si se'n troba algún 
	JugadorBase* receiver = NULL;
	power = player->Jugador_ForcaPassada() * dot;
  	// busquem candidats per rebre la passada
	if (player->isEsticMarcat()  &&
	    player->getEquip()->BuscaPassada(player, receiver, BallTarget, power,player->Jugador_DistanciaMinimaPassada()))
	{     
    		//afegir fressa a la passada per fer-ho més "xungo"
		BallTarget = AfegirFressaAlXut(player->getPilota()->getPosicio(), BallTarget,player->Jugador_Punteria());
		Punt3 KickDirection = BallTarget - player->getPilota()->getPosicio();
		   
		// ---- XAVIER: Calcular la força necessaria per xutar
		power = player->getPilota()->ForcaPerMoure(BallTarget, player->Jugador_ForcaPassada());
		player->getPilota()->Xuta(KickDirection, power);
		
#ifdef PLAYER_STATE_INFO_ON
    	printf("... Jugador %d PASSA a %d que és a (%f,%f)\n", player->getID(), receiver->getID(), BallTarget.x, BallTarget.y);
#endif		
		// Avisar al receptor que li enviem la pilota 
		Carter->DispatchMsg(SEND_MSG_IMMEDIATELY,
			            player->getID(),
			            receiver->getID(),
			            Msg_ReceiveBall,
			            &BallTarget);
    		//the player should wait at his current position unless instruced
    		//otherwise  
		player->getFSM()->canviaEstat(JugadorEstatEsperant::Instance());
		player->BuscaAjuda();
      		return;
	}
	else
	{
		//cannot shoot or pass, so dribble the ball upfield
#ifdef PLAYER_STATE_INFO_ON
    		printf("... Jugador %d només pot dribblar\n", player->getID());
#endif	
		player->BuscaAjuda();
		player->getFSM()->canviaEstat(JugadorEstatDribbla::Instance());		
	}
};

void JugadorEstatXuta::Exit(Jugador* player)
{
	// Torna a la imatge per defecte!
	player->setMovimentActual(JUGADORCORRE);
};

//---------------- RECEPTOR DE LA PASSADA ------------------------------
JugadorEstatRebrePasse* JugadorEstatRebrePasse::Instance()
{
	static JugadorEstatRebrePasse instance;

	return &instance;
}

void JugadorEstatRebrePasse::Enter(Jugador* player)
{
 //let the team know this player is receiving the ball
	player->getEquip()->setJugadorDeRecepcio(player);
  //this player is also now the controlling player
	player->getEquip()->setJugadorAmbLaPilota(player);

	// Pot fer dues coses:
	//  1- Anar allà on ha dit el passador que tirara la pilota 
	//  2- Perseguir la pilota 
	//
	// Es tria entre un i l'altre a partir de: 
	//  	- la variable del jugador ChanceOfUsingArriveTypeReceiveBehavior 
	//      - Si hi ha o no un jugador contrari a prop 
	//      - Si estem en la regio "calenta" (1/3 del camp a prop de la 
	//        porteria contraria 
	const double PassThreatRadius = 70.0;

	if (( player->isInZonaCalenta() || RandFloat() < player->Jugador_EsperaOHiVa()) && 
	      !player->getEquip()->isHiHaOponentADistancia(player->getPosicio(), PassThreatRadius))
	{
#ifdef PLAYER_STATE_INFO_ON
    		printf("Jugador %d espera pilota (arribe) a (%f,%f)\n",player->getID(), player->getComportament()->getTarget().x, player->getComportament()->getTarget().y);
#endif
		// Possibilitat 1: Anar allà on han dit que aniria la pilota 
		player->getComportament()->ArriveOn();
	}
	else
	{
#ifdef PLAYER_STATE_INFO_ON
   		printf("Jugador %d espera pilota (pursuit) a (%f,%f)\n",player->getID(), player->getComportament()->getTarget().x, player->getComportament()->getTarget().y);
#endif		
		// Possibilitat 2: Perseguir la pilota alla on va 
		player->getComportament()->PursuitOn();
	}
};

void JugadorEstatRebrePasse::Execute(Jugador* player)
{
  //Si la pilota esta prou a prop o be perdem la pilota, hem de canviar de tactica 
	if (player->isPilotaAbastDeRecepcio() || !player->getEquip()->TincPilota())
	{
		player->getFSM()->canviaEstat(JugadorEstatPerseguirPilota::Instance());
		return;
	}  
	// Si perseguim la pilota, cal actualitzar el desti
	if (player->getComportament()->PursuitIsOn())
	{
#ifdef PLAYER_STATE_INFO_ON
  		printf("Jugador %d canvia espera a (%f,%f)\n", player->getID(), player->getPilota()->getPosicio().x,  player->getPilota()->getPosicio().y);
#endif  
		player->getComportament()->setTarget(player->getPilota()->getPosicio());
	}

	// Si ja estem al desti ja no cal que correm mes. Especialment indicat per la 
	// segona opcio
	if (player->isADesti())
	{
#ifdef PLAYER_STATE_INFO_ON
  		printf("Jugador %d espera el passe aturat a (%f,%f)\n",player->getID(), player->getComportament()->getTarget().x, player->getComportament()->getTarget().y);
#endif  
		player->getComportament()->ArriveOff();
		player->getComportament()->PursuitOff();
		player->MiraPilota();
		player->setVelocitat(Punt3(0,0,0));
	} 
};

void JugadorEstatRebrePasse::Exit(Jugador* player)
{
	// Desactivar-ho tot 
	player->getComportament()->ArriveOff();
	player->getComportament()->PursuitOff();
	player->getEquip()->setJugadorDeRecepcio(NULL);
};

//--------------- DONAR SUPORT A L'ATACANT -----------------------------
JugadorEstatAjudaAtac* JugadorEstatAjudaAtac::Instance()
{
	static JugadorEstatAjudaAtac instance;

	return &instance;
}

void JugadorEstatAjudaAtac::Enter(Jugador* player)
{
	// Passar la imatge a ajuda a l'atac
	player->setMovimentActual(JUGADORAJUDA);

	player->getComportament()->ArriveOn();

	player->getComportament()->setTarget(player->getEquip()->getPuntDeSuport());
  
#ifdef PLAYER_STATE_INFO_ON
  		printf("Jugador %d canvia suport a (%f,%f)\n", player->getID(),player->getEquip()->getPuntDeSuport().x,  player->getEquip()->getPuntDeSuport().y);
#endif  
};

void JugadorEstatAjudaAtac::Execute(Jugador* player)
{
	Punt3 temp;
 	// Si l'equip perd la pilota, torna a casa 
	if (!player->getEquip()->TincPilota())
	{
		player->getFSM()->canviaEstat(JugadorEstatTornaACasa::Instance()); return; 
	} 

	// XAVIER: A vegades quan fan el passe i no arriba, es queden idiotes
	// Per tant, si és el més proper, ningú espera passada o pilota aturada
	if (player->isMesProperAPilota() && ( player->isPilotaAbastDeRecepcio() || 
										  player->getEquip()->getJugadorDeRecepcio()!=NULL || 
		                                  player->getPilota()->getVelocitat()==0))
	{
		player->getFSM()->canviaEstat(JugadorEstatPerseguirPilota::Instance()); return;
	}

	// si ha canviat el millor punt, canviem el desti	
	if (player->getEquip()->getPuntDeSuport() != player->getComportament()->getTarget())
	{    
		player->getComportament()->setTarget(player->getEquip()->getPuntDeSuport());
		player->getComportament()->ArriveOn();
#ifdef PLAYER_STATE_INFO_ON
  		printf("Jugador %d canvia suport a (%f,%f)\n", player->getID(),player->getEquip()->getPuntDeSuport().x,  player->getEquip()->getPuntDeSuport().y);
#endif  
	}

  	// Si te un xut a gol i l'atacant li pot passar la pilota l'hauria de rebre
	if( player->getEquip()->PucXutar(player->getPosicio(), player->Jugador_ForcaXut(), temp))
	{
#ifdef PLAYER_STATE_INFO_ON
  	printf("Jugador %d pot fer un gol --> Demana passada\n", player->getID());
#endif
		player->getEquip()->DemanaPassada(player);
	}

  	// Si esta a un punt de suport i tenim la pilota, espera mirant-la 
	if (player->isADesti())
	{
		player->getComportament()->ArriveOff();
        
    		//the player should keep his eyes on the ball!
		player->MiraPilota();
		player->setVelocitat(Punt3(0.0,0.0,0.0));
		// Si no ens marquen a mes demana la passada 
		if (!player->isEsticMarcat())
		{
#ifdef PLAYER_STATE_INFO_ON
  	printf("Jugador %d esta a un punt de suport desmarcat --> Demana passada\n", player->getID());
#endif
			player->getEquip()->DemanaPassada(player);
		}
	}

};

void JugadorEstatAjudaAtac::Exit(Jugador* player)
{
 //set supporting player to null so that the team knows it has to 
  //determine a new one.
	player->getEquip()->setJugadorDonantSuport(NULL);
	player->getComportament()->ArriveOff();
	// Tornem la imatge al defecte
	player->setMovimentActual(JUGADORCORRE);
};


//--------------- ANAR A FER LA SACADA --------------------------
// El més proper a la pilota ha d'anar a treure 
// FALTA: i dos dels altres, posar-se a prop per rebre la passada.
//
// S'ha de poder distingir entre el tipus de joc que fara despres
// Per ara ho xuta tot.
//--------------------------------------------------------------
JugadorEstatAnarASaque* JugadorEstatAnarASaque::Instance()
{
	static JugadorEstatAnarASaque instance;
	return &instance;
}

void JugadorEstatAnarASaque::Enter(Jugador* player)
{
	// El jugador es mourà fins que arribi a la pilota
	player->getComportament()->ArriveOn();
	player->getComportament()->setTarget(player->getPilota()->getPosicio());
#ifdef PLAYER_STATE_INFO_ON
  		printf("Jugador %d va a sacar la pilota\n", player->getID());
#endif  
}

void JugadorEstatAnarASaque::Execute(Jugador* player)
{
	// Anar cap a la pilota i fora del camp i ...
	if (player->isMesProperAPilota())
	{
		// Aqui es on li dic que ha de perseguir 
		player->getComportament()->setTarget(player->getPilota()->getPosicio());
	}
	// Si està dins del camp (faltes, saques de porteria...): isADesti
	// Si és fora de banda (hem de sortir del camp) 
	if (player->isADesti() || 
		(player->getCamp()->getTerrenyDeJoc()->isFora(player->getPosicio()) && player->getCamp()->isFora()))
	{
		//player->MiraReves();
		player->setVelocitat(Punt3(0.0,0.0,0.0));
		// Xutar
		player->getFSM()->canviaEstat(JugadorEstatSacaDeBanda::Instance()); return;
	}

}


void JugadorEstatAnarASaque::Exit(Jugador* player)
{
	player->getComportament()->ArriveOff();
	player->getEquip()->setJugadorAmbLaPilota(player);
	// Xapussa per evitar el problema de que la juguen encara que
	// no s'hagi de fer
	// player->getCamp()->setLaTeElPorter(false);
}


//--------------- SACADA DE BANDA --------------------------------
// Fa efectiu el llançament de banda. Per això busca el company
// que considera més ben col.locat i li dóna la pilota.
//
//
// Nota: No puc fer servir el xut perquè intenta driblar
JugadorEstatSacaDeBanda* JugadorEstatSacaDeBanda::Instance()
{
	static JugadorEstatSacaDeBanda instance;
	return &instance;
}

void JugadorEstatSacaDeBanda::Enter(Jugador* player)
{
	//Hauria de mirar cap al camp
	player->MiraReves();
	player->getPilota()->setPosicio(player->getPosicio());
#ifdef PLAYER_STATE_INFO_ON
  		printf("Jugador %d treu de banda\n", player->getID());
#endif  
}

void JugadorEstatSacaDeBanda::Execute(Jugador* player)
{
	Punt3 BallTarget;
	double power = player->Jugador_ForcaPassada();

	// Ha de fer la passada al millor col.locat.
  	//Aquí hi haurà el receptor si se'n troba algún 
	JugadorBase* receiver = NULL;
//	power = FORCAPASSADAMAXIMA * dot;
  	// busquem candidats per rebre la passada
	if (player->getEquip()->BuscaPassada(player, receiver, BallTarget, power,player->Jugador_DistanciaMinimaPassada()))
	{ 
    		//afegir fressa a la passada per fer-ho més "xungo"
		BallTarget = AfegirFressaAlXut(player->getPilota()->getPosicio(), BallTarget,player->Jugador_Punteria());
		Punt3 KickDirection = BallTarget - player->getPilota()->getPosicio();
		// ---- XAVIER: Calcular la força necessaria per xutar
		power = player->getPilota()->ForcaPerMoure(BallTarget, player->Jugador_ForcaPassada());
		// Per fer-la saltar
		KickDirection.h=8.0;
		player->getPilota()->Xuta(KickDirection, power);
		
#ifdef PLAYER_STATE_INFO_ON
    		printf("... Jugador %d treu la pilota de banda\n", player->getID());
#endif		
		// Avisar al receptor que li enviem la pilota 
		Carter->DispatchMsg(SEND_MSG_IMMEDIATELY,
			            player->getID(),
			            receiver->getID(),
			            Msg_ReceiveBall,
			            &BallTarget);
    		//the player should wait at his current position unless instruced
    		//otherwise  
		player->getFSM()->canviaEstat(JugadorEstatEsperant::Instance());
		player->BuscaAjuda();
      	return;
	}
/*	else
	{

		double LlargadaMigCamp = player->getCamp()->getTerrenyDeJoc()->getAmplada()*0.5;
		// Sóm massa lluny, per tant canviem si cal ...
		if (VectorDistanciaSq(player->getPosicio(), player->getEquip()->getPorteriaNostra()->getMig()) 
			> LlargadaMigCamp*LlargadaMigCamp)
		{
			// si no estic a prop d'ell, anem enrere perquè sino no la passa mai...
			if (player->getEquip()->getFSM()->isEnEstat(*EquipEstatDefensant::Instance())
			{
				player->getEquip()->getFSM()->canviaEstat(EquipEstatAtacant::Instance());
			}
		}
		else
		{
			if (player->getEquip()->getFSM()->isEnEstat(*EquipEstatAtacant::Instance())
			{
				player->getEquip()->getFSM()->canviaEstat(EquipEstatDefensant::Instance());
			}
		}
	}
*/
}


void JugadorEstatSacaDeBanda::Exit(Jugador* player)
{
	// La pilota torna a estar en joc
	player->getCamp()->setPartitEnJoc(true);

}

//--------------- CONTROLAT PER L'USUARI --------------------------------
// 
// 
//
//
// 
JugadorEstatUsuari* JugadorEstatUsuari::Instance()
{
	static JugadorEstatUsuari instance;
	return &instance;
}

void JugadorEstatUsuari::Enter(Jugador* player)
{
	// Potser marcar-lo ? A terra de colors!

}

void JugadorEstatUsuari::Execute(Jugador* player)
{
	// 0 --- Si la pilota no està  en joc, tornar a casa
	if (!player->getCamp()->isPartitEnJoc())
	{
		player->getFSM()->canviaEstat(JugadorEstatTornaACasa::Instance());
	}
	// 0 --- Si NO sóc el més proper a la pilota --> canviar el controlat per l'usuari per un altre
	if (!player->isMesProperAPilota())
	{
		player->getFSM()->canviaEstat(JugadorEstatEsperant::Instance());
		// COM Posar el més proper a la pilota??
		Jugador *j = static_cast<Jugador*>(player->getEquip()->getMesProperAPilota());
		j->getFSM()->canviaEstat(JugadorEstatUsuari::Instance());
	}

	// 1 --- Moure el jugador en la direcció de la tecla actual (Segurament haurà de girar)
	

	// 2 --- Si la pilota és a l'abast de xut --> Condueix Xuta(fluix)
	//                     (si ha xutat)--> Xuta: segons sigui
	if (player->isPilotaAbastDeXut())
	{
		// Conduir o xutar: Falta decidir com recuperar les tecles i xutar en funció de la darrera 
		// tecla premuda
		player->getPilota()->Xuta(player->getMirant(), player->Jugador_ForcaDribbling());  
	}

	// 3 --- He de fer alguna cosa més??
	// 

}


void JugadorEstatUsuari::Exit(Jugador* player)
{
	// Si l'havia marcat --> desmarcar-lo

	// Buidar el buffer de tecles??

}



