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
#ifndef PORTER_H
#define PORTER_H

#include "Jugadorbase.h"
#include "maquinaestats.h"
#include "Animacio.h"
#include <string>

enum Porter_Moviments { PORTERCORRE=0, PORTERESTIRA=1, PORTERSALTA=2 };

/**
	@author Xavier Sala <utrescu@xaviersala.net>
*/
class Porter : public JugadorBase
{
	std::string NomJugador;
	// --- EntitatRepresentacio ---
	SDL_Surface *Peu;
	// ---- Grafics ---------------
	Animacio *Moviments;
	Porter_Moviments MovimentActual;
	// ----------------------------

	int AlturaJugador;
	int AmpladaJugador;
	SDL_Rect RectJugador;

	// ----------------------------
	MaquinaEstats<Porter>* maquinaEstats;
	// Sembla que es fa servir perque sempre miri endavant
	Punt3  Mirant;
	// Determina si estem en ple estirament o no
	bool EsticEstirat;
public:
    Porter(const char *fitxerimatge, const char *nom, Equip* nos, Regio casa, Estat<Porter>* estatinicial,
		   Punt3 mirant, double radi,Punt3 velocitat, double massa, double maxforce, 
		   double maxvelocamb, double maxvelocsense, double maxgir, double xut, double punteria,
		   double provaxut, double passades, double zconf, double dista, double espera, double dribling, 
		   double surt, double segueix, double reaccio, double agafa, jugo_de paper,double Escala, 
		   int anim);

    ~Porter();

    bool isForaDelCamp() const;
   // retorna 'cert' si la pilota es prou a prop del porter
    bool isAbastDeIntercepcio()const;
   //returns true if the keeper has ventured too far away from the goalmouth
    bool  isMassaLlunyDeLaPorteria()const;
   //this method is called by the Intercept state to determine the spot
   //along the goalmouth which will act as one of the interpose targets
   //(the other is the ball).
   //the specific point at the goal line that the keeper is trying to cover
   //is flexible and can move depending on where the ball is on the field.
   //To achieve this we just scale the ball's y value by the ratio of the
   //goal width to playingfield width
    Punt3 GetRearInterposeTarget()const;

    MaquinaEstats<Porter>* getFSM()const{return maquinaEstats;}
    Punt3 getMirant()const{return Mirant;}
    void  setMirant(Punt3 v){Mirant=v;}
    
    bool Mou();
    void Pinta(SDL_Surface *Fons);
    void Pinta(SDL_Surface *Fons, SDL_Rect Visio){}
    bool RebreMissatge(const Missatge& msg);
	// Diu si el porter s'ha estirat o no (si est� estirat no pot canviar de direccio)
	bool isEstirat(void) { return EsticEstirat; }
	void setEstirat(bool b) { EsticEstirat = b; }
};

#endif
