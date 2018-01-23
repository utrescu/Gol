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
#include "steeringbehaviors.h"
#include "C2DMatrix.h"
#include "Jugadorbase.h"
#include "Pilota.h"
#include "Autolist.h"

#include <list>

using std::string;
using std::vector;

#define SEPARATIONCOEFICIENT 10

//------------------------- ctor -----------------------------------------
//
//------------------------------------------------------------------------
SteeringBehaviors::SteeringBehaviors(JugadorBase* agent, Pantalla* world, Pilota* ball): 
			m_pPlayer(agent),
            m_iFlags(0), 
			m_bTagged(false), 
			m_pBall(ball), 
			m_dInterposeDist(0.0), 
			m_Antenna(5,Punt3())
{
	m_dViewDistance = 100;
	m_dMultSeparation = 10;
	m_dMultAvoid=100;
	// Per poder-lo incrementar/reduir a plaer
}

//--------------------- AccumulateForce ----------------------------------
//
//  This function calculates how much of its max steering force the 
//  vehicle has left to apply and then applies that amount of the
//  force to add.
//------------------------------------------------------------------------
bool SteeringBehaviors::AccumulateForce(Punt3 &sf, Punt3 ForceToAdd)
{
  //first calculate how much steering force we have left to use
	double MagnitudeSoFar = sf.Llargada();

	double magnitudeRemaining = m_pPlayer->getForcaMaxima() - MagnitudeSoFar;

  //return false if there is no more force left to use
	if (magnitudeRemaining <= 0.0) return false;

  //calculate the magnitude of the force we want to add
	double MagnitudeToAdd = ForceToAdd.Llargada();
  
  //now calculate how much of the force we can really add  
	if (MagnitudeToAdd > magnitudeRemaining)
	{
		MagnitudeToAdd = magnitudeRemaining;
	}

  //add it to the steering force
	sf += (VectorNormalitza(ForceToAdd) * MagnitudeToAdd); 
  
	return true;
}

//---------------------- Calculate ---------------------------------------
//
//  calculates the overall steering force based on the currently active
//  steering behaviors. 
//------------------------------------------------------------------------
Punt3 SteeringBehaviors::Calculate()
{                                                                         
  	//reset the force
	m_vSteeringForce.Zero();

  	//this will hold the value of each individual steering force
	m_vSteeringForce = SumForces();

  	//make sure the force doesn't exceed the vehicles maximum allowable
	
	m_vSteeringForce.Truncate(m_pPlayer->getForcaMaxima());
	// printf("..... final (%f,%f,%f)\n",m_vSteeringForce.x, m_vSteeringForce.y, m_vSteeringForce.h);
	return m_vSteeringForce;
}

//-------------------------- SumForces -----------------------------------
//
//  this method calls each active steering behavior and acumulates their
//  forces until the max steering force magnitude is reached at which
//  time the function returns the steering force accumulated to that 
//  point
//------------------------------------------------------------------------
Punt3 SteeringBehaviors::SumForces()
{
	Punt3 force;
  
	//S'han de tenir en compte els veins
	FindNeighbours();

	if (On(separation))
	{
		force += Separation() * m_dMultSeparation;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}    

	if (On(seek))
	{
		force += Seek(m_vTarget);
		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(arrive))
	{
		force += Arrive(m_vTarget, fast);

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(pursuit))
	{
		force += Pursuit(m_pBall);

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(interpose))
	{
		force += Interpose(m_pBall, m_vTarget, m_dInterposeDist);

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(obstacle_avoidance))
	{
		force += ObstacleAvoidance(m_pPlayer->getEquip()->getContraris()->Membres()); //  * m_dMultAvoid;
		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
//				m_dWeightObstacleAvoidance;
	}

	return m_vSteeringForce;
}

//------------------------- ForwardComponent -----------------------------
//
//  calculates the forward component of the steering force
//------------------------------------------------------------------------
double SteeringBehaviors::ForwardComponent()
{
	return m_pPlayer->getMirant().Dot(m_vSteeringForce);
}

//--------------------------- SideComponent ------------------------------
//
//  //  calculates the side component of the steering force
//------------------------------------------------------------------------
double SteeringBehaviors::SideComponent()
{
	return m_pPlayer->getCostat().Dot(m_vSteeringForce) * m_pPlayer->getRotacioMaxima();
}


