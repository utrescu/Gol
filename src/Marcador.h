#pragma once
/***************************************************************************
 *   Copyright (C) 2006 by Xavier Sala   *
 *   utrescu@xaviersala.net   *
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
#include <SDL/SDL_ttf.h>

enum QuinEquip
{
    EQUIP_LOCAL = 0,
    EQUIP_VISITANT = 1
};

class Marcador
{
private:
    int Gols[2];
    TTF_Font *font;

public:
    Marcador(void);
    ~Marcador(void);
    void HaMarcat(int costat);
    void Pinta(SDL_Surface *fons);
    int Inicia(void);
};
