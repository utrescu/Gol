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

#include "Porteria.h"
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>
#include "general.h"
#include "Regio.h"
#include "Renderator.h"

Porteria::Porteria(char *nom, int Quina, double ZoomX, double ZoomY, int pPosix, int pPosiy, int pPosalt, 
				   int pPal1, int pPal2, int pPalAmple, int pGol1, int pGol2):Entitat(Entitat::getSeguentID())
{
	SDL_Surface *Imatge2=NULL,*Imatge3=NULL;

	setTipusEntitat(tipus_entitat_porteria);
	// ----------------    Carregar Imatge
	Imatge2 = IMG_Load(nom);
	SDL_SetColorKey(Imatge2, SDL_SRCCOLORKEY , SDL_MapRGB(Imatge2->format, 0, 0, 0));
	EquipContrari=(Quina+1)%2;
	
	// ----------------    Ara depenent del costat fem una cosa o una altra
	
	if (Quina==0)
	{	
		// PORTERIA <---
		// La x ens marca el punt exacte on hi ha la linia de gol. NO ESCALAR MES
		Posicio.x = pPosix - Imatge2->w*ZoomX;
		// Cap on mira la Porteria
		MirantA = Punt3(1.0,0.0,0.0);
	}
	else
	{
		// PORTERIA --->

		// La x la deixem tal com estava perque la dono JUSTA en els marges del camp.
		Posicio.x=pPosix;
		// Cap on mira la Porteria
		MirantA = Punt3(-1.0,0.0,0.0);
		
		// La imatge s'ha de girar perque es la porteria de -->
		Imatge3 = FlipSurfaceH(Imatge2);
		SDL_SetColorKey(Imatge3, SDL_SRCCOLORKEY , SDL_MapRGB(Imatge3->format, 0, 0, 0));
		SDL_FreeSurface(Imatge2);
		Imatge2=Imatge3;
		
	}
	// pPosix ens dona la posicio x de la la linia de gol
	PuntMig.x = pPosix;
	GolDalt.x = PuntMig.x;
	// ----------------    Escalem la imatge a la mida determinada
	Imatge = zoomSurface(Imatge2,ZoomX,ZoomY,1);
	SDL_SetColorKey(Imatge, SDL_SRCCOLORKEY , SDL_MapRGB(Imatge->format, 0, 0, 0));
	
	SDL_FreeSurface(Imatge2);
	
	// ----------------    Posicio de la imatge (la x ja esta perque la necessitavem abans)
	Posicio.y=pPosiy*ZoomY;
	Posicio.w=Imatge->w;
	Posicio.h=Imatge->h;
	// Altura de la porteria 
	Altura = pPosalt*ZoomX;

	// ----------------    Definim els pals
	PalDalt = new Regio(0,Posicio.x, pPal1*ZoomY, Posicio.w, pPalAmple*ZoomY);
	PalBaix = new Regio(1,Posicio.x, pPal2*ZoomY, Posicio.w, pPalAmple*ZoomY);

	// Posem el Z-Order perquè com que no es mou no canviarà mai
	setZOrder(pPal2*ZoomY);

	// ----------------    Per saber si han marcat mirant el terra
	GolDalt.y=pGol1*ZoomY;
	GolDalt.h=0;
	
	GolBaix.x=GolDalt.x;
	GolBaix.y=pGol2*ZoomY;
	GolBaix.h=0;

	// ----------------    La zona on la pilota quedara capturada quan es faci un gol
	Base.x = Posicio.x; // GolDalt.x;
	Base.y = GolDalt.y;
	Base.w = PalDalt->getAmplada();
	Base.h = GolBaix.y - GolDalt.y;

	// ----------------    Acabem de calcular el punt mig de la porteria.
	PuntMig.y = (GolDalt.y+GolBaix.y)*0.5;
	PuntMig.h = 0;
}

Porteria::~Porteria(void)
{
	SDL_FreeSurface(Imatge);
}

int Porteria::XutAPorteria(Pilota* b)
{
	int ResultatXut=FORA;

	Punt3 pilota = b->getPosicio();

	if (pilota.h > Altura)
	{
		// massa alt per ser gol
		ResultatXut = PERSOBRE;
	}
	else
	{
		if (LineIntersection2D(pilota, b->getPosicioAnterior(), GolDalt, GolBaix))
		{
			// Gol
			ResultatXut=GOL;
		}
		else
		{
			if (PalDalt->isFora(pilota) && PalBaix->isFora(pilota))
			{
				// fora
				ResultatXut=FORA;
			}
			else
			{
				// Ha tocat el pal
				ResultatXut=PAL;
			}
		}
	}
	return ResultatXut;
}

void Porteria::Mou(SDL_Rect Visio)
{
	SDL_Rect dest;

	if (IntersectRect(&dest,&Posicio,&Visio))
	{
		Renderitza->Afegir(this);
	}
}


void Porteria::Pinta(SDL_Surface *pantalla, SDL_Rect Visio)
{
	SDL_Rect dest;
	Uint32 color=0xFF0000;
	// Nomes  el pinto si esta dins de la pantalla, per tant ja no cal comprovar
//	if (IntersectRect(&dest,&Posicio,&Visio))
//	{
	SDL_Rect PosSprite=Posicio;
	PosSprite.x -= Visio.x;
	PosSprite.y -= Visio.y;
	// Pinta la porteria
	SDL_BlitSurface(Imatge, 0, pantalla, &PosSprite);
}
