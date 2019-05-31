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
#ifndef EQUIP_H
#define EQUIP_H

/**
	@author Xavier Sala <utrescu@xaviersala.net>
*/

#define NumSweetSpotsX 13
#define NumSweetSpotsY 6
#define NumSupportSpotsX 10
#define NumSupportSpotsY 6

#include <vector>

class Porteria;
class JugadorBase;
class Jugador;
// class Pantalla;
class Porter;
class Pilota;
// class CalculadoraPuntsSuport;

#include "maquinaestats.h"
#include "Pantalla.h"
#include "CalculadoraPuntsSuport.h"

/*
        ___________________________ 
       | 0           |             | 
       |___       O  |          ___|  
       |_  |  O      |         |  _| 
      .| |O|.       ,|.       .| | |. 
      || | | )O   O( | )     ( | | || 
      '|_|O|'       `|'       `| |_|'  
       |___|  O      |         |___| 
       |          O  |             |  
       |__O__________|_____________|  

*/

class Equip
{
public:
	enum quin_equip
	{
		local = 0,
		visitant = 1
	};

private:
	MaquinaEstats<Equip> *maquinaEstats;

	// Ens diu si som els LOCAL o el VISITANT
	quin_equip Soc;

	//punter als jugadors de l'equip
	std::vector<JugadorBase *> Jugadors;
	Regio PosicionsAtacants[8];
	Regio PosicionsDefensives[8];

	// El camp
	Pantalla *Camp;

	// Les porteries
	Porteria *PorteriaContraria;
	Porteria *PorteriaNostra;

	//L'equip contrari
	Equip *Contraris;
	void CrearJugadors(const char *nomfitxer, double Escala);

	double DistanciaAlaPilotaDelMillor;
	int ProvesPerXutar;

	// Es crida a cada passada i calcula el nostre jugador més proper a la pilota
	void CalculaMesProperAPilota();
	// Pel fora de joc
	void CalculaContrariMesProperASevaPorteria();
	// Proves per trobar un bon xut
	int getProvesPerTrobarXut() const { return ProvesPerXutar; }

	// El jugador d'aquest equip que esta mes proper a la pilota
	// SEMPRE n'hi ha un
	JugadorBase *JugadorMesProperALaPilota;
	// El jugador de l'equip contrari mes proper a la pilota
	// SEMPRE n'hi ha un
	JugadorBase *JugadorContrariMesProperASevaPorteria;
	// El jugador que tela pilota
	// Nomes n'hi hauria d'haver un entre els dos equips.
	JugadorBase *JugadorAmbLaPilota;
	// Si estem atacant, hi pot haver un jugador que dona suport a l'atac
	JugadorBase *JugadorDonantSuport;
	// Un jugador que esta esperant la passada
	JugadorBase *JugadorDeRecepcio;

	// Serveix per quan ataquem, busca el millor lloc per passar la pilota
	CalculadoraPuntsSuport *PuntsSuport;

public:
	Equip(const char *nomfitxer, Porteria *nostra, Porteria *seva, Pantalla *pant, quin_equip costat, double Escala);

	~Equip();

	void Mou();
	void Pinta(SDL_Surface *p);

	std::string getNomEquip() const
	{
		if (Soc == local)
			return "local";
		return "visitant";
	}

	quin_equip getQuinEquip() { return Soc; }

	const std::vector<JugadorBase *> &Membres() const { return Jugadors; }

	MaquinaEstats<Equip> *getFSM() const { return maquinaEstats; }

	Porteria *const getPorteriaNostra() const { return PorteriaNostra; }
	Porteria *const getPorteriaContraria() const { return PorteriaContraria; }

	Pantalla *const getCamp() const { return Camp; }

	Equip *const getContraris() const { return Contraris; }
	void setContraris(Equip *Cont) { Contraris = Cont; }
	// Ens diu si hi ha un oponent dins del radi determinat
	bool isHiHaOponentADistancia(Punt3 player, double radi);

