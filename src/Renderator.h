#ifndef RENDERATOR_H
#define RENDERATOR_H

#include <vector>
#include <algorithm>
#include "Entitat.h"

//  -------------------  RENDERATOR ------------------------
//	La idea d'aquesta classe és que serveixi per insertar-hi
//	les Entitats que es veuen per pantalla en aquest moment
//      perquè puguin ser pintades posteriorment des de Pantalla
//
//	Cada Jugador (i Porter) s'autoposarà a la llista només si 
//	és visible - presumiblement en la funció de moure Mou() -
//
//      - La Pilota i Les porteries també ho han de fer
//
//	La pantalla s'encarregarà de pintar-los tots, després de
//      fer la ordenació (que és el motiu d'aquesta classe)
//
//	La millor opció segurament seria un multiset o una 
//      priority_queue, que s'ordenen automàticament, però la 
//      documentació que he llegit diu que val més que ho faci 
//      amb un vector i sort... 
//	(no sé ... )

#define Renderitza Renderator::Instance()

bool SortPerZOrder(Entitat* a, Entitat* b);

class Renderator
{

private:

	std::vector<Entitat*> Elements;

	Renderator(){}

	//copy ctor and assignment should be private
	Renderator(const Renderator&);
	Renderator& operator=(const Renderator&);

public:

	static Renderator* Instance();

	// Afegir una entitat a la llista
	void Afegir(Entitat* e)
	{
		Elements.push_back(e);	  
	};

	// Ordenar-los per Z-Order
	void Ordenar()
	{
		std::sort(Elements.begin(), Elements.end(), SortPerZOrder);
	}
	// Buida la taula
	void Buida()
	{ 
		Elements.clear(); 
	}
	
	// Obtenir un punter a tots els elements
	const std::vector<Entitat*>& Membres()const{return Elements;}

};

#endif
