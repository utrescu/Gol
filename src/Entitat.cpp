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
#include "Entitat.h"
#include <assert.h>

int Entitat::SeguentID = 0;

Entitat::Entitat(int eID):RadiCaptacio(0.0),
	TipusEntitat(tipus_entitat_defecte)
{
	assert ( (eID >= SeguentID) && "<BaseGameEntity::SetID>: invalid ID");
	
	setID(eID);
	SeguentID = eID + 1;
		
	Lloc.x=0.0;
	Lloc.y=0.0;
	Lloc.h=0.0;
	
	Posicio.x=0;
	Posicio.y=0;
	Posicio.w=0;
	Posicio.h=0;
	Altura=0;
}

void Entitat::setID(int val)
{
	ID=val;
}

Entitat::~Entitat()
{
}

/*!
    \fn Entitat::Inicialitza(Punt3 pos, Punt3am)
	Serveix per inicialitzar l'objecte a uns valors determinats
	pos: contindrà la posició de l'objecte
	am: tindrà l'amplada de cada dimensió */
void Entitat::setDimensions(int ample, int fons, int alt)
{
	Posicio.w = ample;
	Posicio.h = fons;
	Altura = alt;
	// També poso la posició per defecte de 
	// l'altura perquè és on ha de ser.
	Lloc.h = 0;

}

/*!
    \fn Entitat::getAltura()
	Ens retornarà el rectangle segons l'altre costat 2D (h i y)
 */
SDL_Rect Entitat::getRectAltura()
{
	SDL_Rect temp;
	temp.x = Lloc.h;
	temp.w = Altura;
	temp.y = Lloc.y-Posicio.h*0.5;
	temp.h = Posicio.h;
	return temp;
}

/*!
    \fn Entitat::setPosicio(Punt3 temp)
	Per canviar la posició de l'objecte en les 3 direccions
 */
void Entitat::setPosicio(Punt3 temp)
{
	Lloc = temp;
}

SDL_Rect Entitat::getPosicioImatge()
{
	Posicio.x = Lloc.x-Posicio.w*0.5;
	Posicio.y = Lloc.y-Posicio.h*0.5;
	return Posicio;
}


