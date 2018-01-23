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
#ifndef AUTOLIST_H
#define AUTOLIST_H

#include <list>
/**
	@author Xavier Sala <utrescu@xaviersala.net>
	
	Els objectes del mateix tipus s'afegiran a la llista automàticament
	només derivant-los d'aquesta classe. Per tant es podrà accedir a 
	tots els membres
 */
 
template <class T>
class AutoList
{
	public:

		typedef std::list<T*> ObjectList;
  
	private:

		static ObjectList Membres;

	protected:

		AutoList()
		{
    			//cast this object to type T* and add it to the list
			Membres.push_back(static_cast<T*>(this));
		}

		~AutoList()
		{
			Membres.remove(static_cast<T*>(this));    
		}

	public:

		// get the number of items in the list.
		unsigned int getListSize()
		{
			return Membres.size();
		}
		
		static ObjectList& GetAllMembers(){return Membres;}

//  		 //! get the item at the index specified.
//		static T* getListItem(unsigned int n)
//		{
//			return List[n];
//		} 
//		static ObjectList& GetAllMembers(){return m_Members;}
		
};


template <class T>
std::list<T*> AutoList<T>::Membres;

#endif
