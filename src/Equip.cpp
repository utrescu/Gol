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
#include "Equip.h"
#include "C2DMatrix.h"
// #include "punt3.h"
#include "general.h"
#include "Pantalla.h"
#include "Jugadorbase.h"
#include "Jugador.h"
#include "Porter.h"
#include "Pilota.h"
#include "EquipEstats.h"
#include "missatger.h"
#include "JugadorMissatges.h"
#include "JugadorEstats.h"
#include "PorterEstats.h"
#include "Regio.h"
#include "EntitatsGestor.h"
#include "CalculadoraPuntsSuport.h"
#include "Animacio.h"

#include "LUAManager.h"

Equip::Equip(const char *nomfitxer, Porteria *nostra, Porteria *seva, Pantalla *pant, quin_equip costat,
						 double Escala) : PorteriaContraria(seva),
															PorteriaNostra(nostra),
															Contraris(NULL),
															Camp(pant),
															Soc(costat),
															DistanciaAlaPilotaDelMillor(0.0),
															JugadorMesProperALaPilota(NULL),
															JugadorAmbLaPilota(NULL),
															JugadorDonantSuport(NULL),
															JugadorDeRecepcio(NULL)
{
	// Iniciar la maquina d'estats
	maquinaEstats = new MaquinaEstats<Equip>(this);

	maquinaEstats->setEstatActual(EquipEstatDefensant::Instance());
	maquinaEstats->setEstatAnterior(EquipEstatDefensant::Instance());
	maquinaEstats->setEstatGlobal(NULL);

	//crear els jugadors
	CrearJugadors(nomfitxer, Escala);

	//posar el comportament per defecte
	std::vector<JugadorBase *>::iterator it = Jugadors.begin();

	for (it; it != Jugadors.end(); ++it)
	{
		(*it)->getComportament()->SeparationOn();
	}

	//crear la calculadora de punts de suport
	PuntsSuport = new CalculadoraPuntsSuport(NumSupportSpotsX, NumSupportSpotsY, this);
}

Equip::~Equip()
{
	delete maquinaEstats;
	// Eliminar els jugadors de l'equip
	std::vector<JugadorBase *>::iterator it = Jugadors.begin();
	for (it; it != Jugadors.end(); ++it)
	{
		delete *it;
	}
	// Eliminar els possibles punts de suport
	delete PuntsSuport;
}

