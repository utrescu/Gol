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
#include "Regio.h"
#include <stdlib.h>

Regio::Regio()
{
	x = 0;
	y = 0;
	w = 0;
	h = 0;
	PuntMig = CalculaPuntMig();
}

Regio::Regio(int id, Regio rec): ID(id)
{
	x = rec.x;
	y = rec.y;
	h = rec.h;
	w = rec.w;
	PuntMig = CalculaPuntMig();
}

Regio::Regio(int id, SDL_Rect rec): ID(id)
{
	x = rec.x;
	y = rec.y;
	h = rec.h;
	w = rec.w;
	PuntMig = CalculaPuntMig();
}

Regio::Regio(int id, int ax, int ay, int aw, int ah): ID(id)
{
	x = ax;
	y = ay;
	w = aw;
	h = ah;
	PuntMig = CalculaPuntMig();
}

Regio::~Regio()
{
}

SDL_Rect Regio::getRectangle() {
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	return r;
}

Punt3 Regio::CalculaPuntMig()
{
	PuntMig.x = x + w*0.5;
	PuntMig.y = y + h*0.5;
	PuntMig.h = 0;
	return PuntMig;
}

inline Punt3 Regio::getPosicioAleatoria() const
{
	return Punt3(x + rand()%w, y + rand()%h, 0.0);
}
