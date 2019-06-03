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
#include "Jugador.h"
#include "general.h"
#include "punt3.h"
#include "C2DMatrix.h"
#include "estat.h"
#include "Jugadorbase.h"
#include "JugadorEstats.h"
#include "Renderator.h"

#include <SDL/SDL_ttf.h>

Jugador::Jugador(const char *fitxerimatge, const char *nom, Equip *nos, Regio casa,
				 Estat<Jugador> *estatinicial, Punt3 mirant, double radi,
				 Punt3 velocitat, double massa, double maxforce, double maxspeedsense, double maxspeedamb,
				 double maxgir, double xut, double punteria, double provaxut, double passades, double zconf,
				 double dista, double espera, double dribling, jugo_de paper,
				 double Escala, int anima) : JugadorBase(nos, nom, casa, mirant, radi, velocitat, massa, maxforce,
														 maxspeedsense, maxspeedamb, maxgir, xut, punteria,
														 provaxut, passades, zconf, dista, espera, dribling, paper)
{
	char sNumero[3];

	setTipusEntitat(tipus_entitat_jugador);

	NomJugador = nom;

	// Abast Rebre
	JUG_DistanciaRecepcio = 132 * 132 * getCamp()->getZoomX(); // * Escala;

	// --------------- GRAFICS -----------------------------------------------------

	Peu = EscalaImatge("jugador-ombra.png", Escala);
	Peu2 = EscalaImatge("jugador-ombra2.png", Escala);

	// Crear l'array d'animacions per cada moviment disponible
	Moviments = new Animacio[anima];
	// Carregar els diferents moviments
	for (int j = 0; j < anima; j++)
	{
		Moviments[j].load((char *)fitxerimatge, j);
		Moviments[j].setEscala(Escala);
	}

	MovimentActual = JUGADORCORRE;
	// número del jugador
	// Per mostrar el número de jugador
	TTF_Font *font = TTF_OpenFont(getFullFileName("start.ttf").c_str(), 20 * Escala);
	if (!font)
	{
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		exit(-1);
	}
	SDL_Color color = {125, 125, 125};
	sprintf(sNumero, "%d", getID());
	Numero = TTF_RenderText_Solid(font, sNumero, color);

	if (font != NULL)
		TTF_CloseFont(font);
	font = NULL;
	// ----------------- Iniciar variables diverses  -------------------------

	// AlturaJugador = 90 * Escala;
	// AmpladaJugador = 45 * Escala;
	AlturaJugador = Moviments[MovimentActual].getAltura();
	AmpladaJugador = Moviments[MovimentActual].getAmplada();
	Altura = AlturaJugador;
	setRadi(AmpladaJugador * 0.5);

	// Pilota a l'abast de xutar ...
	JUG_PilotaAbastXut = 900 * Escala;
	JUG_PilotaAbastDesti = 1500 * Escala;

	// ------------ MAQUINA D'ESTATS --------------------------------------------------------------
	maquinaEstats = new MaquinaEstats<Jugador>(this);
	if (estatinicial)
	{
		maquinaEstats->setEstatActual(estatinicial);

		if (estatinicial)
		{
			maquinaEstats->setEstatActual(estatinicial);
			maquinaEstats->setEstatAnterior(estatinicial);
			maquinaEstats->setEstatGlobal(JugadorEstatGlobal::Instance());

			maquinaEstats->getEstatActual()->Enter(this);
		}

		ComportamentJugador->SeparationOn();
	}
	LimitadorXuts = new Regulator(FREQUENCIAXUTSPERSEGON);
}

Jugador::~Jugador()
{
	SDL_FreeSurface(Peu);
	SDL_FreeSurface(Peu2);
	//	SDL_FreeSurface(Imatge);
	SDL_FreeSurface(Numero);
	delete LimitadorXuts;
	delete maquinaEstats;
	delete[] Moviments;
}

bool Jugador::Mou(void)
{
	//run the logic for the current state
	maquinaEstats->Update();

	//calculate the combined steering force
	ComportamentJugador->Calculate();

	// Si no fa força, decelerar el jugador
	if (ComportamentJugador->getForce().isZero())
	{
		const double BrakingRate = 0.8;
		Velocitat = Velocitat * BrakingRate;
	}

	//the steering force's side component is a force that rotates the
	//player about its axis. We must limit the rotation so that a player
	//can only turn by PlayerMaxTurnRate rads per update.
	double TurningForce = ComportamentJugador->SideComponent();
	Clamp(TurningForce, -Jugador_MaxGir(), Jugador_MaxGir());

	//rotate the heading vector

	VectorRodaPerOrigen(MirantA, TurningForce);

	//make sure the velocity vector points in the same direction as
	//the heading vector
	Velocitat = MirantA * Velocitat.Llargada();

	//and recreate m_vSide
	m_vSide = MirantA.Perpendicular();

	//now to calculate the acceleration due to the force exerted by
	//the forward component of the steering force in the direction
	//of the player's heading
	Punt3 accel = MirantA * ComportamentJugador->ForwardComponent() / Massa;
	Velocitat += accel;
	// printf(".... Velocitat: %d (%f,%f,%f)\n", getID(), Velocitat.x, Velocitat.y, Velocitat.h);

	//make sure player does not exceed maximum velocity
	Velocitat.Truncate(maximaVelocitat);

	//update the position
	Lloc += Velocitat;

	//enforce a non-penetration constraint if desired
	// if(Prm.bNonPenetrationConstraint)
	// {
	EnforceNonPenetrationContraint(this, AutoList<JugadorBase>::GetAllMembers());

	// Posem el Z-Order:
	// setZOrder(getPosicioImatge().y + getPosicioImatge().h - RectJugador.h*0.2);
	setZOrder(getPosicio().y);
	// ---------------------   Miro si es veu...
	SDL_Rect dest, orig;
	SDL_Rect Visio = getCamp()->getZonaVisible();
	orig = getPosicioImatge();
	// Nomes el pinto si esta dins de la pantalla
	if (IntersectRect(&dest, &orig, &Visio))
	{
		Renderitza->Afegir(this);
	}
	// ----------------------------------------
	return 0;
}