void Equip::CrearJugadors(const char *nomfitxer, double Escala)
{
	// SDL_Rect posicioinicial;
	Punt3 MiraCapA;
	char taulajugador[11] = {"JugadorX"};

	LUAManager *lm;

	lm = new LUAManager(nomfitxer);

	double ZoomX = getCamp()->getZoomX();
	double ZoomY = getCamp()->getZoomY();

	if (Soc == local)
	{
		MiraCapA = Punt3(0.0, 1.0, 0.0);
	}
	else
	{
		MiraCapA = Punt3(0.0, -1.0, 0.0);
	}
	// Proves per xutar de l'equip, com més gran més fidelitat
	ProvesPerXutar = lm->getNumeroint("ProvesPerXut");

	int numeroJugadors = lm->getNumeroint("NumJugadors");

	for (int i = 0; i < numeroJugadors; i++)
	{
		// Obtenir el camp de la taula
		if (i == 0)
			sprintf(taulajugador, "Porter");
		else
			sprintf(taulajugador, "Jugador%d", i);

		// ---  Obtenir les seves posicions dins del camp
		PosicionsDefensives[i].x = lm->getTaulaNumeroint(taulajugador, "DefensaX") * ZoomX;
		PosicionsDefensives[i].y = lm->getTaulaNumeroint(taulajugador, "DefensaY") * ZoomY;
		PosicionsDefensives[i].w = 200 * Escala;
		PosicionsDefensives[i].h = 200 * Escala;
		PosicionsAtacants[i] = PosicionsDefensives[i];
		PosicionsAtacants[i].x = lm->getTaulaNumeroint(taulajugador, "AtacX") * ZoomX;
		PosicionsAtacants[i].y = lm->getTaulaNumeroint(taulajugador, "AtacY") * ZoomY;
		// ---- Sistema gràfic: La imatge i les animacions
		std::string Fitxer = lm->getTaulaCadena(taulajugador, "Imatge");
		int anim = lm->getTaulaNumeroint(taulajugador, "Animacions");
		// ---- Detalls individuals
		std::string Nom = lm->getTaulaCadena(taulajugador, "Nom");

		double MassaCorporal = lm->getTaulaNumerofloat(taulajugador, "Massa");
		double ForcaMaximum = lm->getTaulaNumerofloat(taulajugador, "Forca");
		double RotaMaximum = lm->getTaulaNumerofloat(taulajugador, "Rotacio");
		double VelocitatSense = lm->getTaulaNumerofloat(taulajugador, "VelocitatSensePilota") * Escala;
		double VelocitatAmb = lm->getTaulaNumerofloat(taulajugador, "VelocitatAmbPilota") * Escala;
		// xuts
		double xut = lm->getTaulaNumerofloat(taulajugador, "ForcaXut");			 // *Escala?
		double punteria = lm->getTaulaNumerofloat(taulajugador, "Punteria"); // *Escala?
		double provaxut = lm->getTaulaNumerofloat(taulajugador, "PossibleXutBoig");
		// passades
		double passada = lm->getTaulaNumerofloat(taulajugador, "ForcaPassada"); // *Escala?
		double dista = lm->getTaulaNumerofloat(taulajugador, "MinimaDistanciaPassada") * Escala;
		double espera = lm->getTaulaNumerofloat(taulajugador, "EsperaOHiVa");
		double zconf = lm->getTaulaNumerofloat(taulajugador, "ZonaComfort") * ZoomX;
		// dribling
		double dribling = lm->getTaulaNumerofloat(taulajugador, "ForcaDribbling"); // *Escala?

		// ---- Posició dins del camp del jugador
		// Ara distingir si és un jugador o un porter
		int quees = lm->getTaulaNumeroint(taulajugador, "JugaDe");

		if (quees == 1)
		{
			// Específiques del porter
			double segueix = lm->getTaulaNumerofloat(taulajugador, "DistanciaSeguiment");
			double surt = lm->getTaulaNumerofloat(taulajugador, "DistanciaSortirAPerPilota");
			double agafa = lm->getTaulaNumerofloat(taulajugador, "DistanciaAgafarPilota");
			double reaccio = lm->getTaulaNumerofloat(taulajugador, "ReccioPorter");

			// És el porter, per tant recalculen la seva zona (centrat en un requadre de 100)
			PosicionsDefensives[i].x = PosicionsDefensives[i].x - 50 * ZoomX;
			PosicionsAtacants[i].x = PosicionsAtacants[i].x - 50 * ZoomX;
			// Ara la y
			PosicionsDefensives[i].y = PosicionsDefensives[i].y - 50 * ZoomY;
			PosicionsAtacants[i].y = PosicionsAtacants[i].y - 50 * ZoomY;

			PosicionsDefensives[i].w = 100 * ZoomX;
			PosicionsDefensives[i].h = 100 * ZoomY;
			// Porter
			PosicionsAtacants[i].w = PosicionsDefensives[i].w;
			PosicionsAtacants[i].h = PosicionsDefensives[i].h;

			Jugadors.push_back(new Porter(Fitxer.c_str(),												 // Fitxer d'imatge
																		Nom.c_str(),													 // Nom del jugador
																		this,																	 // Equip del jugador
																		PosicionsDefensives[i],								 // Posició d'inici
																		PorterEstatCobrirPorteria::Instance(), // Estat inicial
																		MiraCapA,															 // Quin és el seu costat
																		0,																		 // Radi (no es fa servir encara)
																		Punt3(0.0, 0.0, 0.0),									 // Velocitat inicial
																		MassaCorporal,												 // Massa
																		ForcaMaximum,													 // Força
																		VelocitatSense,												 // Velocitat màxima sense pilota
																		VelocitatAmb,													 // Velocitat màxima amb la pilota
																		RotaMaximum,													 // Velocitat màxima de rotació
																		xut,																	 // Forca màxima de xut
																		punteria,															 // Punteria del jugador 0..1
																		provaxut,															 // Provabilitat de xuts a la babala
																		passada,															 // Forca passada
																		zconf,																 // Zona confiança
																		dista,																 // Distancia minima de passada
																		espera,																 // provabilitat d'esperar o anar a per la pilota
																		dribling,															 // Forca dribling
																		surt,																	 // A quina distancia surt el porter
																		segueix,															 // Seguiment pilota
																		reaccio,															 // Reaccio del porter
																		agafa,																 // Afagar la pilota (porter més bo!)
																		JugadorBase::porter,									 // De que juga el jugador
																		Escala,																 // Escalat de la imatge
																		anim));																 // Quantes animacions té el jugador
		}
		else
		{
			// És un jugador
			Jugadors.push_back(new Jugador(Fitxer.c_str(),									 // Nom del fitxer d'imatge
																		 Nom.c_str(),											 // Nom del jugador
																		 this,														 // De quin equip soc
																		 PosicionsDefensives[i],					 // Posicio per començar
																		 JugadorEstatEsperant::Instance(), // Estat
																		 MiraCapA,												 // Mirant
																		 0,																 // radi
																		 Punt3(0.0, 0.0, 0.0),						 // Velocitat
																		 MassaCorporal,										 // Massa jugador
																		 ForcaMaximum,										 // Força maxima
																		 VelocitatSense,									 // Velocitat maxima sense pilota
																		 VelocitatAmb,										 // Velocitat maxima amb pilota
																		 RotaMaximum,											 // Velocitat de gir màxima
																		 xut,															 // Forca xut
																		 punteria,												 // Punteria del jugador 0..1
																		 provaxut,												 // Provabilitat de xuts a la babala
																		 passada,													 // Número de proves per xutar
																		 zconf,														 // Zona confiança
																		 dista,														 // Distancia minima de passada
																		 espera,													 // provabilitat d'esperar o anar a per la pilota
																		 dribling,												 // Forca dribling
																		 (JugadorBase::jugo_de)quees,			 // De que juga el jugador
																		 Escala,													 // Escala
																		 anim));													 // Número d'animacions
		}
	}

	std::vector<JugadorBase *>::iterator it = Jugadors.begin();

	for (it; it != Jugadors.end(); ++it)
	{
		GestorEntitats->RegisterEntity(*it);
	}

	delete lm;
}

