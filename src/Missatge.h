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
#ifndef MISSATGE_H
#define MISSATGE_H

#include <iostream>
#include <math.h>

/**
	@author Xavier Sala <xavier@norimaki.xaviersala.net>
*/

struct Missatge
{
	int          Emissor;
	int          Receptor;
  //the message itself. These are all enumerated in the file
  //"MessageTypes.h"
	int          Msg;
	double       DispatchTime;
	void*        ExtraInfo;


	Missatge():DispatchTime(-1),
	Emissor(-1),
	Receptor(-1),
	Msg(-1)
	{}

	Missatge(double time,
		 int    sender,
		 int    receiver,
		 int    msg,
		 void*  info = NULL): DispatchTime(time),
	Emissor(sender),
	Receptor(receiver),
	Msg(msg),
	ExtraInfo(info)
	{}
 
};

//these telegrams will be stored in a priority queue. Therefore the >
//operator needs to be overloaded so that the PQ can sort the telegrams
//by time priority. Note how the times must be smaller than
//SmallestDelay apart before two Telegrams are considered unique.
const double SmallestDelay = 0.25;


inline bool operator==(const Missatge& t1, const Missatge& t2)
{
	return ( fabs(t1.DispatchTime-t2.DispatchTime) < SmallestDelay) &&
			(t1.Emissor == t2.Emissor)        &&
			(t1.Receptor == t2.Receptor)    &&
			(t1.Msg == t2.Msg);
}

inline bool operator<(const Missatge& t1, const Missatge& t2)
{
	if (t1 == t2)
	{
		return false;
	}

	else
	{
		return  (t1.DispatchTime < t2.DispatchTime);
	}
}

inline std::ostream& operator<<(std::ostream& os, const Missatge& t)
{
	os << "time: " << t.DispatchTime << "  Emissor: " << t.Emissor
			<< "   Receptor: " << t.Receptor << "   Msg: " << t.Msg;

	return os;
}

//handy helper function for dereferencing the ExtraInfo field of the Telegram 
//to the required type.
template <class T>
inline T DereferenceToType(void* p)
{
	return *(T*)(p);
}

#endif
