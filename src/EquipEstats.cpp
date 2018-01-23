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
#include "EquipEstats.h"
#include "JugadorMissatges.h"
#include "Equip.h"

// #define DEBUG_TEAM_STATES

void setEstrategia(Equip* team, int tipusEstrategia)
{
}

//------------------------ ATACANT !!! ---------------------------------
EquipEstatAtacant* EquipEstatAtacant::Instance()
{
	static EquipEstatAtacant instance;
	return &instance;
}

void EquipEstatAtacant::Enter(Equip* team)
{
#ifdef DEBUG_TEAM_STATES
	debug  
	printf("EQUIP %s Entra en estat d'Atac\n", team->getNomEquip().c_str());
#endif

	team->PassarAPosicionsAtacants();
	team->CanviaObjectiusDelsAturats();
};

void EquipEstatAtacant::Execute(Equip* team)
{
  	// Si ja no tenim la pilota, hem de passar a defensar!
	if (!team->TincPilota())
	{
		team->getFSM()->canviaEstat(EquipEstatDefensant::Instance()); 
		return;
	}

 	//Calcula cap a on s'han de moure els ajudants
	team->TriaLaMillorPosicioDeSuport();
};

void EquipEstatAtacant::Exit(Equip* team)
{
 	// Ja no tenim gent de suport. O no ens calen
	team->setJugadorDonantSuport(NULL);
	team->setJugadorDeRecepcio(NULL);
};

//------------------------ DEFENENT !!! --------------------------
EquipEstatDefensant* EquipEstatDefensant::Instance()
{
	static EquipEstatDefensant instance;
	return &instance;
}

void EquipEstatDefensant::Enter(Equip* team)
{
#ifdef DEBUG_TEAM_STATES
  	printf("EQUIP %s Entra en estat de Defensa\n", team->getNomEquip().c_str());
#endif
	team->PassarAPosicionsDefensives();
	team->CanviaObjectiusDelsAturats();

};

void EquipEstatDefensant::Execute(Equip* team)
{
  //if in control change states
	if (team->TincPilota())
	{
		team->getFSM()->canviaEstat(EquipEstatAtacant::Instance()); 
		return;
	}
};

void EquipEstatDefensant::Exit(Equip* team)
{};

//------------------ TORNAR A CASA --------------------------
EquipEstatTornaPosicioInicial* EquipEstatTornaPosicioInicial::Instance()
{
	static EquipEstatTornaPosicioInicial instance;
	return &instance;
}

void EquipEstatTornaPosicioInicial::Enter(Equip* team)
{
#ifdef DEBUG_TEAM_STATES
  	printf("EQUIP %s Torna a posicio inicial\n",team->getNomEquip().c_str() );
#endif
 //reset key player pointers
	team->setJugadorAmbLaPilota(NULL);
	team->setJugadorDonantSuport(NULL);
	team->setJugadorDeRecepcio(NULL);
	team->setJugadorMesProperAPilota(NULL);

	team->PassarAPosicionsDefensives();
  //send Msg_GoHome to each player.
	team->TothomACasa();

};

void EquipEstatTornaPosicioInicial::Execute(Equip* team)
{
  	// esperem que tothom hagi arribat per tornar a jugar de nou 
	if (team->isTothomACasa() && team->getContraris()->isTothomACasa())
	{
		team->getFSM()->canviaEstat(EquipEstatDefensant::Instance());
	}
};

void EquipEstatTornaPosicioInicial::Exit(Equip* team)
{
	team->getCamp()->PosicionaMig();
	if (team->isQuiHaDeSacar())
	{
		team->SacaFalta();
	}
};
