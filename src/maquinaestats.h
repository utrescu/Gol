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
#ifndef MAQUINAESTATS_H
#define MAQUINAESTATS_H

#include <cassert>
#include <string>

#include "estat.h"
#include "Missatge.h"
#if defined(__GNUC__) //&& (__GNUC__ > ?)	
#include <cxxabi.h>
#endif

template <class T>
class MaquinaEstats
{
	private:

  	// punter al propietari de la instancia
	T*          Propietari;

	// Quin es l'estat que te la nostra maquina
	Estat<T>*   EstatActual;
  
  	// Recordem l'estat que teniem abans, a vegades fa falta
	Estat<T>*   EstatAnterior;

  	// Es crida cada vegada que s'actualitza la FSM
	Estat<T>*   EstatGlobal;
  

	public:

	MaquinaEstats(T* owner):Propietari(owner),
		EstatActual(NULL),
		EstatAnterior(NULL),
		EstatGlobal(NULL)
	{}

	virtual ~MaquinaEstats(){}

  	// Posar valors a la maquina
	void setEstatActual(Estat<T>* s){EstatActual = s;}
	void setEstatGlobal (Estat<T>* s) {EstatGlobal = s;}
	void setEstatAnterior (Estat<T>* s){EstatAnterior = s;}
  
  	// Actualitzar la maquina d'estats
	void  Update()const
	{
    		// si hi ha un estat global, es crida l'execute 
		if(EstatGlobal)   EstatGlobal->Execute(Propietari);
		// si hi ha estat actual, es crida a Execute 
		if (EstatActual) EstatActual->Execute(Propietari);
	}

		
	bool  capturaMissatge(const Missatge& msg)const
	{
    		// L'estat actual es valid i pot agafar el missatge 
		if (EstatActual && EstatActual->OnMessage(Propietari, msg))
		{
			return true;
		}
  
		// si no l'han agafat, s'envia el missatge a l'estat global 
		if (EstatGlobal && EstatGlobal->OnMessage(Propietari, msg))
		{
			return true;
		}

		return false;
	}

	//Canvia d'un estat al segent...
	void  canviaEstat(Estat<T>* pNewEstat)
	{
		assert(pNewEstat && "<MaquinaEstats::ChangeState>:trying to assign null state to current");
	
		//guardem l'estat que teniem abans 
		EstatAnterior = EstatActual;
		//Cridem a sortida de l'estat actual 
		EstatActual->Exit(Propietari);
		//Canviem al nou estat 
		EstatActual = pNewEstat;
		// Entrem al mètode d'entrada de l'estat 
		EstatActual->Enter(Propietari);
	}

  	//torna a l'estat anterior (per aixo l'hem guardat)
	void  setEstatAnterior()
	{
		CanviaEstat(EstatAnterior);
	}

  	// mira si l'estat actual és el que ens han passat
	bool  isEnEstat(const Estat<T>& st)const
	{
		if (typeid(*EstatActual) == typeid(st)) return true;
		return false;
	}

	Estat<T>*  getEstatActual()  const{return EstatActual;}
	Estat<T>*  getEstatGlobal()  const{return EstatGlobal;}
	Estat<T>*  getEstatAnterior() const{return EstatAnterior;}

  	// Per aquesta funció s'ha hagut de fer un xanxullo perquè el GNU C++ donava
	// una cosa i el Visual C++ una altra
	//
	// GNU C++ --> 03Classe
	// Visual C++ --> class Classe
	//

	std::string getNomEstatActual() const
	{
		std::string s(typeid(*EstatActual).name());

#if defined(__GNUC__) //&& (__GNUC__ > ?)		
		std::size_t n;
		int r;
		char*const p (abi::__cxa_demangle(s.c_str(),0,&n,&r));

		if (0 != r) ; //handle failure

		s = p;
		std::free(p);
#else
		// Visual C++ s'ha d'eliminar la part que té el text 'class'
		if (s.size() > 5)
		{
			s.erase(0, 6);
		}
#endif
		return s;
	}

};

#endif