void Equip::Mou()
{
	CalculaMesProperAPilota();
	CalculaContrariMesProperASevaPorteria();

	maquinaEstats->Update();

	// Actualitza la posicio dels jugadors
	std::vector<JugadorBase *>::iterator it = Jugadors.begin();

	for (it; it != Jugadors.end(); ++it)
	{
		(*it)->Mou();
	}
}

void Equip::Pinta(SDL_Surface *p)
{

	// std::vector<JugadorBase*>::const_iterator it = Jugadors.begin();

	PuntsSuport->Pinta();
	if (TincPilota())
	{
		SDL_Rect Posicio;
		Posicio.x = 390;
		Posicio.y = 0;
		Posicio.w = 20;
		Posicio.h = 20;
		if (getQuinEquip() == EQUIP_LOCAL)
			SDL_FillRect(p, &Posicio, 0x0000FF);
		else
			SDL_FillRect(p, &Posicio, 0xFFFF40);
	}

	// Ara no calen perquè tinc el Renderator!!
	/*
	for (it; it != Jugadors.end(); ++it)
	{
		// Li recordo que el que s'ha de pintar es el jugador, no 
		// la estructura JugadorBase 
		// Jugador* plyr = static_cast<Jugador*>(*it);
		// plyr->Pinta(p);
		(*it)->Pinta(p);
	}
	*/
}

