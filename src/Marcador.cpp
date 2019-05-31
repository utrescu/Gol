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
#include "Marcador.h"
#include "filepath.h"

Marcador::Marcador(void)
{
	// Posar els gols a zero
	Gols[EQUIP_LOCAL] = 0;
	Gols[EQUIP_VISITANT] = 0;

	font = TTF_OpenFont(getFullFileName("start.ttf").c_str(), 36);
	if (!font)
	{
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		exit(-1);
	}
}

Marcador::~Marcador(void)
{
	if (font != NULL)
		TTF_CloseFont(font);
	font = NULL;
}

void Marcador::HaMarcat(int costat)
{
	if (costat >= 0)
		Gols[costat]++;
}

std::string Marcador::ToString() {
	std::string s = std::to_string(Gols[EQUIP_LOCAL]);
	s.append(" - ");
	s.append(std::to_string(Gols[EQUIP_VISITANT]));
	return s;
}

void Marcador::Pinta(SDL_Surface *fons)
{
	std::string sGols;
	SDL_Rect dest;
	SDL_Surface *lletres;

	// Pintar els resultats;
	SDL_Color color = {255, 255, 255};
	// sprintf(sGols, "%d", Gols[EQUIP_LOCAL]);
	sGols = std::to_string(Gols[EQUIP_LOCAL]);
	lletres = TTF_RenderText_Solid(font, sGols.c_str(), color);
	dest.x = 10;
	dest.y = 0;
	dest.w = lletres->h;
	dest.h = lletres->w;
	SDL_BlitSurface(lletres, NULL, fons, &dest);
	SDL_FreeSurface(lletres);
	// Les lletres del visitant
	sGols = std::to_string(Gols[EQUIP_VISITANT]);
	lletres = TTF_RenderText_Solid(font, sGols.c_str(), color);
	dest.x = fons->w - lletres->w - 10;
	dest.y = 0;
	dest.w = lletres->h;
	dest.h = lletres->w;
	SDL_BlitSurface(lletres, NULL, fons, &dest);
	SDL_FreeSurface(lletres);
}
int Marcador::Inicia(void)
{
	Gols[EQUIP_LOCAL] = 0;
	Gols[EQUIP_VISITANT] = 0;
	return 0;
}
