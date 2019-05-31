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
#include "Porter.h"
#include "PorterEstats.h"
#include "general.h"
#include "Renderator.h"

Porter::Porter(const char *fitxerimatge, const char *nom, Equip *nos, SDL_Rect casa,
			   Estat<Porter> *estatinicial, Punt3 mirant, double radi, Punt3 velocitat, double massa,
			   double maxforce, double maxvelocamb, double maxvelocsense, double maxgir, double xut,
			   double punteria, double provaxut, double passades, double zconf, double dista, double espera,
			   double dribling, double surt, double segueix, double reaccio, double agafa, jugo_de paper,
			   double Escala, int anima) : JugadorBase(nos, casa, mirant, radi, velocitat, massa,
													   maxforce, maxvelocamb, maxvelocsense, maxgir,
													   xut, punteria, provaxut, passades, zconf,
													   dista, espera, dribling, paper)
{
	setTipusEntitat(tipus_entitat_porter);
	NomJugador = nom;

	POR_SurtAPerPilota = surt * surt;	  // PILOTAABASTINTERCEPTARPORTER;
	POR_Seguiment = segueix;			   // PORTERDISTANCIASEGUIMENT;
	POR_PilotaAbastPorter = agafa * agafa; // PILOTAALABASTDELPORTER
	POR_Reaccio = reaccio;				   // PORTERREACCIO

	// --------------- GRAFICS -----------------------------------------------------
	Peu = EscalaImatge("jugador-ombra.png", Escala);

	Moviments = new Animacio[anima];
	// Carregar els diferents moviments
	for (int j = 0; j < anima; j++)
	{
		Moviments[j].load((char *)fitxerimatge, j);
		Moviments[j].setEscala(Escala);
	}
	MovimentActual = PORTERCORRE;

	AlturaJugador = Moviments[MovimentActual].getAltura();
	AmpladaJugador = Moviments[MovimentActual].getAmplada();
	Altura = AlturaJugador;
	// setRadi(AmpladaJugador*0.5);

	setDimensions(Peu->w, Peu->h, Peu->h);
	setRadi(Peu->h * 0.5);

	EsticEstirat = false;
	// ------------ MAQUINA D'ESTATS --------------------------------------------------------------
	maquinaEstats = new MaquinaEstats<Porter>(this);

	maquinaEstats->setEstatActual(estatinicial);
	maquinaEstats->setEstatAnterior(estatinicial);
	maquinaEstats->setEstatGlobal(PorterEstatGlobal::Instance());
	maquinaEstats->getEstatActual()->Enter(this);
}

Porter::~Porter()
{
	SDL_FreeSurface(Peu);
	//	SDL_FreeSurface(Imatge);
	//	delete LimitadorXuts;
	delete maquinaEstats;
	delete[] Moviments;
}