// Passa tots els jugadors a posicions d'atac
void Equip::PassarAPosicionsAtacants()
{
	int i = 0;
	std::vector<JugadorBase *>::const_iterator it = Jugadors.begin();
	for (it; it != Jugadors.end(); ++it)
	{
		(*it)->setRegioNormal(PosicionsAtacants[i]);
		i++;
	}
}

// Passa tots els jugadors a posicions de defensa
void Equip::PassarAPosicionsDefensives()
{
	int i = 0;
	std::vector<JugadorBase *>::const_iterator it = Jugadors.begin();
	for (it; it != Jugadors.end(); ++it)
	{
		(*it)->setRegioNormal(PosicionsDefensives[i]);
		i++;
	}
}

void Equip::CalculaMesProperAPilota()
{
	double Proper = MAXDOUBLE;

	std::vector<JugadorBase *>::iterator it = Jugadors.begin();

	for (it; it != Jugadors.end(); ++it)
	{
		// distancia a la pilota al quadrat per evitar l'arrel quadrada
		Pilota *pi = Camp->getPilota();
		double dist = VectorDistanciaSq((*it)->getPosicio(), pi->getPosicio());
		// Que els jugadors es guardin la seva distancia
		(*it)->setDistanciaAPilota(dist);

		if (dist < Proper)
		{
			Proper = dist;
			JugadorMesProperALaPilota = *it;
		}
	}
	DistanciaAlaPilotaDelMillor = Proper;
}

void Equip::CalculaContrariMesProperASevaPorteria()
{
	double ClosestSoFar = MAXFLOAT;

	std::vector<JugadorBase *>::const_iterator it = getContraris()->Membres().begin();

	for (it; it != getContraris()->Membres().end(); ++it)
	{
		//calculate the dist. to home goal, remember the opponents home goal
		// is our target
		if ((*it)->getDeQueFaig() != JugadorBase::porter)
		{
			double dist = (*it)->DistanciaPorteriaNostra();
			if (dist < ClosestSoFar)
			{
				ClosestSoFar = dist;
				JugadorContrariMesProperASevaPorteria = *it;
			}
		}
	}
}

// Inicialment era una altra cosa, però en realitat el que fa és comprovar si el jugador
// està marcat per un contrari o no
bool Equip::isHiHaOponentADistancia(Punt3 pos, double radi)
{
	std::vector<JugadorBase *>::const_iterator end = getContraris()->Membres().end();
	std::vector<JugadorBase *>::const_iterator it;
	// Per estalviar calculs el faig al quadrat i d'aquesta forma no caldrà fer arrels quadrades
	double radisq = radi * radi;

	for (it = getContraris()->Membres().begin(); it != end; ++it)
	{
		if (VectorDistanciaSq(pos, (*it)->getPosicio()) < radisq)
		{
			return true;
		}
	}

	return false;
}

// Fer que el porter torni a casa
// Normalment serà perquè s'ha allunyat massa de la porteria
void Equip::PorterACasa() const
{
	//	PassarAPosicionsDefensives();

	std::vector<JugadorBase *>::const_iterator it = Jugadors.begin();
	for (it; it != Jugadors.end(); ++it)
	{
		// Fer tornar tothom cap a casa amb un missatge
		if ((*it)->getDeQueFaig() == JugadorBase::porter)
		{
			Carter->DispatchMsg(SEND_MSG_IMMEDIATELY,
													1,
													(*it)->getID(),
													Msg_GoHome,
													NULL);
		}
	}
}

// Tornar tots els jugadors a les seves posicions inicials.
// Normalment serà perquè s'ha fet un gol, s'ha acabat la primera part, ...
//
void Equip::TothomACasa() const
{
	//	PassarAPosicionsDefensives();

	std::vector<JugadorBase *>::const_iterator it = Jugadors.begin();
	for (it; it != Jugadors.end(); ++it)
	{
		// Fer tornar tothom cap a casa amb un missatge
		if ((*it)->getDeQueFaig() != JugadorBase::porter)
		{
			Carter->DispatchMsg(SEND_MSG_IMMEDIATELY,
													1,
													(*it)->getID(),
													Msg_GoHome,
													NULL);
		}
	}
}