//------------------------------- Seek -----------------------------------
//
//  Given a target, this behavior returns a steering force which will
//  allign the agent with the target and move the agent in the desired
//  direction
//------------------------------------------------------------------------
Punt3 SteeringBehaviors::Seek(Punt3 target)
{
	
 
	Punt3 DesiredVelocity = VectorNormalitza(target - m_pPlayer->getPosicio())
			* m_pPlayer->getVelocitatMaxima();
	return (DesiredVelocity - m_pPlayer->getVectorVelocitat());
}


//--------------------------- Arrive -------------------------------------
//
//  This behavior is similar to seek but it attempts to arrive at the
//  target with a zero velocity
//------------------------------------------------------------------------
Punt3 SteeringBehaviors::Arrive(Punt3    target,
				Deceleration deceleration)
{
	Punt3 ToTarget = target - m_pPlayer->getPosicio();

  //calculate the distance to the target
	double dist = ToTarget.Llargada();

	if (dist > 0)
	{
    //because Deceleration is enumerated as an int, this value is required
    //to provide fine tweaking of the deceleration..
		const double DecelerationTweaker = 0.3;

    //calculate the speed required to reach the target given the desired
    //deceleration
		double speed =  dist / ((double)deceleration * DecelerationTweaker);                    

    //make sure the velocity does not exceed the max
		speed = min(speed, m_pPlayer->getVelocitatMaxima());

    //from here proceed just like Seek except we don't need to normalize 
    //the ToTarget vector because we have already gone to the trouble
    //of calculating its length: dist. 
		Punt3 DesiredVelocity =  ToTarget * speed / dist;

		return (DesiredVelocity - m_pPlayer->getVectorVelocitat());
	}

	return Punt3(0,0,0);
}


//------------------------------ Pursuit ---------------------------------
//
//  this behavior creates a force that steers the agent towards the 
//  ball
//------------------------------------------------------------------------
Punt3 SteeringBehaviors::Pursuit(const Pilota* ball)
{
	Punt3 ToBall = ball->getPosicio() - m_pPlayer->getPosicio();
 
  	//the lookahead time is proportional to the distance between the ball
 	//and the pursuer; 
	double LookAheadTime = 0.0;

	if (ball->getVelocitat() != 0.0)
	{
		LookAheadTime = ToBall.Llargada() / ball->getVelocitat();
	}

  	//calculate where the ball will be at this time in the future
	m_vTarget = ball->getPosicioFutura(LookAheadTime);

  	//now seek to the predicted future position of the ball
	return Arrive(m_vTarget, fast);
}


//-------------------------- FindNeighbours ------------------------------
//
//  tags any vehicles within a predefined radius
//------------------------------------------------------------------------
void SteeringBehaviors::FindNeighbours()
{
	std::list<JugadorBase*>& AllPlayers = AutoList<JugadorBase>::GetAllMembers();
	std::list<JugadorBase*>::iterator curPlyr;
	for (curPlyr = AllPlayers.begin(); curPlyr!=AllPlayers.end(); ++curPlyr)
	{
		//first clear any current tag
		(*curPlyr)->getComportament()->UnTag();

		//work in distance squared to avoid sqrts
		Punt3 to = (*curPlyr)->getPosicio() - m_pPlayer->getPosicio();

		if (to.LlargadaSq() < (m_dViewDistance * m_dViewDistance))
		{
			(*curPlyr)->getComportament()->Tag();
		}
	}//next
}


//---------------------------- Separation --------------------------------
//
// this calculates a force repelling from the other neighbors
//------------------------------------------------------------------------
Punt3 SteeringBehaviors::Separation()
{  
   //iterate through all the neighbors and calculate the vector from the
	Punt3 SteeringForce;
  
	std::list<JugadorBase*>& AllPlayers = AutoList<JugadorBase>::GetAllMembers();
	std::list<JugadorBase*>::iterator curPlyr;
	for (curPlyr = AllPlayers.begin(); curPlyr!=AllPlayers.end(); ++curPlyr)
	{
    //make sure this agent isn't included in the calculations and that
    //the agent is close enough
		if((*curPlyr != m_pPlayer) && (*curPlyr)->getComportament()->isTagged())
		{
			Punt3 ToAgent = m_pPlayer->getPosicio() - (*curPlyr)->getPosicio();

      //scale the force inversely proportional to the agents distance  
      //from its neighbor.
			SteeringForce += VectorNormalitza(ToAgent)/ToAgent.Llargada();
		}
	}

	return SteeringForce;
}
 
