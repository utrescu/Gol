#pragma once
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
 
#include "Entitat.h"
#include "Pilota.h"

/*                    
                    /  ||                       / ||
                   /   ||                      /  ||
      PalAlt --->.----------------------------.   ||<-- PalBaix.h 
                 |w.------------------------. |   ||
				 | |   ||			        | |   ||
				 | |   ..-------------------| |---..
				 | |  //                    | | <----- Base
				 | | //                     | | //
	             | |//                      | |//
			 x-->|_|/__________.____________|_|/
                   ^           ^            ^ 
                   |           |            | 
				 GolDalt       PuntMig    GolBaix     

				(la l�nea de gol est� sempre a GolDalt.x i GolBaix.x o a PuntMig.x)
				( i c�rre entre GolDalt.y + GolBaix.y)
				( PalAlt i PalBaix representen una caixa amb els pals i la xarxa dels costats de 
				  la porteria les faig servir per evitar que entri la pilota des del lateral sense
				  passar per la l�nia de gol )
				( PalDalt.h i PalBaix.h d�na l'altura del pal i el lateral )
				( PalDalt.w i PalBaix.w d�na el gruix del pal i el lateral )

     Com pot ser que una cosa tan senzilla m'hagi portat tants problemes!!

*/






class Porteria: public Entitat
{
public:
	enum ResultatDelXut { FORA=0, PAL=1, GOL=2, PERSOBRE=3 };
private:
	SDL_Surface* Imatge;
	SDL_Surface* Peu;
	// Defineix el poste i el lateral
	Regio* PalDalt;
	Regio* PalBaix;
	// Defineix la l�nea de gol
	Punt3 GolDalt;
	Punt3 GolBaix;

    // Ens d�na el punt mig de la zona de gol
	Punt3 PuntMig;
	// Guarda cap on mira la porteria
	Punt3 MirantA;
	// D�na la zona de gol (per mantenir la pilota quan sigui dins)
	SDL_Rect Base;
	// Qui �s l'equip contrari
	int EquipContrari;

public:
	// Porteria(int Quina, double ZoomX, double ZoomY);
	Porteria(char *nom, int Quina, double ZoomX, double ZoomY, int pPosix, int pPosiy, int pPosalt, 
			 int pPal1, int pPal2, int pPalAmple, int pGol1, int pGol2);
	~Porteria(void);
	// Determina qu� ha passat amb el xut a porteria
	int XutAPorteria(Pilota* b);
	// Encara que no es mou, serveix per saber si l'hem de pintar o no
	void Mou(SDL_Rect Visio);

	void Pinta(SDL_Surface *pantalla, SDL_Rect Visio);
	void Pinta(SDL_Surface *pantalla) {}
	
	// Retorna el punt mig de la porteria sobre la l�nia de gol
	// Si fes PuntMig.h = Altura * 0.5 donaria el mig real
	Punt3 getMig() { return PuntMig; }
	Punt3 getMirant() { return MirantA; }

	// Obtenir el requadre interior de la porteria (per quan marquen un gol)
	SDL_Rect getTerra() { return Base; }
	// --------- OBTENIR LA LINEA DE GOL ----- 
    // Ens d�na on comen�a la l�nea de gol (Ser� gol entre esquerra i dreta)
	int getForatEsquerra() { return GolDalt.y; }
	// Ens d�na on acaba la l�nea de gol (Ser� gol entre esquerra i dreta)
	int getForatDreta() { return GolBaix.y; }
	// Per� potser �s molt millor saber on �s dins
	int getLiniaDeGol() {
		int valor = GolDalt.x;
	}
	// Ens d�na l'amplada de la l�nia de gol
	int getAmpladaPorteria() { return GolBaix.y-GolDalt.y; }
	// Ens d�na el terra de la l�nia de gol (sempre ser� zero, per� ... )
	int getForatBaix() { return 0; }
	// Ens d�na el punt superior de la zona de gol en la porteria
	int getForatDalt() { return Altura; }
};
