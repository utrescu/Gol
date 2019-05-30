#pragma once
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

/**
	@author Xavier Sala <utrescu@xaviersala.net>
*/

#include <SDL/SDL.h>

class Frames
{
	// Sempre tindré una còpia del original per no perdre qualitat
	SDL_Surface *Origen;
	// Aquest és el que mostrarem...
	SDL_Surface *Real;
	// Temps d'espera entre aquesta imatge i l'altra
	Uint32 tempsEspera;
	// A quina escala està la imatge
	double EscalaActual;

public:
	Frames(SDL_Surface *imatge, Uint32 temps);
	Frames(SDL_Surface *orig, SDL_Rect rec, Uint32 temps);
	~Frames(void);
	SDL_Surface *getImatge() { return Real; }
	void setEscala(double b);
	Uint32 getEspera() { return tempsEspera; }
	void setEspera(Uint32 d) { tempsEspera = d; }

	int getAltura() { return Real->h; }
	int getAmplada() { return Real->w; }
};