	Pilota *getPilota() const { return getCamp()->getPilota(); }
	double getDistanciaAPilotaSq() const { return DistanciaAlaPilotaDelMillor; }

	bool TincPilota() const
	{
		if (JugadorAmbLaPilota)
			return true;
		else
			return false;
	}
	void PerdudaPilota()
	{
		JugadorAmbLaPilota = NULL;
		JugadorDeRecepcio = NULL;
	}
	JugadorBase *getJugadorAmbLaPilota() const { return JugadorAmbLaPilota; }
	void setJugadorAmbLaPilota(JugadorBase *player)
	{
		JugadorAmbLaPilota = player;
		getContraris()->PerdudaPilota();
	}

	JugadorBase *getJugadorPerID(int id) const;

	JugadorBase *getMesProperAPilota() const { return JugadorMesProperALaPilota; }
	void setJugadorMesProperAPilota(JugadorBase *player) { JugadorMesProperALaPilota = player; }

	JugadorBase *getContrariMesProperASevaPorteria() const { return JugadorContrariMesProperASevaPorteria; }
	void setContrariMesProperASevaPorteria(JugadorBase *player) { JugadorContrariMesProperASevaPorteria = player; }

	//calculates the best supporting position and finds the most appropriate
	//attacker to travel to the spot
	JugadorBase *BuscaMillorAtacantDeSuport();
	JugadorBase *getJugadorDonantSuport() const { return JugadorDonantSuport; }
	void setJugadorDonantSuport(JugadorBase *player) { JugadorDonantSuport = player; }
	Punt3 getPuntDeSuport() { return PuntsSuport->GetBestSupportingSpot(); }

	JugadorBase *getJugadorDeRecepcio() const { return JugadorDeRecepcio; }
	void setJugadorDeRecepcio(JugadorBase *player) { JugadorDeRecepcio = player; }

	void setPosicioNormal(int plyr, Regio region) const;
	void PassarAPosicionsAtacants();
	void PassarAPosicionsDefensives();

	void TriaLaMillorPosicioDeSuport() const { PuntsSuport->DetermineBestSupportingPosition(); }
	// void DetermineBestSupportingPosition()const{m_pSupportSpotCalc->DetermineBestSupportingPosition();}

	// Miro si el contrari pot interceptar la passada
	bool isPassadaLliureDelContrari(Punt3 from, Punt3 target, const JugadorBase *const receiver,
									const JugadorBase *const opp, double PassingForce) const;
	// La passada es pot fer sense perill
	bool isPassadaLliureTotsContraris(Punt3 from, Punt3 target, const JugadorBase *const receiver,
									  double PassingForce) const;
	// Es demana que es passi la pilota al jugador especificat
	void DemanaPassada(JugadorBase *player);
	bool BuscaPassada(const JugadorBase *const passer, JugadorBase *&receiver,
					  Punt3 &PassTarget, double power, double MinPassingDistance) const;
	// Busca tres possibilitats de passada per veure on es millor passar la pilota
	// Directa, als dos costats (pero no gaire lluny)
	bool TriaMillorPassadaAlReceptor(const JugadorBase *const passer, const JugadorBase *const receiver,
									 Punt3 &PassTarget, double power) const;

	bool PucXutar(Punt3 BallPos, double power, Punt3 &ShotTarget) const;

	void CanviaObjectiusDelsAturats();

	// ------------------- Altres histories  -------------------
	//returns false if any of the team are not located within their home region
	void TothomACasa() const;
	void PorterACasa() const;
	bool isTothomACasa();

	bool isQuiHaDeSacar() { return getCamp()->getQuiHaDeSacar() == Soc; }
	void SacaFalta();		 // Buscar el jugador que ha de sacar i dir-li que ho faci
	void CridaElsDefenses(); // Perquè el porter els faci tornar

	// ----------- Resultat dels xuts  -----------
	void HemMarcat() const; // Gol!
	void PilotaFora();		// Fora de porteria
	void PilotaForaBanda(); // Fora de banda
	void PilotaCorner();	// Còrner
};

#endif
