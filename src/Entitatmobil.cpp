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
#include <assert.h>
#include "Entitatmobil.h"
#include "punt3.h"
#include "general.h"
#include "C2DMatrix.h"

EntitatMobil::EntitatMobil(double rad, Punt3 veloc, double maxveloc, Punt3 mirant, double massa, double maxgir, double maxforce)
 : Entitat(Entitat::getSeguentID())
{
//	Lloc = Pos;
	RadiCaptacio = rad;
	Velocitat = veloc;
	maximaVelocitat = maxveloc;
	maximaRotacio = maxgir;
	maximaForca = maxforce;
	Forca=0;
	MirantA = mirant;
	Massa = massa;
	m_vSide = MirantA.Perpendicular();
}


EntitatMobil::~EntitatMobil()
{
}

void EntitatMobil::setMirant(Punt3 AraA)
{
	assert( (AraA.LlargadaSq() - 1.0) < 0.00001);
	Velocitat = AraA;
  //the side vector must always be perpendicular to the heading
	m_vSide = MirantA.Perpendicular();
	// m_vSide = Velocitat.Perpendicular();
}

bool EntitatMobil::RodaCapA(Punt3 objectiu)
{
	// Primer calcular cap a on m'haig de moure i quin angle 
	Punt3 aObjectiu = VectorNormalitza(objectiu - Lloc);
	
	double dot = MirantA.Dot(aObjectiu);
	
	Clamp(dot, -1, 1);
	
	double angle = acos(dot);
	
	if (angle < 0.00001) return true;
	
	// Si ens hem de girar mes del permes, angle = maxim 
	if (angle > maximaRotacio) angle = maximaRotacio;
	// recalcular de nou el lloc cap a on mirem ...
	// angle = angle * MirantA.Signe(aObjectiu);
	
	//The next few lines use a rotation matrix to rotate the player's heading
  	//vector accordingly
	C2DMatrix RotationMatrix;
	RotationMatrix.Rotate(angle * MirantA.Signe(aObjectiu));
	RotationMatrix.TransformPunt3s(MirantA);
	RotationMatrix.TransformPunt3s(Velocitat);
	
	//finally recreate m_vSide
	m_vSide = MirantA.Perpendicular();

	return false;	
}

void EntitatMobil::RodaCapASenseLimit(Punt3 objectiu)
{
	// Primer calcular cap a on m'haig de moure i quin angle 
	Punt3 aObjectiu = VectorNormalitza(objectiu - Lloc);
	
	double dot = MirantA.Dot(aObjectiu);
	
	Clamp(dot, -1, 1);
	
	double angle = acos(dot);
	
	if (angle < 0.00001) return;
	
	C2DMatrix RotationMatrix;
	RotationMatrix.Rotate(angle * MirantA.Signe(aObjectiu));
	RotationMatrix.TransformPunt3s(MirantA);
	RotationMatrix.TransformPunt3s(Velocitat);
	
	m_vSide = MirantA.Perpendicular();
}