//--------------------------- Interpose ----------------------------------
//
//  Given an opponent and an object position this method returns a 
//  force that attempts to position the agent between them
//------------------------------------------------------------------------
Punt3 SteeringBehaviors::Interpose(const Pilota* ball,
				   Punt3  target,
				   double DistFromTarget)
{
	return Arrive(target + VectorNormalitza(ball->getPosicio() - target) * 
		      DistFromTarget, normal);
}

//---------------------- ObstacleAvoidance -------------------------------
//
//  Given a vector of CObstacles, this method returns a steering force
//  that will prevent the agent colliding with the closest obstacle
//------------------------------------------------------------------------
Punt3 SteeringBehaviors::ObstacleAvoidance(const std::vector<JugadorBase*>& obstacles)
{
  //the detection box length is proportional to the agent's velocity
  m_dDBoxLength = MINDETECTIONBOXLENGTH + 
                  (m_pPlayer->getVelocitat()/m_pPlayer->getVelocitatMaxima()) *
                  MINDETECTIONBOXLENGTH;

  //tag all obstacles within range of the box for processing
  // m_pVehicle->World()->TagObstaclesWithinViewRange(m_pVehicle, m_dDBoxLength);

  FindNeighbours(); // m_pPlayer, m_dDBoxLength);

  //this will keep track of the closest intersecting obstacle (CIB)
  JugadorBase* ClosestIntersectingObstacle = NULL;
 
  //this will be used to track the distance to the CIB
  double DistToClosestIP = MAXDOUBLE;

  //this will record the transformed local coordinates of the CIB
  Punt3 LocalPosOfClosestObstacle;

  std::vector<JugadorBase*>::const_iterator curOb = obstacles.begin();

  while(curOb != obstacles.end())
  {
    //if the obstacle has been tagged within range proceed
    if ((*curOb)->getComportament()->isTagged())
    {
      //calculate this obstacle's position in local space
      Punt3 LocalPos = PointToLocalSpace((*curOb)->getPosicio(),
                                          m_pPlayer->getMirant(),
                                          m_pPlayer->getCostat(),
                                          m_pPlayer->getPosicio());

      //if the local position has a negative x value then it must lay
      //behind the agent. (in which case it can be ignored)
      if (LocalPos.x >= 0)
      {
        //if the distance from the x axis to the object's position is less
        //than its radius + half the width of the detection box then there
        //is a potential intersection.
        double ExpandedRadius = (*curOb)->getRadi() + m_pPlayer->getRadi();

        if (fabs(LocalPos.y) < ExpandedRadius)
        {
          //now to do a line/circle intersection test. The center of the 
          //circle is represented by (cX, cY). The intersection points are 
          //given by the formula x = cX +/-sqrt(r^2-cY^2) for y=0. 
          //We only need to look at the smallest positive value of x because
          //that will be the closest point of intersection.
          double cX = LocalPos.x;
          double cY = LocalPos.y;
          
          //we only need to calculate the sqrt part of the above equation once
          double SqrtPart = sqrt(ExpandedRadius*ExpandedRadius - cY*cY);

          double ip = cX - SqrtPart;

          if (ip <= 0.0)
          {
            ip = cX + SqrtPart;
          }

          //test to see if this is the closest so far. If it is keep a
          //record of the obstacle and its local coordinates
          if (ip < DistToClosestIP)
          {
            DistToClosestIP = ip;

            ClosestIntersectingObstacle = *curOb;

            LocalPosOfClosestObstacle = LocalPos;
          }         
        }
      }
    }

    ++curOb;
  }

  //if we have found an intersecting obstacle, calculate a steering 
  //force away from it
  Punt3 SteeringForce;

  if (ClosestIntersectingObstacle)
  {
    //the closer the agent is to an object, the stronger the 
    //steering force should be
    double multiplier = 1.0 + (m_dDBoxLength - LocalPosOfClosestObstacle.x) / m_dDBoxLength;

    //calculate the lateral force
    SteeringForce.y = (ClosestIntersectingObstacle->getRadi()- LocalPosOfClosestObstacle.y)* multiplier;   

    //apply a braking force proportional to the obstacles distance from
    //the vehicle. 
    const double BrakingWeight = 0.2;

    SteeringForce.x = (ClosestIntersectingObstacle->getRadi() - LocalPosOfClosestObstacle.x) * BrakingWeight;
  }

  //finally, convert the steering vector from local to world space
  return VectorToWorldSpace(SteeringForce, m_pPlayer->getMirant(), m_pPlayer->getCostat());
}
