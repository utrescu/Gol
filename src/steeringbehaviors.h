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
#ifndef STEERINGBEHAVIORS_H
#define STEERINGBEHAVIORS_H

#define MINDETECTIONBOXLENGTH 60

/**
	@author Xavier Sala <utrescu@xaviersala.net>
*/
#include <vector>

#include "punt3.h"

class JugadorBase;
class Pilota;
class Pantalla;

class SteeringBehaviors{
private:
	JugadorBase* m_pPlayer;

	Pilota* m_pBall;

  //the steering force created by the combined effect of all
  //the selected behaviors
	Punt3 m_vSteeringForce;

  //the current target (usually the ball or predicted ball position)
	Punt3 m_vTarget;

  //the distance the player tries to interpose from the target
	double m_dInterposeDist;

  //multipliers. 
	double m_dMultSeparation;
	double m_dMultAvoid;

  //how far it can 'see'
	double m_dViewDistance;
	// Caixa del obstacle avoid
	double m_dDBoxLength;

  //binary flags to indicate whether or not a behavior should be active
	int m_iFlags;
	
	enum behavior_type
	{
		none               = 0x0000,
		seek               = 0x0001,
		arrive             = 0x0002,
		separation         = 0x0004,
		pursuit            = 0x0008,
		interpose          = 0x0010,
		obstacle_avoidance = 0x0020
	};
 
	//used by group behaviors to tag neighbours
	bool m_bTagged;	
	
	 //Arrive makes use of these to determine how quickly a vehicle
  	//should decelerate to its target
	enum Deceleration{slow = 3, normal = 2, fast = 1};
	
 	// Mou el jugador cap a una posició en concret
	Punt3 Seek(Punt3 target);

  	// El mateix que seek però arribant a velocitat zero al desti
	Punt3 Arrive(Punt3 target, Deceleration decel);

  	// Persegueix la pilota
	Punt3 Pursuit(const Pilota* ball);
 
	Punt3 Separation();

  	// Es posiciona entre la pilota i la posició	
	Punt3 Interpose(const Pilota* ball,
			Punt3 pos,
			double    DistFromTarget);

	// Evita els obstacles
	Punt3 ObstacleAvoidance(const std::vector<JugadorBase*>& obstacles);

  	// busca veins en el radi determinat
	void      FindNeighbours();	

	//this function tests if a specific bit of m_iFlags is set
	bool      On(behavior_type bt){return (m_iFlags & bt) == bt;}

	bool      AccumulateForce(Punt3 &sf, Punt3 ForceToAdd);

	Punt3  SumForces();

  	//a vertex buffer to contain the feelers rqd for dribbling
	std::vector<Punt3> m_Antenna;	
	
	double min(double a, double b)
	{
		if (a>b) return b;return a;
	}
	
public:
	SteeringBehaviors(JugadorBase* agent,
			  Pantalla*  world,
			  Pilota*   ball);

	virtual ~SteeringBehaviors(){};
	Punt3 Calculate();

  //calculates the component of the steering force that is parallel
  //with the vehicle heading
	double ForwardComponent();

  //calculates the component of the steering force that is perpendicuar
  //with the vehicle heading
	double SideComponent();

	Punt3 getForce()const{return m_vSteeringForce;}

	Punt3 getTarget()const{return m_vTarget;}
	void setTarget(const Punt3 t){m_vTarget = t;}

	double getInterposeDistance()const{return m_dInterposeDist;}
	void setInterposeDistance(double d){m_dInterposeDist = d;}

	bool isTagged()const{return m_bTagged;}
	void Tag(){m_bTagged = true;}
	void UnTag(){m_bTagged = false;}
  
	void SeekOn(){m_iFlags |= seek;}
	void ArriveOn(){m_iFlags |= arrive;}
	void PursuitOn(){m_iFlags |= pursuit;}
	void SeparationOn(){m_iFlags |= separation;}
	void InterposeOn(double d){m_iFlags |= interpose; m_dInterposeDist = d;}
	void AvoidObstacleOn() {m_iFlags |= obstacle_avoidance; }
  
	void SeekOff()  {if(On(seek))   m_iFlags ^=seek;}
	void ArriveOff(){if(On(arrive)) m_iFlags ^=arrive;}
	void PursuitOff(){if(On(pursuit)) m_iFlags ^=pursuit;}
	void SeparationOff(){if(On(separation)) m_iFlags ^=separation;}
	void InterposeOff(){if(On(interpose)) m_iFlags ^=interpose;}
	void AvoidObstacleOff() { if(On(obstacle_avoidance)) m_iFlags ^=obstacle_avoidance; }

	bool SeekIsOn(){return On(seek);}
	bool ArriveIsOn(){return On(arrive);}
	bool PursuitIsOn(){return On(pursuit);}
	bool SeparationIsOn(){return On(separation);}
	bool InterposeIsOn(){return On(interpose);}
	bool AvoidObstacleIsOn() { return On(obstacle_avoidance); }

};

#endif
