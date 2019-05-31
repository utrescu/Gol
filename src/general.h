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
#ifndef GENERAL_H
#define GENERAL_H

#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <SDL/SDL_image.h>

#include <stdlib.h>
#include <limits>
#include "punt3.h"
#include <string>
#include "filepath.h"

// ------------------- ALEATORIETAT --------------------------------
inline double RandFloat() { return ((rand()) / (RAND_MAX + 1.0)); }
inline int RandInt(int x, int y) { return rand() % (y - x + 1) + x; }
inline double RandomClamped() { return RandFloat() - RandFloat(); }
inline double RandInRange(double x, double y)
{
    return x + RandFloat() * (y - x);
}

// -------------------- Comparacions numeriques ---------------------
template <class T, class U, class V>
inline void Clamp(T &arg, const U &minVal, const V &maxVal)
{
    if (arg < (T)minVal)
    {
        arg = (T)minVal;
    }

    if (arg > (T)maxVal)
    {
        arg = (T)maxVal;
    }
}

//returns true if the parameter is equal to zero
// inline bool IsZero(double val)
// {
//	return ( (-MINDOUBLE < val) && (val < MINDOUBLE) );
// }
/*
double min(double x, double y)
{
	if (x <= y)
	{
		return x;
	}
	return y;
}
*/

inline bool isEqual(float a, float b)
{
    if (fabs(a - b) < 1E-12)
    {
        return true;
    }

    return false;
}

inline bool isEqual(double a, double b)
{
    if (fabs(a - b) < 1E-12)
    {
        return true;
    }
    return false;
}

//returns true is the third parameter is in the range described by the
//first two
inline bool InRange(double start, double end, double val)
{
    if (start < end)
    {
        if ((val > start) && (val < end))
            return true;
        else
            return false;
    }

    else
    {
        if ((val < start) && (val > end))
            return true;
        else
            return false;
    }
}

// ----------- Definicions SDL  -----------------------------
inline bool IntersectRect(SDL_Rect *dest, const SDL_Rect *src1, const SDL_Rect *src2)
{
    int px0, py0, px1, py1;
    int cx0, cy0, cx1, cy1;
    int rx0, ry0, rx1, ry1;

    // fill in default (NULL) result rectangle

    dest->x = 0;
    dest->y = 0;
    dest->w = 0;
    dest->h = 0;

    // get coordinates of the rectangles

    px0 = src1->x;
    py0 = src1->y;
    px1 = src1->x + src1->w - 1;
    py1 = src1->y + src1->h - 1;

    cx0 = src2->x;
    cy0 = src2->y;
    cx1 = src2->x + src2->w - 1;
    cy1 = src2->y + src2->h - 1;

    // check if the rectangles intersect
    if (/*(cx0 < px0) && */ (cx1 < px0))
        return false;

    if ((cx0 > px1) /*&& (cx1 > px1)*/)
        return false;

    if (/*(cy0 < py0) && */ (cy1 < py0))
        return false;

    if ((cy0 > py1) /*&& (cy1 > py1)*/)
        return false;

    // intersect x
    if (cx0 <= px0)
        rx0 = px0;
    else
        rx0 = cx0;

    if (cx1 >= px1)
        rx1 = px1;
    else
        rx1 = cx1;

    // intersect y
    if (cy0 <= py0)
        ry0 = py0;
    else
        ry0 = cy0;

    if (cy1 >= py1)
        ry1 = py1;
    else
        ry1 = cy1;

    // fill in result rect

    dest->x = rx0;
    dest->y = ry0;
    dest->w = (rx1 - rx0) + 1;
    dest->h = (ry1 - ry0) + 1;

    return true;
};

inline SDL_Surface *EscalaImatge(const char *nom, double Escala)
{

    SDL_Surface *Imatge2 = NULL, *On = NULL;

    if (Escala != 1.0)
    {
        Imatge2 = IMG_Load(getFullFileName(nom).c_str());
        SDL_SetColorKey(Imatge2, SDL_SRCCOLORKEY, SDL_MapRGB(Imatge2->format, 255, 255, 255));
        // On = zoomSurface(Imatge2,Escala,Escala,0);
        On = zoomSurface(Imatge2, Escala, Escala, 1);
        SDL_FreeSurface(Imatge2);
    }
    else
    {
        On = IMG_Load(getFullFileName(nom).c_str());
    }
    SDL_SetColorKey(On, SDL_SRCCOLORKEY, SDL_MapRGB(On->format, 255, 255, 255));
    return On;
}

inline SDL_Surface *EscalaSuperficie(SDL_Surface *superficie, double Escala)
{
    return zoomSurface(superficie, Escala, Escala, 1);
}

inline SDL_Surface *FlipSurfaceH(SDL_Surface *bitmap)
{
    if (!bitmap)
        return NULL;
    SDL_Surface *temp = SDL_CreateRGBSurface(0, bitmap->w, bitmap->h,
                                             bitmap->format->BitsPerPixel, bitmap->format->Rmask, bitmap->format->Gmask,
                                             bitmap->format->Bmask, bitmap->format->Amask);
    if (!temp)
        return NULL;
    SDL_Rect src, dest;
    src.w = 1;
    src.h = bitmap->h;
    src.y = 0;
    dest.y = 0;
    int origw = bitmap->w;
    for (int i = 0; i < origw; i++)
    {
        src.x = i;
        dest.x = (origw - 1) - i;
        SDL_BlitSurface(bitmap, &src, temp, &dest);
    }
    return temp;
}

//------------------- EnforceNonPenetrationContraint ---------------------
//
//  Given a pointer to an entity and a std container of pointers to nearby
//  entities, this function checks to see if there is an overlap between
//  entities. If there is, then the entities are moved away from each
//  other
//------------------------------------------------------------------------
template <class T, class conT>
void EnforceNonPenetrationContraint(T entity, const conT &others)
{
    typename conT::const_iterator it;

    //iterate through all entities checking for any overlap of bounding
    //radii
    for (it = others.begin(); it != others.end(); ++it)
    {
        //make sure we don't check against this entity
        if (*it == entity)
            continue;

        //calculate the distance between the positions of the entities
        Punt3 ToEntity = entity->getPosicio() - (*it)->getPosicio();

        double DistFromEachOther = ToEntity.Llargada();

        //if this distance is smaller than the sum of their radii then this
        //entity must be moved away in the direction parallel to the
        //ToEntity vector
        double AmountOfOverLap = (*it)->getRadi() + entity->getRadi() -
                                 DistFromEachOther;

        if (AmountOfOverLap >= 0)
        {
            //move the entity a distance away equivalent to the amount of overlap.
            entity->setPosicio(entity->getPosicio() + (ToEntity / DistFromEachOther) * AmountOfOverLap);
        }
    } //next entity
}

#endif
