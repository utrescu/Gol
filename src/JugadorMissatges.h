#ifndef JUGADOR_MISSATGES_H
#define JUGADOR_MISSATGES_H
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
#include <string>

enum MessageType
{
	Msg_ReceiveBall,
	Msg_PassToMe,
	Msg_SupportAttacker,
	Msg_Gol,
	Msg_GoHome,
	Msg_AnarASaque,
	Msg_Wait
};

//converts an enumerated value to a string
inline std::string MessageToString(int msg)
{
	switch (msg)
	{
		case Msg_ReceiveBall: 
			return "Msg_ReceiveBall";
			break;

		case Msg_PassToMe:
			return "Msg_PassToMe";
			break;

		case Msg_SupportAttacker:
			return "Msg_SupportAttacker";
			break;

		case Msg_GoHome:
			return "Msg_GoHome";
			break;	
		case Msg_Gol:
			return "Msg_Gol";
			break;

		case Msg_Wait:
			return "Msg_Wait";
			break;

		case Msg_AnarASaque:
			return "Msg_AnarASaque";
			break;

		default:
			return "INVALID MESSAGE!!";
			break;
	}
}
#endif
