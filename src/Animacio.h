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
#ifndef ANIMACIO_H
#define ANIMACIO_H

/**
	@author Xavier Sala <utrescu@xaviersala.net>
*/

#include <vector>
#include "Frames.h"

#define DIRECCIONS 8

enum Direccions {
	DRETA=0,
	DRETADALT=1,
	DALT=2,
	ESQUERRADALT=3,
	ESQUERRA=4,
	ESQUERRABAIX=5,
	BAIX=6,
	DRETABAIX=7
};

class Animacio{
    std::vector<Frames*> Direccio[DIRECCIONS];
	Direccions QuinMoviment;
	int ImatgeActual;
	int DireccioAnimacio;

	bool RotacioImatges;
	bool AnimacioInterrupcio;

    	Uint32 tempsActual;
    
public:
    Animacio();
    ~Animacio();

    // Li passarem un arxiu lua	i ha de generar tots els frames
    bool load(char *nom, int numero); 
    // Funcions per la gestió dels frames
    void AfegirImatge(int Direccio,SDL_Surface* imatge, Uint32 Espera);
	void AfegirImatge(int Direc, SDL_Surface* origen, SDL_Rect origrec, Uint32 Espera);
	void setEspera(int Direccio, int index, Uint32 Espera);

    // --------------------  Canvi d'imatges
    void Avanca(Direccions novadireccio);
	void CanviaDireccio(Direccions dir);
	// Dóna l'índex de l'animació actual
	int getImatgeActual() { return ImatgeActual; }
	// Per fer que la direcció sigui endavant o enrere
	void setDireccio(int dir) { DireccioAnimacio = dir; }
	// Per quan el jugador està aturat
	void stop() { ImatgeActual=0; }
	// Obtenir la imatge actual
	SDL_Surface *getImatge();
	// Som a final d'animació?
	bool isFinalAnimacio();
	// Determinem si la animació pot ser trencada o no
	void AnimacioInterrumpible(bool b) { AnimacioInterrupcio = b; }

	int getAltura() { return Direccio[QuinMoviment].at(ImatgeActual)->getAltura(); }
	int getAmplada() { return Direccio[QuinMoviment].at(ImatgeActual)->getAmplada(); }

	// Les imatges tenen rotació o no...
	void setRotacio(bool roda) { RotacioImatges=roda; }
	// Establir l'escalat
	void setEscala(float escala);
	
    	Frames* GetFrameActual() { return Direccio[QuinMoviment].at(ImatgeActual); }
};

#endif