bool Porter::Mou()
{
	//run the logic for the current state
	maquinaEstats->Update();
	// #define DEBUG_EQUIP
	//calculate the combined force from each steering behavior
	Punt3 SteeringForce = ComportamentJugador->Calculate();

	//Acceleration = Force/Mass
	Punt3 Acceleration = SteeringForce / Massa;

	//update velocity
	Velocitat += Acceleration;
	//make sure player does not exceed maximum velocity
	Velocitat.Truncate(maximaVelocitat);
	//update the position
	Lloc += Velocitat;

	//enforce a non-penetration constraint if desired
	// if(Prm.bNonPenetrationConstraint)
	// {
	EnforceNonPenetrationContraint(this, AutoList<JugadorBase>::GetAllMembers());
	// }

	//update the heading if the player has a non zero velocity
	if (!Velocitat.isZero())
	{
		MirantA = VectorNormalitza(Velocitat);
		m_vSide = MirantA.Perpendicular();
	}
	//look-at vector always points toward the ball
	if (!getCamp()->isEnMansDelPorter())
	{
		Mirant = VectorNormalitza(getPilota()->getPosicio() - getPosicio());
	}
	// Posem el Z-Order
	setZOrder(getPosicioImatge().y + getPosicioImatge().h - RectJugador.h * 0.2);

	// ---------------------   Miro si es veu...
	SDL_Rect dest, orig;
	SDL_Rect Visio = getCamp()->getZonaVisible();
	orig = getPosicioImatge();
	// Només el pinto si esta dins de la pantalla
	if (IntersectRect(&dest, &orig, &Visio))
	{
		Renderitza->Afegir(this);
	}
	// ----------------------------------------
	return true;
}
// Pinta el porter
void Porter::Pinta(SDL_Surface *Fons)
{
	SDL_Rect dest, orig, QuinaImatge;
	SDL_Rect Visio = getCamp()->getZonaVisible();

	SDL_Rect PosSprite = getPosicioImatge();
	PosSprite.x -= Visio.x;
	PosSprite.y -= Visio.y;
	// Pinta la ombra de la pilota

	SDL_BlitSurface(Peu, 0, Fons, &PosSprite);

	RectJugador.y = getPosicioImatge().y - RectJugador.h * 0.8;
	RectJugador.x = getPosicioImatge().x - (PosSprite.w - RectJugador.w) * 0.2;
	RectJugador.x -= Visio.x;
	RectJugador.y -= Visio.y;

	RectJugador.h = AlturaJugador;
	RectJugador.w = AmpladaJugador;

	// ------------- EN QUINA DIRECCIO ESTA MIRANT? -------	Punt3 temp(1.0,0.0,0.0);
	Punt3 temp(1.0, 0.0, 0.0);
	// double producte = temp.Dot(MirantA);
	double producte = temp.Dot(Mirant);
	double angle = acos(producte);

	// if (getMirant().y>0) angle = DOSPI - angle;
	if (Mirant.y > 0)
		angle = DOSPI - angle;
	// PERQUE a vegades dóna -1.000000095
	if (producte < -1)
	{
		producte = -1;
	}

	int tria = angle / PIQUARTS;

	Moviments[MovimentActual].Avanca((Direccions)tria);
	// Si està aturat no ha de moure's
	if (getVelocitat() == 0)
		Moviments[MovimentActual].stop();

	SDL_BlitSurface(Moviments[MovimentActual].getImatge(), NULL, Fons, &RectJugador);
	/*
	RetardMoviment++;
	if (RetardMoviment==EsperaMoviment)
	{
		QuinaImatgeToca = (QuinaImatgeToca+1)%4;
		if (getVelocitat()==0) QuinaImatgeToca=0;
		RetardMoviment=0;
	}
	QuinaImatge.x = QuinaImatgeToca*(AmpladaJugador+4);
	QuinaImatge.y = tria * (RectJugador.h+1);
	QuinaImatge.h = RectJugador.h;
	QuinaImatge.w = RectJugador.w;

	SDL_BlitSurface(Imatge, &QuinaImatge, Fons, &RectJugador);
*/
}

// Ens retorna si està fora del camp
bool Porter::isForaDelCamp() const
{
	return getCamp()->getTerrenyDeJoc()->isFora(getPosicio());
}
// Ens retorna si ha de sortir a buscar la pilota
bool Porter::isAbastDeIntercepcio() const
{
	return (VectorDistanciaSq(getEquip()->getPorteriaNostra()->getMig(), getPilota()->getPosicio()) <= Porter_SurtAPerPilota());
}

// Dóna si el porter està massa lluny de la porteria i per tant ha de tornar
bool Porter::isMassaLlunyDeLaPorteria() const
{
	return (VectorDistanciaSq(getPosicio(), GetRearInterposeTarget()) > Porter_SurtAPerPilota());
};

// Es posiciona entre la porteria i la línia de xut del davanter
Punt3 Porter::GetRearInterposeTarget() const
{
	double xPosTarget = getEquip()->getPorteriaNostra()->getMig().x;
	double yPosTarget = getCamp()->getTerrenyDeJoc()->getPuntMig().y -
						getEquip()->getPorteriaNostra()->getAmpladaPorteria() * 0.5 +
						(getPilota()->getPosicio().y * getEquip()->getPorteriaNostra()->getAmpladaPorteria() / getCamp()->getTerrenyDeJoc()->getAltura());

	return Punt3(xPosTarget, yPosTarget, 0.0);
}
//-------------------- HandleMessage -------------------------------------
//
//  captura els missatges
//------------------------------------------------------------------------
bool Porter::RebreMissatge(const Missatge &msg)
{
	return maquinaEstats->capturaMissatge(msg);
}
