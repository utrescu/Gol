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
#ifndef JUGADORESTATS_H
#define JUGADORESTATS_H

/**
	@author Xavier Sala <utrescu@xaviersala.net>
*/
#include <string>

#include "estat.h"
#include "Missatge.h"

class Jugador;
class Pantalla;

//------------------------   ESTAT GLOBAL ------------------------------
// Es per poder capturar els missatges sempre, independentment de 
// l'estat en que ens trobem
// ---------------------------------------------------------------------
class JugadorEstatGlobal: public Estat<Jugador>
{
private:
  
	JugadorEstatGlobal(){}

public:

 	//this is a singleton
	static JugadorEstatGlobal* Instance();

	void Enter(Jugador* player);

	void Execute(Jugador* player);

	void Exit(Jugador* player);

	bool OnMessage(Jugador*, const Missatge&);
};

//--------------------   PERSEGUIR PILOTA ------------------------------
// El jugador es posa a perseguir la pilota directament
// 
// ---------------------------------------------------------------------
class JugadorEstatPerseguirPilota : public Estat<Jugador>
{
private:

	JugadorEstatPerseguirPilota(){}

public:

//this is a singleton
	static JugadorEstatPerseguirPilota* Instance();

	void Enter(Jugador* player);

	void Execute(Jugador* player);

	void Exit(Jugador* player);

	bool OnMessage(Jugador*, const Missatge&){return false;}
};

//---------------   DRIBBLAR EL CONTRARI  ------------------------------
// 
// 
// ---------------------------------------------------------------------
class JugadorEstatDribbla: public Estat<Jugador>
{
	private:
  
		JugadorEstatDribbla(){}

	public:

 	//this is a singleton
		static JugadorEstatDribbla* Instance();

		void Enter(Jugador* player);

		void Execute(Jugador* player);

		void Exit(Jugador* player);

		bool OnMessage(Jugador*, const Missatge&){return false;};
};

//---------------------   TORNAR A CASA  -------------------------------
// 
// 
// ---------------------------------------------------------------------
class JugadorEstatTornaACasa: public Estat<Jugador>
{
	private:
  
		JugadorEstatTornaACasa(){}

	public:

 	//this is a singleton
		static JugadorEstatTornaACasa* Instance();

		void Enter(Jugador* player);

		void Execute(Jugador* player);

		void Exit(Jugador* player);

		bool OnMessage(Jugador*, const Missatge&){return false;};
};

//-----------------------   ATURAR-SE  ---------------------------------
// 
// 
// ---------------------------------------------------------------------
class JugadorEstatEsperant: public Estat<Jugador>
{
	private:
  
		JugadorEstatEsperant(){}

	public:

 	//this is a singleton
		static JugadorEstatEsperant* Instance();

		void Enter(Jugador* player);

		void Execute(Jugador* player);

		void Exit(Jugador* player);

		bool OnMessage(Jugador*, const Missatge&){return false;};
};

//-----------------------------   XUTA ---------------------------------
// 
// 
// ---------------------------------------------------------------------
class JugadorEstatXuta: public Estat<Jugador>
{
	private:
  
		JugadorEstatXuta(){}

	public:

 	//this is a singleton
		static JugadorEstatXuta* Instance();

		void Enter(Jugador* player);

		void Execute(Jugador* player);

		void Exit(Jugador* player);

		bool OnMessage(Jugador*, const Missatge&){return false;};
};

//--------------------   RECEPTOR DE LA PASSADA ------------------------
// 
// 
// ---------------------------------------------------------------------
class JugadorEstatRebrePasse: public Estat<Jugador>
{
	private:
  
		JugadorEstatRebrePasse(){}

	public:

 	//this is a singleton
		static JugadorEstatRebrePasse* Instance();

		void Enter(Jugador* player);

		void Execute(Jugador* player);

		void Exit(Jugador* player);

		bool OnMessage(Jugador*, const Missatge&){return false;};
};

//--------------------   DONAR SUPORT A L'ATACANT ----------------------
// 
// 
// ---------------------------------------------------------------------
class JugadorEstatAjudaAtac: public Estat<Jugador>
{
	private:
  
		JugadorEstatAjudaAtac(){}

	public:

 	//this is a singleton
		static JugadorEstatAjudaAtac* Instance();

		void Enter(Jugador* player);

		void Execute(Jugador* player);

		void Exit(Jugador* player);

		bool OnMessage(Jugador*, const Missatge&){return false;};
};

//---------------------   SACADA DE BANDA  -----------------------------
// 
// 
// ---------------------------------------------------------------------
class JugadorEstatAnarASaque: public Estat<Jugador>
{
	private:
  
		JugadorEstatAnarASaque(){}

	public:

 	//this is a singleton
		static JugadorEstatAnarASaque* Instance();

		void Enter(Jugador* player);

		void Execute(Jugador* player);

		void Exit(Jugador* player);

		bool OnMessage(Jugador*, const Missatge&){return false;};
};

class JugadorEstatSacaDeBanda: public Estat<Jugador>
{
	private:
  
		JugadorEstatSacaDeBanda(){}

	public:

 	//this is a singleton
		static JugadorEstatSacaDeBanda* Instance();

		void Enter(Jugador* player);

		void Execute(Jugador* player);

		void Exit(Jugador* player);

		bool OnMessage(Jugador*, const Missatge&){return false;};
};


class JugadorEstatUsuari: public Estat<Jugador>
{
	private:
  
		JugadorEstatUsuari(){}

	public:

 	//this is a singleton
		static JugadorEstatUsuari* Instance();

		void Enter(Jugador* player);

		void Execute(Jugador* player);

		void Exit(Jugador* player);

		bool OnMessage(Jugador*, const Missatge&){return false;};
};




#endif
