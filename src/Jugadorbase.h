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
#ifndef JUGADORBASE_H
#define JUGADORBASE_H

#include "Autolist.h"
#include "Entitatmobil.h"
#include "steeringbehaviors.h"
#include "Equip.h"
#include "Pantalla.h"

// ---- CONSTANTS QUE PODRIEN SER VARIABLES DELS JUGADORS ---- 
#define FREQUENCIAXUTSPERSEGON              8
#define DISTANCIAATACANTSGIREN            475 // 468

class Regio;
class Equip;
/**
	@author Xavier Sala <xavier@norimaki.xaviersala.net>
*/
class JugadorBase : public EntitatMobil,
                   public AutoList<JugadorBase>
{
public:	
	enum jugo_de{ porter=1, defensa=2, migcampista=3, davanter=4};
protected:
	jugo_de DeQueJugo;
	// Posicio en la que estem quan comença el partit
	// nomes serveix per posicionar-lo inicialment i despres dels gols
	Regio* RegioInicial;
	// Zona del camp a la que juga el jugador
	Regio* RegioNormal;
	// Classe de comportaments
	SteeringBehaviors* ComportamentJugador; // Behaviors
	// Punter al nostre equip
	Equip *Nosaltres;
	// Distancia del jugador a la pilota al quadrat (al cap i a la fi es el mateix que sense quadrat) 
	double  DistanciaAPilota;

	// ---- Caracteristiques especifiques dels jugadors ----
	double JUG_VelocitatSense; 
	double JUG_VelocitatAmb; 
	// xuts
	double JUG_ForcaXut; 
	double JUG_Punteria; 
	double JUG_XutaIgualment;
	// passades
	double JUG_DistanciaMinimaPassada;
	double JUG_ForcaPassada;
	double JUG_EsperaOHiVa;
	double JUG_ZonaComfort;
	// dribbling
	double JUG_ForcaDribbling;
	// porter
	double POR_SurtAPerPilota;
	double POR_Seguiment;
	double POR_PilotaAbastPorter;
	double POR_Reaccio;
	// Paràmetres generals
	double JUG_DistanciaRecepcio;
	double JUG_PilotaAbastXut;
	double JUG_PilotaAbastDesti;
	// -----------------------------------------------------


public:
	JugadorBase(Equip* equ, SDL_Rect casa, Punt3 Mirant, double radi, Punt3 Veloc, double massa, 
		    double max_force, double maxvelocamb, double maxvelocsense, double max_turn, double xut, double punteria,
			double provaxut, double passades, double zconf, double dista, double espera, double dribling, jugo_de deq);
	
	~JugadorBase();
	// Torna quin paper fa el jugador (porter, defensa, etc..)
	jugo_de getDeQueFaig()const{return DeQueJugo;}
	// Ens torna un punter al nostre equip 
	Equip* getEquip() const { return Nosaltres; }
	// Un punter al terreny de joc 
	Pantalla* getCamp() const { return getEquip()->getCamp(); }
	
	Regio* getRegioInicial() { return RegioInicial; }
	void setRegioInicial(Regio* reg) { RegioInicial = reg; }
	Regio* getRegioNormal() { return RegioNormal; }
	void setRegioNormal(SDL_Rect reg) 
	{ 
		if (RegioNormal!=NULL) delete RegioNormal;
		RegioNormal = new Regio(0,reg); 
	}
	
	void setTornaARegioInicial() { RegioNormal = RegioInicial;}
	
	// ----------------------- Tractament de la Pilota -------------------------
	// Ens torna un punter a la pilota (per saber on es)
	Pilota* const getPilota()const;
	
	void setDistanciaAPilota(double dis) { DistanciaAPilota=dis; }
	double getDistanciaAPilota() { return DistanciaAPilota;}
	
	void MiraPilota() { 
		// printf("####### %d Mira a (%f,%f,%f) -->",getID(),MirantA.x,MirantA.y,MirantA.h);
		RodaCapA(getPilota()->getPosicio());
		// printf("(%f,%f,%f)\n",MirantA.x,MirantA.y,MirantA.h);
	};
	
	//returna si el porter pot retenir la pilota 
	bool isPilotaAbastPorter() const; 
	// retorna si podem xutar la pilota
	bool isPilotaAbastDeXut() const;
	
	//returns true if a ball comes within range of a receiver
	bool isPilotaAbastDeRecepcio()const;
	// Diu si el jugador es el mes proper a la pilota del nostre equip 
	bool isMesProperAPilota() const;
	// Diu si el jugador es el mes proper de TOTS a la pilota
	bool isElMesProperDelCampAPilota()const;
	// Ens pot servir per saber si estem en fora de joc, ja que som els mes propers a la porteria 
	// contraria (fins i tot mes que els contraris) 
	bool isElMesProperDelCampAPorteriaContraria() const;
	// Diu si estem controlant la pilota 
	bool isControlantPilota()const;
	// Estic mes endavant que el que porta la pilota 
	bool isMesEndavantQueLAtacant()const;
	// Estic envoltat o no puc passar 
	bool isEsticMarcat() const;
	// -------------- Pilota end -----------------------------------------------
	// Ens diu si la posicio especificada esta davant del nostre jugador 
	bool isPosicioDavantNostra(Punt3 position)const;
	// Estem en una zona propera a la porteria contraria 
	bool isInZonaCalenta()const;
	
	SteeringBehaviors* getComportament() { return ComportamentJugador; }
	bool isADesti();
	
	bool isACasa();
			
	double DistanciaPorteriaContraria()const;
	double DistanciaPorteriaNostra()const;	
	// Fara mirar al jugador cap al punt mig
	void MiraReves()
	{
		RodaCapASenseLimit(getCamp()->getTerrenyDeJoc()->getPuntMig());
	}

	// Fara mirar al jugador cap a l'objectiu que tingui
	void MiraObjectiu() 
	{
		setMirant(VectorNormalitza(getComportament()->getTarget() - getPosicio()));	
	}
	
	// --------  Caracteristiques dels jugadors  ------------------------
	// velocitats
	double Jugador_VelocitatSense() const { return JUG_VelocitatSense; }
	double Jugador_VelocitatAmb() const { return JUG_VelocitatAmb; }
	double Jugador_MaxGir() const { return maximaRotacio; }
	// xuts
	double Jugador_ForcaXut() const { return JUG_ForcaXut; }
	double Jugador_Punteria() const { return JUG_Punteria; }
	double Jugador_XutaIgualment() const { return JUG_XutaIgualment; }
	// passades
	double Jugador_DistanciaMinimaPassada() { return JUG_DistanciaMinimaPassada; }
	double Jugador_ForcaPassada() const { return JUG_ForcaPassada; }
	double Jugador_EsperaOHiVa() const { return JUG_EsperaOHiVa; }
	double Jugador_ZonaComfort() const { return JUG_ZonaComfort; }
	// dribbling
	double Jugador_ForcaDribbling() const { return JUG_ForcaDribbling; }
	// Porter
	double Porter_SurtAPerPilota() const { return POR_SurtAPerPilota; }
	double Porter_Seguiment() const { return POR_Seguiment; }
	double Porter_Reaccio() const { return POR_Reaccio; }
	
	// ------------------------------------------------------------------
	
	// El poso per fer proves, s'ha d'eliminar
//	void Pinta(SDL_Surface *p) {};
	
	void BuscaAjuda();
};

#endif
