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
#ifndef EQUIPESTATS_H
#define EQUIPESTATS_H

/**
	@author Xavier Sala <xavier@norimaki.xaviersala.net>
*/
#include "estat.h"
#include "Missatge.h"

class Equip;

//--------------------- ATACANT !! ------------------------------------------
class EquipEstatAtacant : public Estat<Equip>
{ 
private:
  
	EquipEstatAtacant(){}
public:

  //this is a singleton
	static EquipEstatAtacant* Instance();
	
	void Enter(Equip* team);
	
	void Execute(Equip* team);
	
	void Exit(Equip* team);
	
	bool OnMessage(Equip*, const Missatge&){return false;}
};

//--------------------- DEFENENT !! ------------------------------------------
class EquipEstatDefensant : public Estat<Equip>
{ 
	private:
  
		EquipEstatDefensant(){}
	public:
		//Singleton
		static EquipEstatDefensant* Instance();
	
		void Enter(Equip* team);
	
		void Execute(Equip* team);
	
		void Exit(Equip* team);
	
		bool OnMessage(Equip*, const Missatge&){return false;}
};
//------------------ TORNAR A CASA --------------------------
class EquipEstatTornaPosicioInicial : public Estat<Equip>
{ 
	private:
  
		EquipEstatTornaPosicioInicial(){}
	public:
		//Singleton
		static EquipEstatTornaPosicioInicial* Instance();
	
		void Enter(Equip* team);
	
		void Execute(Equip* team);
	
		void Exit(Equip* team);
	
		bool OnMessage(Equip*, const Missatge&){return false;}
};

#endif
