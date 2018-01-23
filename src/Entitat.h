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
#ifndef ENTITAT_H
#define ENTITAT_H

struct Telegram;
/**
	@author Xavier Sala <utrescu@xaviersala.net>
					    
	Missatges que s'envien entre els objectes per fer-los fer alguna
	cosa determinada.
*/

#include <SDL.h>
#include "punt3.h"

struct Missatge;
	
class Entitat{
public:
	enum {tipus_entitat_defecte = -1, tipus_entitat_pilota=0, tipus_entitat_porteria=1, tipus_entitat_jugador=2, tipus_entitat_porter=3};
private:
	// Identificador de l'entitat
	int  ID;
	static int SeguentID;
  	// De quin tipus és la entitat
	int TipusEntitat;
	// Posar un ID a cada entitat, s'ha de cridar en el constructor de
	// cada classe
	void setID(int val);
protected:
	// Posició de la entitat. El SDL_Rect és per pintar i per guardar
	// l'alçada i l'amplada
	// Altura és per guardar l'Amplada de l'altura
	Punt3 Lloc;
	SDL_Rect Posicio;
	double Altura;	
	// Z-Order el faig servir per ordenar abans de pintar per pantalla
	int ZOrder;
	// Radi de captació de la pilota 
	double RadiCaptacio;
	friend bool IntersectRect(SDL_Rect *dest,const SDL_Rect *src1,const SDL_Rect *src2);
	// friend SDL_Surface* FlipSurfaceH(SDL_Surface *bitmap);
public:
	Entitat(int eID);
	~Entitat();
    
	int getID() { return ID; }

	// Obtenir i assignar el z-order
	int getZOrder() { return ZOrder; }
	void setZOrder(int z) { ZOrder=z; }
	
	void setDimensions(int ample, int fons, int alt);
	
    
	Punt3 getPosicio()const { return Lloc;}
	void setPosicio(Punt3 temp);
	Punt3 getPosicioMig() { return Lloc; };
	
	SDL_Rect getPosicioImatge();
    
	SDL_Rect getRectAltura();
    
	double getRadi()const{return RadiCaptacio;}
	void   setRadi(double r){RadiCaptacio = r;}

	double getAltura()const { return Altura; }
	void setAltura(double alt) { Altura=alt; }

	int getTipusEntitat()const { return TipusEntitat; }
	void setTipusEntitat(int a) { TipusEntitat=a; }
    
	virtual bool Mou(void){return false;}; 
	virtual void Pinta(SDL_Surface *Fons, SDL_Rect ZonaVisible)=0;
	virtual void Pinta(SDL_Surface *Fons)=0;
	virtual bool RebreMissatge(const Missatge& msg){return false;};
	
	static int   getSeguentID(){return SeguentID;}
};

#endif
