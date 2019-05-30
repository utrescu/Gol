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

#include <SDL/SDL.h>
#include "Entitatmobil.h"

class Regio;

class Pilota : public EntitatMobil
{
private:
	SDL_Surface *Imatge;
	int QuinaPilota;

	SDL_Surface *Ombra;
	// Ens diu cap on va la pilota
	// Punt3 Direccio;
	// int Radi;
	// int Forca;
	double ForcaPassada;

	double temps;
	float velocitatinicial;

	bool isCapturada;
	bool isGol;
	// Posició de la porteria on es quedarà la pilota en cas de gol
	Regio *ZonaCaptura;
	void MoureLa();

public:
	Pilota(Punt3 pos, double rad, double massa, double Escala);
	~Pilota(void);

	Punt3 getPosicioAnterior() { return LlocAnterior; };
	Punt3 getPosicioFutura();
	Punt3 getPosicioFutura(double time) const;
	// SDL_Rect getAltura();
	SDL_Rect getMida();

	SDL_Rect getPosicioImatge();

	bool Mou();
	void Pinta(SDL_Surface *Fons, SDL_Rect Visio);
	void Pinta(SDL_Surface *Fons){};

	void CapturaPilota(SDL_Rect re, bool gol);
	void AlliberaPilota();
	void Xuta2(double x, double y, double h, double f);
	void Xuta(Punt3, double);
	void Agafada() { Velocitat.Zero(); }
	void InverteixXut(int quant);

	double TempsPerRecorrerDistancia(Punt3 A, Punt3 B, double force) const;
	double ForcaPerMoure(Punt3 desti, double max);
};

Punt3 AfegirFressaAlXut(Punt3 BallPos, Punt3 BallTarget, double Punteria);
