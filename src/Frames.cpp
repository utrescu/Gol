#include "Frames.h"
#include "general.h"
/***************************************************************************
 *   Copyright (C) 2006 by Xavier Sala   *
 *   utrescu@xaviersala.net    *
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
Frames::Frames(SDL_Surface* imatge, Uint32 temps)
{
	// **** Revisar-ho ****
	// Potser: SDL_BlitSurface(imatge, NULL, Origen, NULL);

	// Potser he de crear la superficie abans de fer aixo...
	// Origen=EscalaSuperficie(imatge, 1.0);
	Origen=SDL_DisplayFormatAlpha(SDL_GetVideoSurface());
	Origen = SDL_CreateRGBSurface(SDL_HWSURFACE,imatge->w, imatge->h, SDL_GetVideoSurface()->format->BitsPerPixel, 
		              SDL_GetVideoSurface()->format->Rmask, SDL_GetVideoSurface()->format->Gmask,
					  SDL_GetVideoSurface()->format->Bmask,SDL_GetVideoSurface()->format->Amask);
	SDL_BlitSurface(imatge, NULL, Origen, NULL);
	// ---------
	Real = EscalaSuperficie(imatge, 1.0);
	SDL_SetColorKey(Real, SDL_SRCCOLORKEY , SDL_MapRGB(Real->format, 255, 255, 255));
	EscalaActual=1.0;
	tempsEspera=temps;
}

Frames::Frames(SDL_Surface* orig, SDL_Rect rec, Uint32 temps)
{
	// Rebem la superficie, i el rectangle a extreure, per tant copiar i prou.
//	Origen=SDL_DisplayFormatAlpha(orig);
	// Real=SDL_DisplayFormatAlpha(SDL_GetVideoSurface());

   Uint32 rmask, gmask, bmask, amask;

	// Perque sino no es copia la transferencia
   	SDL_SetAlpha(orig, 0, 0);

    // SDL interprets each pixel as a 32-bit number, so our masks must depend on the endianness (byte order) 
	// of the machine 
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	Origen = SDL_CreateRGBSurface(SDL_SWSURFACE, rec.w, rec.h, 32, rmask, gmask, bmask, amask);
	if (!Origen)
	{
		printf("ERROR en la creacio de la superficie\n");
		return;
	}

	if (SDL_BlitSurface(orig, &rec, Origen, NULL)!=0)
	{
		printf("ERROR al copiar la imatge des de l'origen\n");
	}
	// Real = EscalaSuperficie(Origen,1.0);
	Real = zoomSurface(Origen,1.0,1.0,0);

	EscalaActual=1.0;
	tempsEspera=temps;
}

Frames::~Frames(void)
{
	SDL_FreeSurface(Real);
	SDL_FreeSurface(Origen);
}

void Frames::setEscala(double b)
{
	if (b!=EscalaActual)
	{
		// Escalar la imatge Origen a Real
		Real = EscalaSuperficie(Origen, b);
		EscalaActual = b;
	}
}



