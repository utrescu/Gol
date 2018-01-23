#ifndef PUNT3_H
#define PUNT3_H
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
#include <math.h>
#include <limits>
#include "constants.h"
// #include "C2DMatrix.h"

enum {clockwise = 1, anticlockwise = -1};

template <typename T>
struct tagPunt3 {
	T x;
	T y;
	T h;

	tagPunt3<T>():x(0.0),y(0.0), h(0.0){};
	
	tagPunt3<T>(T a, T b, T c):x(a),y(b),h(c) {};
	
	inline void Zero() 
	{
		x=0.0;
		y=0.0;
		h=0.0;
	};
	
	bool isZero()const{return ((x*x + y*y) < MINDOUBLE);}
	
        //Llargada del vector x,y (evitar l'arrel quadrada)
	inline double LlargadaSq()const
	{
		return (x * x + y * y);
	};

	inline double Llargada()const
	{
		return sqrt(x * x + y * y);
	}
	
	inline void Reflect(const tagPunt3<T>& norm)
	{
		*this += 2.0 * this->Dot(norm) * norm.GetReverse();
	}
	
	inline tagPunt3<T> getReverse()const
	{
		return tagPunt3<T>(-this->x, -this->y, -this->h);
	}

	inline void Normalitza()
	{ 
		double vector_length = this->Llargada();

		if (vector_length > std::numeric_limits<double>::epsilon())
		{
			this->x /= vector_length;
			this->y /= vector_length;
		}
	}
	// Producte vectorial
	inline double Dot(const tagPunt3<T> &v2)const
	{
		return x*v2.x + y*v2.y;
	}
	// Distancia al vector
	inline double Distancia(const tagPunt3<T> &v2)const
	{
		double ySeparation = v2.y - y;
		double xSeparation = v2.x - x;

		return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
	}
	// Distancia al vector al quadrat
	inline double DistanciaSq(const tagPunt3<T> &v2)const
	{
		double ySeparation = v2.y - y;
		double xSeparation = v2.x - x;

		return ySeparation*ySeparation + xSeparation*xSeparation;
	}
	// Escurça el vector perque sigui menys que max
	inline void Truncate(double max)
	{
		if (this->Llargada() > max)
		{
			this->Normalitza();

			*this *= max;
		} 
	}
	// Perpendicular
	inline tagPunt3<T> Perpendicular()const
	{
		return tagPunt3<T>(-y, x,0.0);
	}
	//  returna positiu si el v2 estem en rotacio horaria del vector
	// o be negatiu si estem en posicio antihoraria (Y avall, x dreta)
	inline int Signe(const tagPunt3<T>& v2)const
	{
		if (y*v2.x > x*v2.y)
		{ 
			return anticlockwise;
		}
		else 
		{
			return clockwise;
		}
	}
	
