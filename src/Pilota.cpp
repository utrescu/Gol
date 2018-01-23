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
#include "Pilota.h"

#include <stdlib.h>
#include <math.h>
#include <SDL/SDL_image.h>
#include "general.h"
#include "Regio.h"
#include "C2DMatrix.h"
#include "Renderator.h"

// Es una forca que afegim al càlcul de la passada exacta
// per donar-li mes velocitat (si era just arribava molt lenta)
#define FORCAAFEGIDA 0.75
//----------------------------- AfegirFressaAlXut --------------------------------
//
//  Es fa servir per fer que el jugador falli una mica els xuts. S'ha de 
// cridar abans de xutar amb la posició de la pilota i el destí
//-----------------------------------------------------------------------------
Punt3 AfegirFressaAlXut(Punt3 BallPos, Punt3 BallTarget, double Punteria)
{

	double displacement = (PI - PI*Punteria) * RandomClamped();

	Punt3 toTarget = BallTarget - BallPos;

	VectorRodaPerOrigen(toTarget, displacement);

	return toTarget + BallPos;
}


Pilota::Pilota(Punt3 pos, double rad, double massa, double Escala): EntitatMobil(rad,Punt3(0.0,0.0,0.0),-1,
																				 Punt3(0,1,0), massa, 0,0)
{	//                       .----------.
	//                      /          /|
	//                     /	  / |  Altura 
	//               Lloc *----------.  |
	//                    |          |  .
	//                    |          | /
	//                    |          |/ Posicio.h 
	//                    .----------.
	//                      Posicio.w 
	//
	//   Posicio (x i y) només serveixen per pintar 
	//   Altura ens dóna l'altura del cub (també una referència sobre quan la 
	//          pilota s'està arrossegant per terra. (posicio.h mai pot ser més 
	//          petit que Altura perquè voldria dir que s'enfonsa 
	//   
	
	setTipusEntitat(tipus_entitat_pilota);

	Lloc = pos;
	
	Posicio.x=Lloc.x;
	Posicio.y=Lloc.y;

	temps=0.0;

	// #### Pilota aturada 
	// FramePilota = Posicio;
	// FramePilota.x=0;
	// AddFrame(ESTA_ATURAT,FramePilota, 20);
	// #### Pilota rodant 
	// for (int i=0; i<4; i++) 
	// {
	//	FramePilota.x = i*FramePilota.w;
	//	AddFrame(ESTA_GIRANT,FramePilota,40);
	// }		
	//
	// Estat = ESTA_ATURAT;
	ZonaCaptura=NULL;
	isCapturada=false;
	isGol=false;
	// --------------- GRAFICS -----------------------------------------------------

	Imatge = EscalaImatge("pilota.png", Escala); // IMG_Load("pilota.png");
	Ombra = EscalaImatge("pilota-ombra.png", Escala);
	SDL_SetColorKey(Imatge, SDL_SRCCOLORKEY , SDL_MapRGB(Imatge->format, 255, 0, 0));
	QuinaPilota=0;
	setDimensions(Imatge->w/4, Imatge->h, Imatge->h);
	// ------------------------------------------------------------------------------
// Si aprenc com passar-li el radi per la funció hauria d'eliminar aquesta línia 
	setRadi(Imatge->h*0.5);
}

Pilota::~Pilota(void)
{
	if (Imatge!=NULL) SDL_FreeSurface(Imatge);
	if (Ombra!=NULL) SDL_FreeSurface(Ombra);
}

SDL_Rect Pilota::getPosicioImatge()
{
	Posicio.x = Lloc.x;
	Posicio.y = Lloc.y;
	Posicio.w=Imatge->w/4;
	Posicio.h=Imatge->h;	
	return Posicio;
}

void Pilota::AlliberaPilota()
{
	isCapturada=false;
	isGol=false;
	if (ZonaCaptura!=NULL) 
	{
		delete ZonaCaptura;
		ZonaCaptura=NULL;
	}
	
};