// SacaFalta()
// El faig servir per les sacades de banda o de porteria.
// S'ha de canviar ja que les sacades de banda són diferents (ja que tenen
// imatges diferents
void Equip::SacaFalta()
{
	CalculaMesProperAPilota();
	Carter->DispatchMsg(SEND_MSG_IMMEDIATELY,
											1,
											JugadorMesProperALaPilota->getID(),
											Msg_AnarASaque,
											NULL);
}

void Equip::PilotaCorner()
{
	if (getJugadorAmbLaPilota() != NULL)
	{
		// són els atacants
		PassarAPosicionsAtacants();
		CanviaObjectiusDelsAturats();
		// Li direm que saqui la falta quan estigui segur que es posiciona be. Per ara ...
		// SacaFalta();
	}
	else
	{
		PassarAPosicionsDefensives();
		CanviaObjectiusDelsAturats();
	}
}

void Equip::PilotaFora()
{
	// Per evitar els problemes que tinc, faré que sempre sigui el porter
	// el que vagi a buscar la pilota...
	if (getJugadorAmbLaPilota() != NULL)
	{
		// Tothom ha de tornar perquè no tenim la pilota i per tant ara
		// ataca l'equip contrari
		TothomACasa();
	}
	else
	{
		PassarAPosicionsDefensives();
		CanviaObjectiusDelsAturats();
		/* Carter->DispatchMsg(SEND_MSG_IMMEDIATELY,
				    1,
				    JugadorMesProperALaPilota->getID(),
				    Msg_AnarASaque,
				    NULL);
		*/
	}
}

void Equip::PilotaForaBanda()
{
	// Sóc el que ha tirat la pilota a fora, per tant
	if (getJugadorAmbLaPilota() != NULL)
	{
		TothomACasa();
	}
	else
	{
		// Avisar al jugador més proper a la pilota perquè vagi a
		// buscar-la
		Carter->DispatchMsg(SEND_MSG_IMMEDIATELY,
												1,
												JugadorMesProperALaPilota->getID(),
												Msg_AnarASaque,
												NULL);
	}
}

void Equip::HemMarcat() const
{
	/*
	std::vector<JugadorBase*>::const_iterator it = Jugadors.begin();

	for (it; it != Jugadors.end(); ++it)
	{
		// Fer tornar tothom cap a casa amb un missatge 
		if ((*it)->getDeQueFaig() != JugadorBase::porter)
		{
			Carter->DispatchMsg(SEND_MSG_IMMEDIATELY,
					    1,
					    (*it)->getID(),
					    Msg_Gol,
					    NULL);
		}
	}	
*/
}

// isTothomACasa()
// Serveix per comprovar que tothom ha tornat a les seves posicions inicials
// Això és perquè quan es marca un gol, el joc no recomenci fins que tots els jugadors no
// hagin tornat a les seves posicions
bool Equip::isTothomACasa()
{
	std::vector<JugadorBase *>::const_iterator it = Jugadors.begin();

	for (it; it != Jugadors.end(); ++it)
	{
		if ((*it)->getDeQueFaig() != JugadorBase::porter)
		{
			if ((*it)->isACasa() == false)
			{
				return false;
			}
		}
	}
	return true;
}

void Equip::CanviaObjectiusDelsAturats()
{
	std::vector<JugadorBase *>::const_iterator it = Jugadors.begin();

	for (it; it != Jugadors.end(); ++it)
	{
		if ((*it)->getDeQueFaig() != JugadorBase::porter)
		{
			//cast to a Jugador
			Jugador *plyr = static_cast<Jugador *>(*it);

			if (plyr->getFSM()->isEnEstat(*JugadorEstatEsperant::Instance()) ||
					plyr->getFSM()->isEnEstat(*JugadorEstatTornaACasa::Instance()))
			{
				// plyr->getComportament()->setTarget(plyr->getRegioInicial()->getPuntMig());
				plyr->getComportament()->setTarget(plyr->getRegioNormal()->getPuntMig());
			}
		}
	}
}

