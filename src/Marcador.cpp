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

Marcador::Marcador(void) {
	// Posar els gols a zero
	Gols[EQUIP_LOCAL] = 0;
	Gols[EQUIP_VISITANT] = 0;

	font = TTF_OpenFont(getFullFileName("start.ttf").c_str(), 36);
	if (!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		exit(-1);
	}

	local = CreaImatgeDelNumero("0");
	visitant = CreaImatgeDelNumero("0");
}

Marcador::~Marcador(void) {
	if (font != NULL)
		TTF_CloseFont(font);
	font = NULL;

	if (local != NULL) {
		SDL_FreeSurface(local);
		local = NULL;
	}

	if (visitant != NULL) {
		SDL_FreeSurface(visitant);
		visitant = NULL;
	}
}

void Marcador::HaMarcat(int costat) {


	if (costat < 0)
		return;

	Gols[costat]++;

	std::string s = std::to_string(Gols[costat]);
	char const *numero = s.c_str();

	if (costat == 0) {
		local = CreaImatgeDelNumero(numero);
	} else {
		visitant = CreaImatgeDelNumero(numero);
	}
}

std::string Marcador::ToString() {
	std::string s = std::to_string(Gols[EQUIP_LOCAL]);
	s.append(" - ");
	s.append(std::to_string(Gols[EQUIP_VISITANT]));
	return s;
}

SDL_Surface* Marcador::CreaImatgeDelNumero(const char* gols) {

	SDL_Color color = { 255, 255, 255 };
	SDL_Surface *text_surface;

	if (!(text_surface = TTF_RenderText_Solid(font, gols, color))) {
		printf("%s -> %s\n", gols, TTF_GetError());
		exit(-1);
	}
	return text_surface;
}

void Marcador::Pinta(SDL_Surface *fons) {
	SDL_Rect dest;

	// Pintar els resultats;
	SDL_Color color = { 255, 255, 255 };

	dest.x = 10;
	dest.y = 0;
	dest.w = local->h;
	dest.h = local->w;
	SDL_BlitSurface(local, NULL, fons, &dest);

	dest.x = fons->w - visitant->w - 10;
	dest.y = 0;
	dest.w = visitant->h;
	dest.h = visitant->w;
	SDL_BlitSurface(visitant, NULL, fons, &dest);

}

int Marcador::Inicia(void) {
	Gols[EQUIP_LOCAL] = 0;
	Gols[EQUIP_VISITANT] = 0;
	return 0;
}
