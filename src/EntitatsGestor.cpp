#include "EntitatsGestor.h"
#include "Entitat.h"

//---------------------------   Instance   ------------------------------------
//-----------------------------------------------------------------------------
EntitatsGestor* EntitatsGestor::Instance()
{
	static EntitatsGestor instance;

	return &instance;
}
//------------------------- GetEntityFromID -----------------------------------
//-----------------------------------------------------------------------------
Entitat* EntitatsGestor::GetEntityFromID(int id)const
{
  //find the entity
	MapaEntitats::const_iterator ent = m_EntityMap.find(id);

	if (ent != m_EntityMap.end())
	{
		return ent->second;
	}
	
	return NULL;
}

//--------------------------- RemoveEntity ------------------------------------
//-----------------------------------------------------------------------------
void EntitatsGestor::RemoveEntity(Entitat* pEntity)
{    
	m_EntityMap.erase(m_EntityMap.find(pEntity->getID()));
} 

//---------------------------- RegisterEntity ---------------------------------
//-----------------------------------------------------------------------------
void EntitatsGestor::RegisterEntity(Entitat* NewEntity)
{
	m_EntityMap.insert(std::make_pair(NewEntity->getID(), NewEntity));
}
