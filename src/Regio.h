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
#ifndef REGIO_H
#define REGIO_H

/**
Defineix una regió del camp. Dividira el camp en regions rectangulars...

La farem servir per accelerar la comprovacio de collisions, nomes caldra comprovar els que son a la mateixa regio
	@author Xavier Sala <xavier@norimaki.xaviersala.net>
*/
#include <SDL/SDL.h>
#include "punt3.h"

class Regio
{
public:
    enum region_modifier
    {
        halfsize,
        normal
    };

private:
    SDL_Rect regio;
    Punt3 PuntMig;

    int ID;

    Punt3 CalculaPuntMig();

public:
    Regio();
    Regio(int id, SDL_Rect reg);
    Regio(int id, int ax, int ay, int aw, int ah);
    ~Regio();

    int getID() { return ID; }
    SDL_Rect getRectangle() { return regio; }
    int getAltura() const { return regio.h; }
    int getAmplada() const { return regio.w; }

    int getMinX() const { return regio.x; }
    int getMaxX() const { return regio.x + regio.w; }
    int getMinY() const { return regio.y; }
    int getMaxY() const { return regio.y + regio.h; }

    Punt3 getPuntMig() { return PuntMig; }

    inline bool isDins(Punt3 pos, region_modifier r) const
    {
        if (r == normal)
        {
            return ((pos.x > regio.x) && (pos.x < regio.x + regio.w) &&
                    (pos.y > regio.y) && (pos.y < regio.y + regio.h));
        }
        else
        {
            const double marginX = regio.w * 0.25;
            const double marginY = regio.h * 0.25;

            return ((pos.x > (regio.x + marginX)) && (pos.x < (regio.x + regio.w - marginX)) &&
                    (pos.y > (regio.y + marginY)) && (pos.y < (regio.y + regio.h - marginY)));
        }
    }

    inline bool isDins(Punt3 pos) const
    {
        return ((pos.x > regio.x) && (pos.x < regio.x + regio.w) &&
                (pos.y > regio.y) && (pos.y < regio.y + regio.h));
    }
    inline bool isDins(SDL_Rect pos) const
    {
        return ((pos.x > regio.x) && (pos.x + pos.w < regio.x + regio.w) &&
                (pos.y > regio.y) && (pos.y + pos.h < regio.y + regio.h));
    }

    inline bool isFora(SDL_Rect pos) const
    {
        return ((pos.x + pos.w < regio.x) || (pos.x > regio.x + regio.w) ||
                (pos.y + pos.h < regio.y) || (pos.y > regio.y + regio.h));
    }

    inline bool isFora(Punt3 pos) const
    {
        return ((pos.x < regio.x) || (pos.x > regio.x + regio.w) ||
                (pos.y < regio.y) || (pos.y > regio.y + regio.h));
    }

    inline Punt3 getPosicioAleatoria() const;
};

#endif