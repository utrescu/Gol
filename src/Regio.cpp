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
	regio.x= 0;
	regio.y=0;
	regio.w=0;
	regio.h=0;
	PuntMig = CalculaPuntMig();
}

Regio::Regio(int id, SDL_Rect rec): ID(id)
{
	regio = rec;
	PuntMig = CalculaPuntMig();
}

Regio::Regio(int id, int ax, int ay, int aw, int ah): ID(id)
{
	regio.x = ax;
	regio.y = ay;
	regio.w = aw;
	regio.h = ah;
	PuntMig = CalculaPuntMig();
}

Regio::~Regio()
{
}

Punt3 Regio::CalculaPuntMig()
{
	PuntMig.x = regio.x + regio.w*0.5;
	PuntMig.y = regio.y + regio.h*0.5;
	PuntMig.h = 0;
	return PuntMig;
}

inline Punt3 Regio::getPosicioAleatoria() const
{
	return Punt3(regio.x + rand()%regio.w, regio.y + rand()%regio.h, 0.0);
}
