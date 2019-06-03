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

    Punt3 PuntMig;

    int ID;

    Punt3 CalculaPuntMig();

public:
    int x;
        int y;
        int w;
        int h;

    Regio();
    Regio(int id, SDL_Rect reg);
    Regio(int id, Regio reg);
    Regio(int id, int ax, int ay, int aw, int ah);
    ~Regio();

    int getID() { return ID; }
    SDL_Rect getRectangle();
    int getAltura() const { return h; }
    int getAmplada() const { return w; }

    int getMinX() const { return x; }
    int getMaxX() const { return x + w; }
    int getMinY() const { return y; }
    int getMaxY() const { return y + h; }

    Punt3 getPuntMig() { return PuntMig; }

    inline bool isDins(Punt3 pos, region_modifier r) const
    {
        if (r == normal)
        {
            return ((pos.x > x) && (pos.x < x + w) &&
                    (pos.y > y) && (pos.y < y + h));
        }
        else
        {
            const double marginX = w * 0.25;
            const double marginY = h * 0.25;

            return ((pos.x > (x + marginX)) && (pos.x < (x + w - marginX)) &&
                    (pos.y > (y + marginY)) && (pos.y < (y + h - marginY)));
        }
    }

    inline bool isDins(Punt3 pos) const
    {
    	// Arrodonir els valors de la posició
    	int posx = (int)(x);
    	int posy = (int)(y);
    	int posh = (int)(h);
    	int posw = (int)(w);


        return ((pos.x > x) && (pos.x < x + w) &&
                (pos.y > y) && (pos.y < y + h));
    }
    inline bool isDins(SDL_Rect pos) const
    {
        return ((pos.x > x) && (pos.x + pos.w < x + w) &&
                (pos.y > y) && (pos.y + pos.h < y + h));
    }

    inline bool isFora(SDL_Rect pos) const
    {
        return ((pos.x + pos.w < x) || (pos.x > x + w) ||
                (pos.y + pos.h < y) || (pos.y > y + h));
    }

    inline bool isFora(Punt3 pos) const
    {
    	// Arrodonir els valors de la posició
		int posx = (int)(x);
		int posy = (int)(y);
		int posh = (int)(h);
		int posw = (int)(w);

        return ((pos.x < x) || (pos.x > x + w) ||
                (pos.y < y) || (pos.y > y + h));
    }

    inline Punt3 getPosicioAleatoria() const;
};

#endif