	const tagPunt3<T>& operator+=(const tagPunt3<T> &rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	const tagPunt3<T>& operator-=(const tagPunt3<T> &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	const tagPunt3<T>& operator*=(const double& rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	const tagPunt3<T>& operator/=(const double& rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}

	bool operator==(const tagPunt3<T>& rhs)const
	{
		return (isEqual(x, rhs.x) && isEqual(y,rhs.y) );
	}

	bool operator!=(const tagPunt3<T>& rhs)const
	{
		return (x != rhs.x) || (y != rhs.y);
	}	
	
};

typedef tagPunt3<double> Punt3;

inline Punt3 operator*(const Punt3 &lhs, double rhs);
inline Punt3 operator*(double lhs, const Punt3 &rhs);
inline Punt3 operator-(const Punt3 &lhs, const Punt3 &rhs);
inline Punt3 operator+(const Punt3 &lhs, const Punt3 &rhs);
inline Punt3 operator/(const Punt3 &lhs, double val);


inline Punt3 VectorNormalitza(const Punt3 &v)
{
	Punt3 vec = v;

	double vector_length = vec.Llargada();

	if (vector_length > std::numeric_limits<double>::epsilon())
	{
		vec.x /= vector_length;
		vec.y /= vector_length;
	}

	return vec;
}

inline double VectorDot(const Punt3 &v1, const Punt3 &v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.h*v2.h;
}


inline double VectorDistancia(const Punt3 &v1, const Punt3 &v2)
{

	double ySeparation = v2.y - v1.y;
	double xSeparation = v2.x - v1.x;

	return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}

inline double VectorDistanciaSq(const Punt3 &v1, const Punt3 &v2)
{

	double ySeparation = v2.y - v1.y;
	double xSeparation = v2.x - v1.x;

	return ySeparation*ySeparation + xSeparation*xSeparation;
}

inline double VectorLlargada(const Punt3& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

inline double VectorLlargadaSq(const Punt3& v)
{
	return (v.x*v.x + v.y*v.y);
} 
		
inline Punt3 operator*(const Punt3 &lhs, double rhs)
{
	Punt3 result(lhs);
	result *= rhs;
	return result;
}

inline Punt3 operator*(double lhs, const Punt3 &rhs)
{
	Punt3 result(rhs);
	result *= lhs;
	return result;
}

//overload the - operator
inline Punt3 operator-(const Punt3 &lhs, const Punt3 &rhs)
{
	Punt3 result(lhs);
	result.x -= rhs.x;
	result.y -= rhs.y;
  
	return result;
}

//overload the + operator
inline Punt3 operator+(const Punt3 &lhs, const Punt3 &rhs)
{
	Punt3 result(lhs);
	result.x += rhs.x;
	result.y += rhs.y;
  
	return result;
}

//overload the / operator
inline Punt3 operator/(const Punt3 &lhs, double val)
{
	Punt3 result(lhs);
	result.x /= val;
	result.y /= val;

	return result;
}

//------------------------------------------------------------------------
//  Given a point P and a circle of radius R centered at C this function
//  determines the two points on the circle that intersect with the
//  tangents from P to the circle. Returns false if P is within the circle.
//
//  thanks to Dave Eberly for this one.
//------------------------------------------------------------------------
inline bool GetTangentPoints(Punt3 C, double R, Punt3 P, Punt3& T1, Punt3& T2)
{
	Punt3 PmC = P - C;
	double SqrLen = PmC.LlargadaSq();
	double RSqr = R*R;
	if ( SqrLen <= RSqr )
	{
      		// P is inside or on the circle
		return false;
	}
	double InvSqrLen = 1/SqrLen;
	double Root = sqrt(fabs(SqrLen - RSqr));

	T1.x = C.x + R*(R*PmC.x - PmC.y*Root)*InvSqrLen;
	T1.y = C.y + R*(R*PmC.y + PmC.x*Root)*InvSqrLen;
	T2.x = C.x + R*(R*PmC.x + PmC.y*Root)*InvSqrLen;
	T2.y = C.y + R*(R*PmC.y - PmC.x*Root)*InvSqrLen;

	return true;
}

inline Punt3 PointToLocalSpace2(Punt3 punt, Punt3 ix, Punt3 iy, Punt3 origen)
{
	Punt3 Resultat;
	
	double tempX =(ix.x*punt.x) + (ix.y*punt.y) - origen.Dot(ix);
	double tempY = (iy.x*punt.x) + (iy.y*punt.y) - origen.Dot(iy);

	Resultat.x = tempX;
	Resultat.y = tempY;
	Resultat.h=0.0;

	return Resultat;
}

//--------------------LineIntersection2D-------------------------
//
//	Given 2 lines in 2D space AB, CD this returns true if an 
//	intersection occurs.
//
//  No fa servir per res la coordenada d'altura. NOMES 2D!!!
//
//----------------------------------------------------------------- 

inline bool LineIntersection2D(Punt3 A, Punt3 B, Punt3 C, Punt3 D)
{
  double rTop = (A.y-C.y)*(D.x-C.x)-(A.x-C.x)*(D.y-C.y);
  double sTop = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);

	double Bot = (B.x-A.x)*(D.y-C.y)-(B.y-A.y)*(D.x-C.x);

  if (Bot == 0)//parallel
  {
    return false;
  }

  double invBot = 1.0/Bot;
  double r = rTop * invBot;
  double s = sTop * invBot;

  if( (r > 0) && (r < 1) && (s > 0) && (s < 1) )
  {
    //lines intersect
    return true;
  }

  //lines do not intersect
  return false;
}

//------------------ isSecondInFOVOfFirst -------------------------------------
//
//  returns true if the target position is in the field of view of the entity
//  positioned at posFirst facing in facingFirst
//-----------------------------------------------------------------------------
inline bool isSecondInFOVOfFirst(Punt3 posFirst,
                                 Punt3 facingFirst,
                                 Punt3 posSecond,
                                 double    fov)
{
  Punt3 toTarget = VectorNormalitza(posSecond - posFirst);

  return facingFirst.Dot(toTarget) >= cos(fov/2.0);
}



#endif
