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
#include "missatger.h"
#include "Entitat.h"
#include "EntitatsGestor.h"
#include "Missatge.h"
#include "FrameCounter.h"

Missatger::~Missatger()
{
}

void Missatger::Discharge(Entitat* Receptor, const Missatge& msg)
{
	Receptor->RebreMissatge(msg);
	/*	
	if (!pReceiver->HandleMessage(msg))
	{
    //telegram could not be handled
#ifdef SHOW_MESSAGING_INFO
    debug_con << "Message not handled" << "";
#endif
	}
	*/
}
Missatger* Missatger::Instance()
{
	static Missatger instance; 
  
	return &instance;
}

//---------------------------- DispatchMsg ---------------------------
//
//  given a message, a receiver, a sender and any time delay, this function
//  routes the message to the correct agent (if no delay) or stores
//  in the message queue to be dispatched at the correct time
//------------------------------------------------------------------------
void Missatger::DispatchMsg(double       delay,
			    int          sender,
			    int          receiver,
			    int          msg,
			    void*        AdditionalInfo = NULL)
{

  //get a pointer to the receiver
	Entitat* pReceiver = GestorEntitats->GetEntityFromID(receiver);

  //make sure the receiver is valid
	if (pReceiver != NULL)
	{
	  //create the telegram
		Missatge mis(0,sender, receiver, msg, AdditionalInfo);
  
  		//if there is no delay, route telegram immediately                       
		if (delay <= 0.0)                                                        
		{
		    //send the telegram to the recipient
    		    Discharge(pReceiver, mis);
		}
		//else calculate the time when the telegram should be dispatched
		else
		{
			double CurrentTime = TickCounter->GetCurrentFrame(); 
			mis.DispatchTime = CurrentTime + delay;

			//and put it in the queue
			PriorityQ.insert(mis);
		}
	}
}

//---------------------- DispatchDelayedMessages -------------------------
//
//  This function dispatches any telegrams with a timestamp that has
//  expired. Any dispatched telegrams are removed from the queue
//------------------------------------------------------------------------
void Missatger::DispatchDelayedMessages()
{ 
  	//first get current time
	double CurrentTime = TickCounter->GetCurrentFrame(); 

  	//now peek at the queue to see if any telegrams need dispatching.
  	//remove all telegrams from the front of the queue that have gone
  	//past their sell by date
	while( !PriorityQ.empty() &&
		       (PriorityQ.begin()->DispatchTime < CurrentTime) && 
		       (PriorityQ.begin()->DispatchTime > 0) )
	{
    		//read the telegram from the front of the queue
		const Missatge& telegram = *PriorityQ.begin();
    		//find the recipient
		Entitat* pReceiver = GestorEntitats->GetEntityFromID(telegram.Receptor);
    		//send the telegram to the recipient
    		Discharge(pReceiver, telegram);
		//remove it from the queue
    		PriorityQ.erase(PriorityQ.begin());
	}
}

