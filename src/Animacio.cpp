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
#include "Animacio.h"
#include "LUAManager.h"

#include <SDL/SDL_image.h>

Animacio::Animacio()
{
	QuinMoviment=ESQUERRA;
	ImatgeActual=0;
	tempsActual=SDL_GetTicks();
	DireccioAnimacio = 1;
	// Per defecte les animacions són rotacions
	RotacioImatges = true;
	// Per defecte les animacions es poden acabar sense problemes
	AnimacioInterrupcio = true;
}


Animacio::~Animacio()
{

}

// Carrega la animació determinada
//
bool Animacio::load(char *nom, int numero)
{
	// Obtenir les dades del LUA
	char quina[11]={ "AnimacioX" };
	SDL_Rect original;
	SDL_Rect origen;
	int numImatges;
	double espera;

	LUAManager *lu = new LUAManager(nom);

	// Primer carreguem imatge
	std::string Fitxer = lu->getCadena("Imatge");
	SDL_Surface *Imatge = IMG_Load(Fitxer.c_str());

	if (Imatge==NULL)
	{
		// La imatge no existeix!!
		SDL_Quit();
	}

	// Per obtenir els valors de la taula concreta
	sprintf(quina, "Animacio%d",numero);

	numImatges = lu->getTaulaNumeroint(quina,"num");
	
	// Ha de carregar totes les direccions de la mateixa animacio
	// primer carreguem les dades de la primera i després ja els
	// farem avançar en el bucle
	original.x = lu->getTaulaNumeroint(quina,"x");
	original.y = lu->getTaulaNumeroint(quina,"y");
	original.w = lu->getTaulaNumeroint(quina,"ample");
	original.h = lu->getTaulaNumeroint(quina,"alt");
	espera = lu->getTaulaNumeroint(quina,"espera");
	// lu->getTaulaNumeroint(quina,"trencable");
	// lu->getTaulaNumeroint(quina,"rotacio");

	// Hi ha 'numimatges' per cada direcció
	for (int it=0; it<numImatges; it++)
	{
		// Sempre tindrem animacions per totes les direccions
		for (int jt=0; jt<DIRECCIONS; jt++)
		{
			origen = original;
			origen.x = original.x + it * (original.w+3);
			origen.y = original.y + jt * (original.h+1); 
			// Moviments[it].AfegirImatge(k,Imatge,temp,RetardMoviment);
			AfegirImatge(jt,Imatge,origen,espera);
		}
	}

	SDL_FreeSurface(Imatge);
	delete lu;

	return true;
}

/*
	AfegirImatge()

	Funció que afegeix un nou marc a una direcció En els marcs hi ha
	el temps d'espera i el requadre d'imatge.
	Retorna: res
*/
void Animacio::AfegirImatge(int Direc, SDL_Surface* imatge, Uint32 Espera)
{
	Direccio[Direc].push_back(new Frames(imatge,Espera));
}
// Per intentar solucionar el problema de les transparències li passem tota la imatge i la retallarà
// el FRAME!
void Animacio::AfegirImatge(int Direc, SDL_Surface* origen, SDL_Rect origrec, Uint32 Espera)
{
	Direccio[Direc].push_back(new Frames(origen,origrec,Espera));
}


void Animacio::setEspera(int Direc, int index, Uint32 Espera)
{
	Direccio[Direc].at(index)->setEspera(Espera);
}

/*
	Avanca()

	Funció que canvia el Frame actual, si ha passat prou temps
	Retorna: veritat si hem canviat de frame, o fals si no ha canviat
*/
void Animacio::Avanca(Direccions novadireccio)
{
	Uint32 tempsNou, tempsTotal;

	tempsTotal = Direccio[QuinMoviment].at(ImatgeActual)->getEspera();
	tempsNou = SDL_GetTicks() - tempsActual;

	if (tempsTotal<tempsNou)
	{
		// Mirar si ha girat, i per tant hem de mostrar una altra imatge
		if (novadireccio!=QuinMoviment) 
		{
			CanviaDireccio(novadireccio);
		}
		else
		{
			// Només avançar si la velocitat no és zero

			ImatgeActual=(ImatgeActual+DireccioAnimacio)%Direccio[QuinMoviment].size();
			tempsActual=SDL_GetTicks();
		}
	}
}

//
//	CanviaAnimacio()
//
//	Funció que canvia la animació actual per una altra
//
void Animacio::CanviaDireccio(Direccions dir)
{
	QuinMoviment=dir;
	// ImatgeActual=0;
	tempsActual=SDL_GetTicks();
}

// getImatge()
//
// Retorna la imatge actual segons quina sigui el moviment
SDL_Surface* Animacio::getImatge()
{
	return Direccio[QuinMoviment].at(ImatgeActual)->getImatge();
}


// bool getFinalAnimacio()
//
// Ens diu si estem al final de l'animació o no
// en el cas de que es pugui trencar l'animació
// sempre dirà que SI.
// Això ens servirà per poder fer que una animació
// acabi abans de canviar.
bool Animacio::isFinalAnimacio()
{
	return (Direccio[QuinMoviment].size()==ImatgeActual-1 || AnimacioInterrupcio);
};

// setEscala(float)
//
// Es tracta d'escalar totes les imatges en el valor especificat
void Animacio::setEscala(float escala)
{
	for (int j=0; j<DIRECCIONS; j++)
	{
		for(int i=0; i<Direccio[QuinMoviment].size(); i++)
		{
			Direccio[j].at(i)->setEscala(escala);
		}
	}
};
