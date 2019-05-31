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
#include "Jugadorbase.h"
#include "JugadorMissatges.h"
#include "missatger.h"
#include "Regio.h"

JugadorBase::JugadorBase(Equip *equ, std::string nom, Regio casa, Punt3 mirant, double radi, Punt3 veloc, double massa,
						 double maxforce, double maxvelocamb, double maxvelocsense, double maxgir, double xut, double punteria,
						 double provaxut, double passades, double zconf, double dista, double espera, double dribling, jugo_de deq) : EntitatMobil(radi, veloc, maxvelocsense, mirant, massa, maxgir, maxforce), Nosaltres(equ),
																																	  DistanciaAPilota(MAXFLOAT), DeQueJugo(deq)
{
	// Carregar les característiques especifiques del jugador
	// **** Velocitat
	NomJugador = nom;
	JUG_VelocitatSense = maxvelocsense;
	JUG_VelocitatAmb = maxvelocamb;

	// **** xuts
	JUG_ForcaXut = xut;
	JUG_Punteria = punteria;
	JUG_XutaIgualment = provaxut;
	// passades
	JUG_DistanciaMinimaPassada = dista;
	JUG_ForcaPassada = passades;
	JUG_EsperaOHiVa = espera;
	JUG_ZonaComfort = zconf * zconf;
	// dribbling
	JUG_ForcaDribbling = dribling;

	//----------------------------------

	RegioInicial = new Regio(0, casa);
	RegioNormal = new Regio(0, casa);

	Lloc = RegioNormal->getPuntMig();
	// MovingEntity(velocity, max_speed, heading, mass, max_turn_rate, max_force),

	// Crear els Steerings ....
	ComportamentJugador = new SteeringBehaviors(this, Nosaltres->getCamp(), getPilota());
	// El Destí inicial serà la seva posició inicial
	ComportamentJugador->setTarget(RegioNormal->getPuntMig());
}

JugadorBase::~JugadorBase()
{
}

// --------------------------------------- POSICIONAMENT -----------------------------
bool JugadorBase::isACasa()
{
	if (DeQueJugo == porter)
	{
		return RegioNormal->isDins(getPosicio());
		// Pitch()->GetRegionFromIndex(m_iHomeRegion)->Inside(Pos(), Region::normal);
	}
	else
	{
		return RegioNormal->isDins(getPosicio(), Regio::halfsize);
		// Pitch()->GetRegionFromIndex(m_iHomeRegion)->Inside(Pos(), Region::halfsize);
	}
	return true;
}

// Ens diu si el jugador està allà on volia anar
// L'animació hauria de canviar per el jugador aixecant la mà ...
bool JugadorBase::isADesti()
{
	return (VectorDistanciaSq(getPosicio(), getComportament()->getTarget()) < JUG_PilotaAbastDesti);
}

bool JugadorBase::isPosicioDavantNostra(Punt3 position) const
{
	Punt3 ToSubject = position - getPosicio();

	if (ToSubject.Dot(getMirant()) > 0)
		return true;
	else
		return false;
}

// --------------------------------------- PILOTEIG -----------------------------
Pilota *const JugadorBase::getPilota() const
{
	return getEquip()->getPilota();
}

// Per saber si el pot agafar la pilota
bool JugadorBase::isPilotaAbastPorter() const
{
	// s'ha de buscar la posició del porter ....
	return (VectorDistanciaSq(getPosicio(), getPilota()->getPosicio()) < POR_PilotaAbastPorter);
};

// Per saber si podem xutar o no
bool JugadorBase::isPilotaAbastDeXut() const
{
	return ((VectorDistanciaSq(getPosicio(), getPilota()->getPosicio()) < JUG_PilotaAbastXut) &&
			(getPosicio().h + Altura > getPilota()->getPosicio().h)); // -getPilota()->getAltura());
};

// Si la pilota està prou a prop nostre
bool JugadorBase::isPilotaAbastDeRecepcio() const
{
	return (VectorDistanciaSq(getPosicio(), getPilota()->getPosicio()) < JUG_DistanciaRecepcio);
};

// Si el jugador és el més proper a la pilota del nostre equip
bool JugadorBase::isMesProperAPilota() const
{
	return getEquip()->getMesProperAPilota() == this;
}

// Si el jugador és el més proper a la pilota del camp
bool JugadorBase::isElMesProperDelCampAPilota() const
{
	return isMesProperAPilota() &&
		   (DistanciaAPilota < getEquip()->getContraris()->getDistanciaAPilotaSq());
}

// Si el jugador té la pilota
bool JugadorBase::isControlantPilota() const
{
	return getEquip()->getJugadorAmbLaPilota() == this;
}