void Pilota::CapturaPilota(SDL_Rect re, bool gol)
{
	isGol = gol;
	isCapturada=true;
	ZonaCaptura = new Regio(0,re);
}


Punt3 Pilota::getPosicioFutura()
{
	double temps2=temps+PAS;
	Punt3 temp=getPosicio();
	temp.x += Velocitat.x;
	temp.y += Velocitat.y;
	temp.h += velocitatinicial*temps + 0.5*GRAVETAT*(temps*temps);
	return temp;
}

SDL_Rect Pilota::getMida()
{
	SDL_Rect temp;
	temp = getPosicioImatge();
	temp.x = 0;
	temp.y = 0;
	return temp;
}

// Faig que retorni CERT si s'ha mogut la pilota o no (per facilitar la vida al camp)
bool Pilota::Mou()
{
//	SDL_Rect Temp,temp;
	bool retorna = false;
	
	// Mirar si s'ha de moure o no.
	if (Velocitat.LlargadaSq() > DOSFREGAMENT || LlocAnterior.h>0)
	{
		if (isCapturada==true)
		{
			Punt3 nouLloc = getPosicioFutura();
			if (!ZonaCaptura->isDins(nouLloc) && isGol || ZonaCaptura->isDins(nouLloc) && !isGol)
			{
				if (nouLloc.x < ZonaCaptura->getMinX() || nouLloc.x > ZonaCaptura->getMaxX())
				{
					Velocitat.x*=-1;
				}
                		else 
				{
					Velocitat.y*=-1;
				}
			}
		}
		LlocAnterior = getPosicio();
		MoureLa();
		retorna=true;
	}
	else
	{
		Lloc.h=0;
		Forca=0;
		setVelocitat(Punt3());
		// Estat=ESTA_ATURAT;
	}

	// Perquè sinó no es veu...
	Renderitza->Afegir(this);

	return retorna;
}

void Pilota::MoureLa()
{
	temps+=PAS;
	// Moure-la
	Lloc.x = Lloc.x + Velocitat.x;
	Lloc.y = Lloc.y + Velocitat.y;
	
	Lloc.h = Lloc.h+ velocitatinicial*temps + 0.5*GRAVETAT*(temps*temps);
	
	if (Lloc.h<0)
	{
		Lloc.h=0;
		temps=0;
		velocitatinicial/=2;
	}

	// Posem el Z-Order de la pilota. és rodoneta
	setZOrder((int)Lloc.y+Posicio.h);
	// Treure força a la pilota després de moure-la.
	// Forca += FREGAMENT;
	Velocitat += VectorNormalitza(Velocitat) * FREGAMENT;
}

double Pilota::TempsPerRecorrerDistancia(Punt3 A, Punt3 B, double force)const
{
	//this will be the velocity of the ball in the next time step *if*
	//the player was to make the pass. 
	double speed = force / Massa;
	//calculate the velocity at B using the equation
	//
	//  v^2 = u^2 + 2as
	//
	//first calculate s (the distance between the two positions)
	double DistanceToCover =  VectorDistancia(A, B);
	double term = speed*speed + 2.0*DistanceToCover*FREGAMENT;
  	//if  (u^2 + 2as) is negative it means the ball cannot reach point B.
	if (term <= 0.0) return -1.0;
	double v = sqrt(term);
  	//it IS possible for the ball to reach B and we know its speed when it
  	//gets there, so now it's easy to calculate the time using the equation
	//
  	//    t = v-u
  	//        ---
  	//         a
	//
	return (v-speed)/FREGAMENT;
}

double Pilota::ForcaPerMoure(Punt3 desti, double max)
{
	double forca; 
	Punt3 Objectiu = desti - getPosicio();
	double dist = Objectiu.Llargada();

	if (dist>0)
	{
		forca = sqrt(-2 * FREGAMENT * dist * Massa * Massa);
	}
	// return (dist*Massa)/DOSFREGAMENT;
	if (forca>max) return max;
	// 2006-10-23: Afegeixo +CONSTANT per veure si va més fort
			else return forca + FORCAAFEGIDA; 
}

