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
#ifndef ENTITATMOBIL_H
#define ENTITATMOBIL_H

#include "Entitat.h"

/**
	@author Xavier Sala <utrescu@xaviersala.net>
*/
class EntitatMobil : public Entitat
{
public:

	
protected:
	Punt3 LlocAnterior;
	Punt3 Velocitat; // Ens diu cap on va l'objecte o on vol anar
	Punt3 MirantA; // Diu cap on està mirant
	Punt3 m_vSide; //Perpendicular a on mirem
	
	double Forca;
	double Massa;
	// double Altura;
	
	double maximaVelocitat;
	double maximaForca;
	double maximaRotacio;
public:
    EntitatMobil(double rad, Punt3 veloc, double maxveloc, Punt3 mirant, double massa, double maxgir, double maxforce);

    ~EntitatMobil();

    double getVelocitatMaxima()const{return maximaVelocitat;}                       
    void   setVelocitatMaxima(double speed){maximaVelocitat = speed;}

    double getForcaMaxima()const{return maximaForca;}
    void   setForcaMaxima(double mf){maximaForca = mf;}

    // bool      IsSpeedMaxedOut()const{return m_dMaxSpeed*m_dMaxSpeed >= m_vVelocity.LengthSq();}
    double getVelocitat()const{return Velocitat.Llargada();}
    Punt3  getVectorVelocitat() const { return Velocitat; }
    void   setVelocitat(Punt3 vel) { Velocitat = vel; }
    // double    SpeedSq()const{return m_vVelocity.LengthSq();}

    Punt3 getMirant()const{return MirantA;}
    void setMirant(Punt3 AraA);
    bool RodaCapA(Punt3 objectiu);
	void RodaCapASenseLimit(Punt3 objectiu);

    Punt3  getCostat() const { return m_vSide; }
    
    double getRotacioMaxima()const{return maximaRotacio;}
    void   setRotacioMaxima(double val){maximaRotacio = val;}

};

#endif
