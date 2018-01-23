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
#ifndef MISSATGER_H
#define MISSATGER_H

/**
	@author Xavier Sala <utrescu@xaviersala.net>
*/

#include <set>
#include "Missatge.h"



class Entitat;

#define Carter Missatger::Instance()

const double SEND_MSG_IMMEDIATELY = 0.0;
const int    NO_ADDITIONAL_INFO   = 0;
const int    SENDER_ID_IRRELEVANT = -1;

class Missatger{
private:  
  
	// Pels missatges que no s'han d'entregar immediatament
	std::set<Missatge> PriorityQ;

	void Discharge(Entitat* pReceiver, const Missatge& msg);

	Missatger(){}

  //copy ctor and assignment should be private
	Missatger(const Missatger&);
	Missatger& operator=(const Missatger&);
public:

	~Missatger();
	// Pel sistema de'n Singleton 
	static Missatger* Instance();

  	//entrega el missatge a un altre agent 
	void DispatchMsg(double      delay,
			 int         sender,
			 int         receiver,
			 int         msg,
			 void*       ExtraInfo);

  	//Entrega els missatges retardats. S'ha de cridar a cada passada 
  	//serà efectiu
	void DispatchDelayedMessages();	

};

#endif
