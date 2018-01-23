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
#ifndef CALCULADORAPUNTSSUPORT_H
#define CALCULADORAPUNTSSUPORT_H

#include <vector>

#include "Regio.h"
#include "punt3.h"

#define ACTUALITZACIOPUNTSSUPORT 3

/* 

Pel cacul de punts de suport :

NumSweetSpotsX                  13
NumSweetSpotsY                  6
//these values tweak the various rules used to calculate the support spots
*/
#define ESPOTPASSARSCORE       2.0
#define POTMARCARSCORE         1.0
#define DISTANCIAPASSADORSCORE 2.0
#define MASSAPROPSCORE         0.0
#define DAVANTATACANTSCORE     0.0
#define DISTANCIAOPTIMA		 600.0


class JugadorBase;
class Porteria;
class Pilota;
class Equip;
class Regulator;

/**
	@author Xavier Sala Pujolar <utrescu@xaviersala.net>
*/
class CalculadoraPuntsSuport{
  	// Una estructura per guardar els valors dels punts de suport
	struct PuntSuport
	{
		Punt3 m_vPos;
		double    m_dScore;

		PuntSuport(Punt3 pos, double value):m_vPos(pos), m_dScore(value) {}
	};

private:
	Equip*               m_pTeam;

	std::vector<PuntSuport>  m_Spots;

  	// un punter al millor punt de suport 
	PuntSuport* m_pBestSupportingSpot;

  	// Un regulador per evitar que es calculi continuament 
	Regulator* m_pRegulator;

public:
  
	CalculadoraPuntsSuport(int numX, int numY, Equip* team);
	~CalculadoraPuntsSuport();

  	// Pinta els rectangles de la mida del valor dels punts de suport 
	// El més bo té un requade groc a dintre 
	void Pinta()const;

  	//this method iterates through each possible spot and calculates its
  	//score.
	Punt3 DetermineBestSupportingPosition();

  	//returns the best supporting spot if there is one. If one hasn't been
  	//calculated yet, this method calls DetermineBestSupportingPosition and
  	//returns the result.
	Punt3 GetBestSupportingSpot();
};

#endif