void Equip::CridaElsDefenses()
{
	/*
	std::vector<JugadorBase*>::const_iterator it = Jugadors.begin();

	for (it; it != Jugadors.end(); ++it)
	{
		if ((*it)->getDeQueFaig() != JugadorBase::defensa)
		{
			Carter->DispatchMsg(SEND_MSG_IMMEDIATELY,
					    1,
					    (*it)->getID(),
					    Msg_Recula,
					    NULL);
		}
	}
*/
}

JugadorBase *Equip::getJugadorPerID(int id) const
{
	std::vector<JugadorBase *>::const_iterator it = Jugadors.begin();

	for (it; it != Jugadors.end(); ++it)
	{
		if ((*it)->getID() == id)
			return *it;
	}

	return NULL;
}

void Equip::setPosicioNormal(int plyr, SDL_Rect region) const
{
	// assert ( (plyr>=0) && (plyr<m_Players.size()) );
	Jugadors[plyr]->setRegioNormal(region); // SetHomeRegion(region);
}

void Equip::DemanaPassada(JugadorBase *player)
{
	if (RandFloat() > 0.1)
		return;

	if (isPassadaLliureTotsContraris(JugadorAmbLaPilota->getPosicio(),
																	 player->getPosicio(),
																	 player,
																	 player->Jugador_ForcaPassada()))
	{
		//tell the player to make the pass
		//let the receiver know a pass is coming
		Carter->DispatchMsg(SEND_MSG_IMMEDIATELY,
												player->getID(),
												JugadorAmbLaPilota->getID(),
												Msg_PassToMe,
												player);
	}
}

bool Equip::BuscaPassada(const JugadorBase *const passer, JugadorBase *&receiver,
												 Punt3 &PassTarget, double power, double MinPassingDistance) const
{
	std::vector<JugadorBase *>::const_iterator curPlyr = Membres().begin();

	double ClosestToGoalSoFar = MAXFLOAT;
	Punt3 Target;

	// Mirar per tots els jugadors per veure quina es la millor passada de totes
	for (curPlyr; curPlyr != Membres().end(); ++curPlyr)
	{
		// No s'ha de poder passar a si mateix ...
		// I ha de ser una mica lluny perque sino no tenim cap avantatge
		if ((*curPlyr != passer) &&
				(VectorDistanciaSq(passer->getPosicio(), (*curPlyr)->getPosicio()) > MinPassingDistance * MinPassingDistance))
		{
			if (TriaMillorPassadaAlReceptor(passer, *curPlyr, Target, power))
			{
				// Si la passada es la mes propera a la porteria contraria, la
				// guardem per si de cas ... Potser sera bona :-)
				double Dist2Goal = fabs(Target.x - getPorteriaContraria()->getMig().x);

				if (Dist2Goal < ClosestToGoalSoFar)
				{
					ClosestToGoalSoFar = Dist2Goal;
					//guardar el jugador
					receiver = *curPlyr;
					// ...i l'objectiu
					PassTarget = Target;
				}
			}
		}
	}

	if (receiver)
		return true;

	else
		return false;
}

//------------- DetermineBestSupportingAttacker ------------------------
//
// calculate the closest player to the SupportSpot
//------------------------------------------------------------------------
JugadorBase *Equip::BuscaMillorAtacantDeSuport()
{
	double ClosestSoFar = MAXFLOAT;

	JugadorBase *BestPlayer = NULL;

	std::vector<JugadorBase *>::iterator it = Jugadors.begin();

	for (it; it != Jugadors.end(); ++it)
	{
		//Només es miren els davanters que no són el que té la pilota
		if (((*it)->getDeQueFaig() == JugadorBase::davanter) && ((*it) != JugadorAmbLaPilota))
		{
			// calcula la distància (al quadrat per estalviar càlculs)
			double dist = VectorDistanciaSq((*it)->getPosicio(), PuntsSuport->GetBestSupportingSpot());

			// si és el més proper i no és el que té la pilota, es guarda
			if ((dist < ClosestSoFar))
			{
				ClosestSoFar = dist;
				BestPlayer = (*it);
			}
		}
	}
	return BestPlayer;
}

