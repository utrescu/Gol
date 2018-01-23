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
#ifndef PORTERESTATS_H
#define PORTERESTATS_H

/**
	@author Xavier Sala <utrescu@xaviersala.net>
 */
#include <string>
#include "Regulator.h" 

#include "estat.h"
#include "Missatge.h"

class Porter;
class Pantalla;

//------------------------   ESTAT GLOBAL ------------------------------
// És per poder capturar els missatges sempre, independentment de 
// l'estat en que ens trobem
// ---------------------------------------------------------------------
class PorterEstatGlobal: public Estat<Porter>
{
	private:
  
		PorterEstatGlobal(){}

	public:

 	//this is a singleton
		static PorterEstatGlobal* Instance();

		void Enter(Porter* player);

		void Execute(Porter* player);

		void Exit(Porter* player);

		bool OnMessage(Porter*, const Missatge&);
};
//---------------------   TORNAR A CASA  -------------------------------
// 
// 
// ---------------------------------------------------------------------
class PorterEstatTornaACasa: public Estat<Porter>
{
	private:
		PorterEstatTornaACasa(){}
	public:

 	//this is a singleton
		static PorterEstatTornaACasa* Instance();

		void Enter(Porter* player);

		void Execute(Porter* player);

		void Exit(Porter* player);

		bool OnMessage(Porter*, const Missatge&) { return false; };
};
//------------------  ITERCEPTA PILOTA -------------------------
class PorterEstatIntercepta: public Estat<Porter>
{
	private:
  
		PorterEstatIntercepta(){}

	public:

 	//this is a singleton
		static PorterEstatIntercepta* Instance();

		void Enter(Porter* player);

		void Execute(Porter* player);

		void Exit(Porter* player);

		bool OnMessage(Porter*, const Missatge&){ return false; };
};
//------------------  XUTA PILOTA -------------------------
class PorterEstatXuta: public Estat<Porter>
{
	private:
		PorterEstatXuta(){}
	public:
 		// Singleton
		static PorterEstatXuta* Instance();

		void Enter(Porter* player);

		void Execute(Porter* player);

		void Exit(Porter* player);

		bool OnMessage(Porter*, const Missatge&) { return false; };
};
//------------------  SEGUEIX PILOTA -------------------------
class PorterEstatCobrirPorteria: public Estat<Porter>
{
	private:
  
		PorterEstatCobrirPorteria(){}

	public:

 	//this is a singleton
		static PorterEstatCobrirPorteria* Instance();

		void Enter(Porter* player);

		void Execute(Porter* player);

		void Exit(Porter* player);

		bool OnMessage(Porter*, const Missatge&){ return false; };
};
//---------------------   ESTIRAR-SE -------------------------------
// 
// 
// ---------------------------------------------------------------------
class PorterEstatEstirat: public Estat<Porter>
{
	private:
		
		PorterEstatEstirat(){}
	public:

 	//this is a singleton
		static PorterEstatEstirat* Instance();

		void Enter(Porter* player);

		void Execute(Porter* player);

		void Exit(Porter* player);

		bool OnMessage(Porter*, const Missatge&) { return false; };
};

//---------------------------   CAIGUT ---------------------------------
// 
// 
// ---------------------------------------------------------------------
class PorterEstatCaigut: public Estat<Porter>
{
	private:
		Regulator *m_pRegulator;
		PorterEstatCaigut(){}
	public:

 	//this is a singleton
		static PorterEstatCaigut* Instance();

		void Enter(Porter* player);

		void Execute(Porter* player);

		void Exit(Porter* player);

		bool OnMessage(Porter*, const Missatge&) { return false; };
};

#endif