bool Jugador::RebreMissatge(const Missatge &msg)
{
	return maquinaEstats->capturaMissatge(msg);
};

void Jugador::Pinta(SDL_Surface *Fons)
{

	SDL_Rect QuinaImatge;
	int MovimentImatgeY = 0;

	SDL_Rect Visio = getCamp()->getZonaVisible();

	// Si el criden, està dins de la pantalla
	SDL_Rect PosSprite = getPosicioImatge();
	PosSprite.x -= Visio.x;
	PosSprite.y -= Visio.y;

	// Pinta la ombra
	if (Nosaltres->getJugadorAmbLaPilota() == this)
	{
		SDL_BlitSurface(Peu2, 0, Fons, &PosSprite);
	}
	else
	{
		SDL_BlitSurface(Peu, 0, Fons, &PosSprite);
	}

	PosSprite.y -= PosSprite.h;
	SDL_BlitSurface(Numero, 0, Fons, &PosSprite);

	// Pintar el tio
	RectJugador.y = getPosicioImatge().y - RectJugador.h * 0.8;
	RectJugador.x = getPosicioImatge().x - (PosSprite.w - RectJugador.w) * 0.2;
	RectJugador.x -= Visio.x;
	RectJugador.y -= Visio.y;

	// Aquests dos em penso que els ignorarà i de fet
	// amb la classe Animació ja no em fan falta
	RectJugador.h = AlturaJugador;
	RectJugador.w = AmpladaJugador;

	// ------------- EN QUINA DIRECCIO ESTA MIRANT? --------
	Punt3 temp(1.0, 0.0, 0.0);

	double producte = temp.Dot(MirantA);

	// PERQUE a vegades dóna -1.000000095
	if (producte < -1)
	{
		producte = -1;
	}
	double angle = acos(producte);

	if (getMirant().y > 0)
		angle = DOSPI - angle;

	int tria = QUATREDIVPI * angle;

	//	Amb la classe Animacio només hauria de caldre cridar-la perquè ja l'haurem
	//	carregat correctament. Això implica que podem pintar tota la imatge directament
	//	sense preocupar-nos de les dimensions

	Moviments[MovimentActual].Avanca((Direccions)tria);
	// Si està aturat no ha de moure's
	if (getVelocitat() == 0)
		Moviments[MovimentActual].stop();

	SDL_BlitSurface(Moviments[MovimentActual].getImatge(), NULL, Fons, &RectJugador);

	// Això si, els d'ajuda s'han de buscar la videta i canviar
	// la variable MovimentActual, com ho ha de fer el xut
	//

	/*
	RetardMoviment++;
	if (RetardMoviment==EsperaMoviment)
	{
		QuinaImatgeToca = (QuinaImatgeToca+1)%4;
		if (getVelocitat()==0) QuinaImatgeToca=0;

		QuinMovimentToca=tria;

		RetardMoviment=0;
		// Si estem ajudant l'atac ha d'aixecar la mà
		std::string s = getFSM()->getNomEstatActual();
		if (getFSM()->getNomEstatActual()=="JugadorEstatAjudaAtac") QuinaImatgeToca+=4;
	}
	QuinaImatge.x = QuinaImatgeToca*(AmpladaJugador+3);
	QuinaImatge.y = QuinMovimentToca * (RectJugador.h+1);
	QuinaImatge.h = RectJugador.h;
	QuinaImatge.w = RectJugador.w;
	
	SDL_BlitSurface(Imatge, &QuinaImatge, Fons, &RectJugador);
*/

	// ------------- ACABADA RECERCA IMATGE --------
	// Un rectangle de DEBUG
	// RectJugador.x = getPosicio().x - Visio.x;
	// RectJugador.y = getPosicio().y - Visio.y;
	// RectJugador.w = sqrt(PILOTAABASTREBRE);
	// RectJugador.h = sqrt(PILOTAABASTREBRE);

	// SDL_FillRect(Fons, &RectJugador,(Uint32) 0xFF0000);
}