//---------------------- TriaMillorPassadaAlReceptor ---------------------------
//
//	Es calculen tres passades.
//	- La passada directa (1)
//	- Passades al costat en un 'radi' del receptor (2 i 3)
//  - SI ÉS MOLT PROPER, potser s'hauria de passar davant! (4)
//                  --2o --
//   Emisor       /Receptor \
//    X          (    1x     )o4
//                \         /
//                 -- 3o --
//	S'ha de mirar si la poden interceptar i que estigui dins de la zona
//	de gol.
//  Si cap passada val la pena pot arribar tonarà 'fals'
//  o bé la passada més propera al gol
//------------------------------------------------------------------------

bool Equip::TriaMillorPassadaAlReceptor(const JugadorBase *const passer, const JugadorBase *const receiver,
																				Punt3 &PassTarget, double power) const
{
	// primer, quant de temps tardara la pilota a arribar al jugador si
	// no es mou gens
	double time = getCamp()->getPilota()->TempsPerRecorrerDistancia(getPilota()->getPosicio(),
																																	receiver->getPosicio(), power);
	// retorna fals si la pilota no pot arribar despres del xut amb la potencia
	if (time < 0)
		return false;

	// Distacia minima que es pot moure el jugador en aquest temps
	double InterceptRange = time * receiver->getVelocitatMaxima();

	//Scale the intercept range
	const double ScalingFactor = 0.3;
	InterceptRange *= ScalingFactor;

	//now calculate the pass targets which are positioned at the intercepts
	//of the tangents from the ball to the receiver's range circle.
	Punt3 ip1, ip2;

	GetTangentPoints(receiver->getPosicio(), InterceptRange, getPilota()->getPosicio(), ip1, ip2);

	// Passada endavant
	// Punt3 ip3;
	// ip3 = receiver->getPosicio();
	// if (receiver->isInZonaCalenta() && !receiver->isEsticMarcat())
	// {
	// Si es donen les condicions avaluem un nou passe
	//	ip3.y += receiver->getRadi();
	// }

	const int NumPassesToTry = 3;
	Punt3 Passes[NumPassesToTry] = {ip1, receiver->getPosicio(), ip2};

	// Es considera el millor passe si:
	//  1. La passada mes propera a la porteria contraria es la millor ...
	//  2. Evidentment ha de ser dins del terreny de joc
	//  3. Si no la intercepten ja seria la ostia
	//  4. Si estem molt a prop de la porteria, passar endavant encara és millor perquè permetrà xutar...

	double ClosestSoFar = MAXFLOAT;
	bool bResult = false;

	for (int pass = 0; pass < NumPassesToTry; ++pass)
	{
		double dist = fabs(Passes[pass].x - getPorteriaContraria()->getMig().x);
		//
		if ((dist < ClosestSoFar) && getCamp()->getTerrenyDeJoc()->isDins(Passes[pass]) &&
				isPassadaLliureTotsContraris(getPilota()->getPosicio(), Passes[pass], receiver, power))
		{
			ClosestSoFar = dist;
			PassTarget = Passes[pass];
			bResult = true;
		}
	}

	return bResult;
}