// --------------------------------------- ALTRES JUGADORS -----------------------------
// Ens pot servir per saber si estem en fora de joc, ja que sé els més propers a la porteria
// contraria (fins i tot més que els contraris)
bool JugadorBase::isElMesProperDelCampAPorteriaContraria() const
{
	return (DistanciaPorteriaContraria() < getEquip()->getContrariMesProperASevaPorteria()->DistanciaPorteriaNostra());
}

// Diu si el jugador està més endavant que el que controla la pilota
bool JugadorBase::isMesEndavantQueLAtacant() const
{
	return fabs(getPosicio().x - getEquip()->getPorteriaContraria()->getMig().x) <
		   fabs(getEquip()->getJugadorAmbLaPilota()->getPosicio().x - getEquip()->getPorteriaContraria()->getMig().x);
}

// Diu si hi ha un jugador a poca distància
bool JugadorBase::isEsticMarcat() const
{
	// He de comprovar amb tots els contraris
	std::vector<JugadorBase *>::const_iterator curOpp;
	curOpp = getEquip()->getContraris()->Membres().begin();

	for (curOpp; curOpp != getEquip()->getContraris()->Membres().end(); ++curOpp)
	{
		//calculem la distància al jugador. Si és menor que la distància de confort
		// és que està massa a prop
		if (isPosicioDavantNostra((*curOpp)->getPosicio()) &&
			(VectorDistanciaSq(getPosicio(), (*curOpp)->getPosicio()) < Jugador_ZonaComfort()))
		{
			return true;
		}
	} // next opp
	return false;
}

void JugadorBase::BuscaAjuda()
{
	// Si ningú ens ajuda, haurem de buscar-ne
	if (getEquip()->getJugadorDonantSuport() == NULL)
	{
		JugadorBase *BestSupportPly = getEquip()->BuscaMillorAtacantDeSuport();
		// Em trobo que a vegades no en troba cap (per passar quan només hi hagi un davanter)
		if (BestSupportPly != NULL)
		{
			getEquip()->setJugadorDonantSuport(BestSupportPly);

			Carter->DispatchMsg(SEND_MSG_IMMEDIATELY,
								getID(),
								getEquip()->getJugadorDonantSuport()->getID(),
								Msg_SupportAttacker,
								NULL);
		}
	}

	JugadorBase *BestSupportPly = getEquip()->BuscaMillorAtacantDeSuport();

	// Ha canviat el jugador que dóna millor suport, per tant hem de fer que
	// el d'abans torni cap a casa i l'actual es posi per feina
	if (BestSupportPly && (BestSupportPly != getEquip()->getJugadorDonantSuport()))
	{

		if (getEquip()->getJugadorDonantSuport())
		{
			Carter->DispatchMsg(SEND_MSG_IMMEDIATELY,
								getID(),
								getEquip()->getJugadorDonantSuport()->getID(),
								Msg_GoHome,
								NULL);
		}

		getEquip()->setJugadorDonantSuport(BestSupportPly);

		Carter->DispatchMsg(SEND_MSG_IMMEDIATELY,
							getID(),
							getEquip()->getJugadorDonantSuport()->getID(),
							Msg_SupportAttacker,
							NULL);
	}
}

// --------------------------------------- PORTERIES -----------------------------
// A quina distància estem de la porteria contraria
double JugadorBase::DistanciaPorteriaContraria() const
{
	return fabs(getPosicio().x - getEquip()->getPorteriaContraria()->getMig().x);
}

// A quina distància estem de la nostra porteria
double JugadorBase::DistanciaPorteriaNostra() const
{
	return fabs(getPosicio().x - getEquip()->getPorteriaNostra()->getMig().x);
}

// Si estem en la zona d'atac
bool JugadorBase::isInZonaCalenta() const
{
	return fabs(getPosicio().y - getEquip()->getPorteriaContraria()->getMig().y) < getCamp()->getZonaJoc().w / 3.0;
}

//------------------------------------------------------------------------
//
// predicats binaris per calcular std::sort (PucPassarEndavant / Enrere )
// No els estic fent servir.
//------------------------------------------------------------------------
bool SortByDistanciaPorteriaContraria(const JugadorBase *const p1,
									  const JugadorBase *const p2)
{
	return (p1->DistanciaPorteriaContraria() < p2->DistanciaPorteriaContraria());
}

bool SortByInversaDistanciaPorteriaContraria(const JugadorBase *const p1,
											 const JugadorBase *const p2)
{
	return (p1->DistanciaPorteriaContraria() > p2->DistanciaPorteriaContraria());
}
