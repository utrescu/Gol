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

#include <SDL.h>
#include <SDL_gfxPrimitives.h>

// #include "debug/DebugConsole.h"

//------------------------------- dtor ----------------------------------------
//-----------------------------------------------------------------------------
CalculadoraPuntsSuport::~CalculadoraPuntsSuport()
{
	delete m_pRegulator;
}


//------------------------------- ctor ----------------------------------------
//-----------------------------------------------------------------------------
CalculadoraPuntsSuport::CalculadoraPuntsSuport(int numX, int numY, Equip* team):m_pBestSupportingSpot(NULL),
                                                                  m_pTeam(team)
{
	const Regio* PlayingField = team->getCamp()->getTerrenyDeJoc();

	//calculate the positions of each sweet spot, create them and 
  	//store them in m_Spots
	double HeightOfSSRegion = PlayingField->getAltura()  * 0.8;
	double WidthOfSSRegion  = PlayingField->getAmplada() * 0.9;
	double SliceX = WidthOfSSRegion / numX ;
	double SliceY = HeightOfSSRegion / numY;

	double left  = PlayingField->getMinX() + (PlayingField->getAmplada()-WidthOfSSRegion)/2.0 + SliceX/2.0;
	double right = PlayingField->getMaxX() - (PlayingField->getAmplada()-WidthOfSSRegion)/2.0 - SliceX/2.0;
	double top   = PlayingField->getMinY() + (PlayingField->getAltura()-HeightOfSSRegion)/2.0 + SliceY/2.0;

	for (int x=0; x<(numX/2)-1; ++x)
	{
		for (int y=0; y<numY; ++y)
		{      
			if (m_pTeam->getQuinEquip() == Equip::visitant)
			{
				m_Spots.push_back(PuntSuport(Punt3(left+x*SliceX, top+y*SliceY,0.0), 0.0));
			}

			else
			{
				m_Spots.push_back(PuntSuport(Punt3(right-x*SliceX, top+y*SliceY,0.0), 0.0));
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
 
	double BestX 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ W T I M E . I N L  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ X S T D D E F  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ C S T D D E F  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ S T D D E F . H  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ I N I T I A L I Z E R _ L I S T  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ N E W  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ E X C E P T I O N  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ E H . H  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ M A L L O C . H  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ S T R I N G . H  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ X U T I L I T Y  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ U T I L I T Y  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ I O S F W D  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ C S T D I O  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ S T D I O . H  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ C S T R I N G  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ C R T D B G . H  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ T Y P E _ T R A I T S  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ X R E F W R A P  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ X A T O M I C 0 . H  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ I N T R I N . H  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ S E T J M P . H  
 C : \ P R O G R A M   F I L E S   ( X 8 6 ) \ M I C R O S O F T   V I S U A L   S T U D I O   1 2 . 0 \ V C \ I N C L U D E \ I M M I N T R I N . H  
 C : \ P R O G R A M 