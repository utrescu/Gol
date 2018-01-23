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
#ifndef ESTAT_H
#define ESTAT_H

/**
	@author Xavier Sala <utrescu@xaviersala.net>

 Plantilla d'estats genèrica. Està feta per poder implementar els diferents
estats que tenen els objectes del joc.

És una classe abstracta virtual, i per tant cada objecte ha d'implementar les diferents
funcions de l'estat.
 
He vist la forma de fer-ho a www.gamedev.net i m'ha semblat fantàstic
 
 */

struct Missatge;

template <class T>
class Estat
{
	public:

		virtual ~Estat(){}
		virtual void Enter(T*)=0;
		virtual void Execute(T*)=0;
		virtual void Exit(T*)=0;
		// Per rebre missatges que canviin l'estat ... 
		virtual bool OnMessage(T*, const Missatge&)=0;
};

#endif
