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
#ifndef JUGADOR_H
#define JUGADOR_H

#include "Jugadorbase.h"
#include "maquinaestats.h"
#include "Regulator.h"
#include "Animacio.h"
#include <string>
#include "filepath.h"

enum Jugador_Moviments
{
	JUGADORCORRE = 0,
	JUGADORAJUDA = 1,
	JUGADORXUTA = 2,
	JUGADORSALTA = 3,
	JUGADORCAU = 4
};

/**
	@author Xavier Sala <utrescu@xaviersala.net>
*/
class Jugador : public JugadorBase
{
private:
		// --- EntitatRepresentacio ---
	// SDL_Surface *Imatge;
	SDL_Surface *Peu;
	SDL_Surface *Peu2;
	SDL_Surface *Numero;
	//	int QuinaImatgeToca;
	//	int QuinMovimentToca;
	//	int RetardMoviment;
	//	int EsperaMoviment;

	// ---- Grafics ---------------
	Animacio *Moviments;
	Jugador_Moviments MovimentActual;
	// ----------------------------

	int AlturaJugador;
	int AmpladaJugador;
	SDL_Rect RectJugador;

	// ----------------------------
	MaquinaEstats<Jugador> *maquinaEstats;

	Regulator *LimitadorXuts;

public:
	Jugador(const char *fitxerimatge, const char *nom, Equip *nos, Regio casa, Estat<Jugador> *estatinicial,
			Punt3 Mirant, double radi, Punt3 Velocitat, double massa, double maxforce, double maxspeedsense,
			double maxspeedamb, double maxgir, double xut, double punteria, double provaxut, double passades,
			double zconf, double dista, double espera, double dribling, jugo_de paper, double Escala, int anima);

	~Jugador();

	bool Mou(void);
	void Pinta(SDL_Surface *Fons);
	void Pinta(SDL_Surface *Fons, SDL_Rect Visio){};

	bool RebreMissatge(const Missatge &msg);
	MaquinaEstats<Jugador> *getFSM() const { return maquinaEstats; }

	bool PreparatPerTornarAXutar() const { return LimitadorXuts->isReady(); }

	void setMovimentActual(Jugador_Moviments ma) { MovimentActual = ma; }
	Jugador_Moviments getMovimentActual() { return MovimentActual; }
};

#endif