//----------------------- isPassadaLliureDelContrari -------------------------
//
//  Comprovar si es pot fer un passe des del jugador 'from' fins al 'target' amb
// força 'PassingForce' sense que sigui interceptat per cap contrari.
//------------------------------------------------------------------------
bool Equip::isPassadaLliureDelContrari(Punt3 from, Punt3 target, const JugadorBase *const receiver,
																			 const JugadorBase *const opp, double PassingForce) const
{
	//move the opponent into local space.
	Punt3 ToTarget = target - from;
	Punt3 ToTargetNormalized = VectorNormalitza(ToTarget);
	Punt3 Perpend = ToTargetNormalized.Perpendicular();

	//	Punt3 LocalPosOpp = PointToLocalSpace(opp->getPosicio(), ToTargetNormalized, ToTargetNormalized.Perpendicular(), from);
	Punt3 LocalPosOpp = PointToLocalSpace(opp->getPosicio(), ToTargetNormalized, Perpend, from);

	// Si el contrari esta darrera nostre, no molestara per fer la passada
	// Podria haver-hi problemes si el contrari corris mes que la pilota ...
	if (LocalPosOpp.x < 0)
	{
		return true;
	}

	// si el contrari esta mes lluny que el lloc a on passem, hem de veure si te temps
	// d'arribar-hi abans que el receptor
	if (VectorDistanciaSq(from, target) < VectorDistanciaSq(opp->getPosicio(), from))
	{
		if (receiver)
		{
			if (VectorDistanciaSq(target, opp->getPosicio()) > VectorDistanciaSq(target, receiver->getPosicio()))
			{
				return true;
			}

			else
			{
				return false;
			}
		}

		else
		{
			return true;
		}
	}

	// calcular quant temps tardarà la pilota en arribar
	double TimeForBall = getCamp()->getPilota()->TempsPerRecorrerDistancia(Punt3(0.0, 0.0, 0.0),
																																				 Punt3(LocalPosOpp.x, 0.0, 0.0),
																																				 PassingForce);
	// Amb el temps determinat, mirem la distacia que pot correr el contrari
	double reach = opp->getVelocitatMaxima() * TimeForBall + getCamp()->getPilota()->getRadi() +
								 opp->getRadi();

	// si la distància que pot córrer l'oponent és més petita que el seu rang més el radi de la pilota
	// i el radi de l'oponent es pot interceptar
	if (fabs(LocalPosOpp.y) < reach)
	{
		return false;
	}

	return true;
}

//---------------------- isPassadaLliureTotsContraris ----------------------
//
//  Mirar si la passada es pot fer lliure de tots els contraris
//  cert si es pot fer el passe sense que sigui interceptat
//------------------------------------------------------------------------
bool Equip::isPassadaLliureTotsContraris(Punt3 from, Punt3 target,
																				 const JugadorBase *const receiver, double PassingForce) const
{
	std::vector<JugadorBase *>::const_iterator opp = getContraris()->Membres().begin();

	for (opp; opp != getContraris()->Membres().end(); ++opp)
	{
		if (!isPassadaLliureDelContrari(from, target, receiver, *opp, PassingForce))
		{
			return false;
		}
	}

	return true;
}

//------------------------ CanShoot --------------------------------------
//
//  Given a ball position, a kicking power and a reference to a vector2D
//  this function will sample random positions along the opponent's goal-
//  mouth and check to see if a goal can be scored if the ball was to be
//  kicked in that direction with the given power. If a possible shot is
//  found, the function will immediately return true, with the target
//  position stored in the vector ShotTarget.
//------------------------------------------------------------------------
bool Equip::PucXutar(Punt3 BallPos, double power, Punt3 &ShotTarget) const
{
	//the number of randomly created shot targets this method will test
	int NumAttempts = getProvesPerTrobarXut();

	while (NumAttempts--)
	{
		// Tria una posicio aleatoria dins de la porteria contraria
		ShotTarget = getPorteriaContraria()->getMig();

		//the y value of the shot position should lay somewhere between two
		//goalposts (taking into consideration the ball diameter)
		int MinYVal = getPorteriaContraria()->getForatEsquerra() + getCamp()->getPilota()->getRadi();
		int MaxYVal = getPorteriaContraria()->getForatDreta() - getCamp()->getPilota()->getRadi();

		ShotTarget.y = (double)RandInt(MinYVal, MaxYVal);

		// Assegurar-se de que el xut arribara a dins de la porteria
		double time = getCamp()->getPilota()->TempsPerRecorrerDistancia(BallPos, ShotTarget, power);

		//if it is, this shot is then tested to see if any of the opponents
		//can intercept it.
		if (time >= 0)
		{
			if (isPassadaLliureTotsContraris(BallPos, ShotTarget, NULL, power))
			{
				return true;
			}
		}
	}

	return false;
}
