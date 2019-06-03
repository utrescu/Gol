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
#include "CalculadoraPuntsSuport.h"

#include "Jugadorbase.h"
#include "Porteria.h"
#include "Pilota.h"
// #include "constants.h"
#include "Regulator.h"
#include "Equip.h"
// #include "ParamLoader.h"
#include "Pantalla.h"

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

// #include "debug/DebugConsole.h"

//------------------------------- dtor ----------------------------------------
//-----------------------------------------------------------------------------
CalculadoraPuntsSuport::~CalculadoraPuntsSuport()
{
	delete m_pRegulator;
	m_pRegulator = NULL;
}

//------------------------------- ctor ----------------------------------------
//-----------------------------------------------------------------------------
CalculadoraPuntsSuport::CalculadoraPuntsSuport(int numX, int numY, Equip *team) : m_pBestSupportingSpot(NULL),
																				  m_pTeam(team)
{
	const Regio *PlayingField = team->getCamp()->getTerrenyDeJoc();

	//calculate the positions of each sweet spot, create them and
	//store them in m_Spots
	double HeightOfSSRegion = PlayingField->getAltura() * 0.8;
	double WidthOfSSRegion = PlayingField->getAmplada() * 0.9;
	double SliceX = WidthOfSSRegion / numX;
	double SliceY = HeightOfSSRegion / numY;

	double left = PlayingField->getMinX() + (PlayingField->getAmplada() - WidthOfSSRegion) / 2.0 + SliceX / 2.0;
	double right = PlayingField->getMaxX() - (PlayingField->getAmplada() - WidthOfSSRegion) / 2.0 - SliceX / 2.0;
	double top = PlayingField->getMinY() + (PlayingField->getAltura() - HeightOfSSRegion) / 2.0 + SliceY / 2.0;

	for (int x = 0; x < (numX / 2) - 1; ++x)
	{
		for (int y = 0; y < numY; ++y)
		{
			if (m_pTeam->getQuinEquip() == Equip::visitant)
			{
				m_Spots.push_back(PuntSuport(Punt3(left + x * SliceX, top + y * SliceY, 0.0), 0.0));
			}

			else
			{
				m_Spots.push_back(PuntSuport(Punt3(right - x * SliceX, top + y * SliceY, 0.0), 0.0));
			}
		}
	}

	//create the regulator per fer que nomes  els calculi un cop cada segon
	// aixo ho fa perque no carregar massa els calculs a cada passada
	m_pRegulator = new Regulator(ACTUALITZACIOPUNTSSUPORT);
}

//--------------------------- DetermineBestSupportingPosition -----------------
//
//  see header or book for description
//-----------------------------------------------------------------------------
Punt3 CalculadoraPuntsSuport::DetermineBestSupportingPosition()
{
	//only update the spots every few frames
	if (!m_pRegulator->isReady() && m_pBestSupportingSpot)
	{
		return m_pBestSupportingSpot->m_vPos;
	}

	//reset the best supporting spot
	m_pBestSupportingSpot = NULL;

	double BestScoreSoFar = 0.0;

	std::vector<PuntSuport>::iterator curSpot;

	for (curSpot = m_Spots.begin(); curSpot != m_Spots.end(); ++curSpot)
	{
		//first remove any previous score. (the score is set to one so that
		//the viewer can see the positions of all the spots if he has the
		//aids turned on)
		curSpot->m_dScore = 1.0;

		//Test 1. is it possible to make a safe pass from the ball's position
		//to this position?
		if (m_pTeam->isPassadaLliureTotsContraris(m_pTeam->getJugadorAmbLaPilota()->getPosicio(),
												  curSpot->m_vPos, NULL, m_pTeam->getJugadorAmbLaPilota()->Jugador_ForcaPassada()))
		{
			curSpot->m_dScore += ESPOTPASSARSCORE;
		}

		//Test 2. Determine if a goal can be scored from this position
		Punt3 temp;
		// No n'estic segur de fer servir la força correcta ... Perque no se qui hi anira a buscar-lo
		if (m_pTeam->PucXutar(curSpot->m_vPos, m_pTeam->getJugadorAmbLaPilota()->Jugador_ForcaXut(), temp))
		{
			curSpot->m_dScore += POTMARCARSCORE;
		}

		//Test 3. calculate how far this spot is away from the controlling
		//player. The further away, the higher the score. Any distances further
		//away than OptimalDistance pixels do not receive a score.
		if (m_pTeam->getJugadorDonantSuport())
		{
			double dist = VectorDistancia(m_pTeam->getJugadorAmbLaPilota()->getPosicio(),
										  curSpot->m_vPos);
			double temp = fabs(DISTANCIAOPTIMA - dist);
			if (temp < DISTANCIAOPTIMA)
			{
				//normalize the distance and add it to the score
				curSpot->m_dScore += DISTANCIAPASSADORSCORE *
									 (DISTANCIAOPTIMA - temp) / DISTANCIAOPTIMA;
			}
		}

		//check to see if this spot has the highest score so far
		if (curSpot->m_dScore > BestScoreSoFar)
		{
			BestScoreSoFar = curSpot->m_dScore;
			m_pBestSupportingSpot = &(*curSpot);
		}
	}

	return m_pBestSupportingSpot->m_vPos;
}

//------------------------------- GetBestSupportingSpot -----------------------
//-----------------------------------------------------------------------------
Punt3 CalculadoraPuntsSuport::GetBestSupportingSpot()
{
	if (m_pBestSupportingSpot)
	{
		return m_pBestSupportingSpot->m_vPos;
	}
	else
	{
		return DetermineBestSupportingPosition();
	}
}

void CalculadoraPuntsSuport::Pinta() const
{
	SDL_Rect dest, orig;

	SDL_Surface *p = m_pTeam->getCamp()->getSuperficie();
	SDL_Rect Visio = m_pTeam->getCamp()->getZonaVisible();

	Uint32 blanc = SDL_MapRGB(p->format, 255, 255, 255);
	Uint32 vermell = SDL_MapRGB(p->format, 255, 0, 0);

	for (unsigned int spt = 0; spt < m_Spots.size(); ++spt)
	{
		orig.x = m_Spots[spt].m_vPos.x + m_Spots[spt].m_dScore * 0.25;
		orig.y = m_Spots[spt].m_vPos.y + m_Spots[spt].m_dScore * 0.25;
		orig.w = m_Spots[spt].m_dScore * 2;
		orig.h = m_Spots[spt].m_dScore * 2;

		if (IntersectRect(&dest, &orig, &Visio))
		{
			orig.x -= Visio.x;
			orig.y -= Visio.y;
			SDL_FillRect(p, &orig, (Uint32)0xFF0000);
		}
	}

	if (m_pBestSupportingSpot)
	{
		orig.x = m_pBestSupportingSpot->m_vPos.x + m_pBestSupportingSpot->m_dScore * 0.5;
		orig.y = m_pBestSupportingSpot->m_vPos.y + m_pBestSupportingSpot->m_dScore * 0.5;
		orig.w = m_pBestSupportingSpot->m_dScore;
		orig.h = m_pBestSupportingSpot->m_dScore;

		if (IntersectRect(&dest, &orig, &Visio))
		{
			orig.x -= Visio.x;
			orig.y -= Visio.y;
			SDL_FillRect(p, &orig, (Uint32)0xFFFF00);
		}
	}
}