void Pilota::Xuta2(double x, double y, double h, double f)
{
	// Direcció del xut
	Velocitat.x = x;
	Velocitat.y = y;
	// Potser hauria de posar-hi l'alçada mínima
	Velocitat.h = h;
	
	temps = 0;
	// velocitat inicial perquè la pilota pugi ... 
	velocitatinicial = sqrt((float)2*abs(GRAVETAT)*h);
	Forca = f;
	
	// Si redueixo automàticament la força no em farà falta fer això ..
	ForcaPassada = ((abs((int)x)+abs((int)y))/2)+(h/2); //+ abs(Direccio.h);
	
	// Estat = ESTA_GIRANT;
}

void Pilota::Xuta(Punt3 direccio, double forca)
{
	Forca = forca;

	// if (forca>6) direccio.h=forca / (rand()%2+1);

	direccio.Normalitza();
	velocitatinicial=sqrt((float)2*abs(GRAVETAT)*direccio.h);;
	Punt3 acceleracio = (direccio * Forca) / Massa;
	Velocitat = acceleracio;
	
	// ---------- Segons les lleis de la física Newtoniana ---------- 
	// Primer he de mirar quin angle forma el vector amb el terra
	// per reduir la força del xut en funció de l'altura ...
	// Forca = forca;
	// direccio.Normalitza();
	// double angle = VectorDot(direccio, Punt3(direccio.x,direccio.y,0));
	// Punt3 acceleracio = (direccio*Forca)/Massa;
	// velocitatinicial = acceleracio.h * sin(angle);
	// Velocitat = acceleracio; 
	
	temps=0;
}

// El paràmetre ens indica la quantitat de reducció de força que tindrem 
void Pilota::InverteixXut(int quan)
{
	Velocitat.x *= -1;
	Velocitat.y *= -1;
	Velocitat.h *=-1;
	Velocitat = Velocitat / quan; 
}

Punt3 Pilota::getPosicioFutura(double time)const
{
  	//using the equation s = ut + 1/2at^2, where s = distance, a = friction
  	//u=start velocity 
  	//calculate the ut term, which is a vector
	Punt3 ut = Velocitat * time;
  	//calculate the 1/2at^2 term, which is scalar
	double half_a_t_squared = 0.5 * FREGAMENT * time * time;
  	//turn the scalar quantity into a vector by multiplying the value with
  	//the normalized velocity vector (because that gives the direction)
	Punt3 ScalarToVector = half_a_t_squared * VectorNormalitza(Velocitat);
  	//the predicted position is the balls position plus these two terms
	return Lloc + ut + ScalarToVector;
}

void Pilota::Pinta(SDL_Surface *Fons, SDL_Rect ZonaVisible)
{
	SDL_Rect PilotaAPintar;
	
	// if (NextFrame())
	// {
	//    Frame* f = GetFrame();
	SDL_Rect Pos = getPosicioImatge();
	Pos.x -= ZonaVisible.x;
	Pos.y -= ZonaVisible.y;
	// Pinta la ombra de la pilota
	SDL_BlitSurface(Ombra, 0, Fons, &Pos);
	
	// S'ha de pintar segons l'altura, per tant modifico el destí
	// perquè sigui una mica més a dalt
    	Pos.y -= Lloc.h;
	//    PilotaAPintar = f->GetOrigen();
	PilotaAPintar=getPosicioImatge();
	// S'ha de canviar la pilota quan canvii un PAS 
	if (Forca > 0) QuinaPilota=(QuinaPilota+1)%4;
	PilotaAPintar.x = QuinaPilota*PilotaAPintar.w;
	PilotaAPintar.y=0;

	SDL_BlitSurface(Imatge, &PilotaAPintar, Fons, &Pos);
}
