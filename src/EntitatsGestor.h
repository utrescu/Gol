#ifndef ENTITATSGESTOR_H
#define ENTITATSGESTOR_H

/**

	@author Xavier Sala <xavier@norimaki.xaviersala.net>
	
	Modified from EntityManager of Mat Buckland (fup@ai-junkie.com)
*/

#include <map>

class Entitat;

#define GestorEntitats EntitatsGestor::Instance()

class EntitatsGestor{	
private:
	typedef std::map<int, Entitat*> MapaEntitats;
	
	MapaEntitats m_EntityMap;

	EntitatsGestor(){}

  //copy ctor and assignment should be private
	EntitatsGestor(const EntitatsGestor&);
	EntitatsGestor& operator=(const EntitatsGestor&);


public:
	static EntitatsGestor* Instance();
	// Afegir una entitat al mapa 
	void RegisterEntity(Entitat* NewEntity);
	// Obtenir el punter a la entitat que tingui el ID especificat 
	Entitat* GetEntityFromID(int id)const;
	// Eliminar la entitat del mapa 
	void RemoveEntity(Entitat* pEntity);
	// Buidar el mapa d'entitats  
	void Reset(){m_EntityMap.clear();}
};

#endif
