#ifndef REGULATOR_H
#define REGULATOR_H
//------------------------------------------------------------------------
//
//  Name:   Regulator.h
//
//  Desc:   Use this class to regulate code flow (for an update function say)
//          Instantiate the class with the frequency you would like your code
//          section to flow (like 10 times per second) and then only allow
//          the program flow to continue if Ready() returns true
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
// #pragma comment(lib,"winmm.lib") //if you don't use MSVC make sure this library is included in your project
// #include "mmsystem.h"

#include "general.h"
#include <SDL/SDL.h>

class Regulator
{
private:
	//the time period between updates
	double m_dUpdatePeriod;

	//the next time the regulator allows code flow
	Uint32 m_dwNextUpdateTime;

public:
	Regulator(double NumUpdatesPerSecondRqd)
	{
		m_dwNextUpdateTime = (Uint32)(SDL_GetTicks() + RandFloat() * 1000);

		if (NumUpdatesPerSecondRqd > 0)
		{
			m_dUpdatePeriod = 1000.0 / NumUpdatesPerSecondRqd;
		}

		else if (isEqual(0.0, NumUpdatesPerSecondRqd))
		{
			m_dUpdatePeriod = 0.0;
		}

		else if (NumUpdatesPerSecondRqd < 0)
		{
			m_dUpdatePeriod = -1;
		}
	}

	//returns true if the current time exceeds m_dwNextUpdateTime
	bool isReady()
	{
		//if a regulator is instantiated with a zero freq then it goes into
		//stealth mode (doesn't regulate)
		if (isEqual(0.0, m_dUpdatePeriod))
			return true;

		//if the regulator is instantiated with a negative freq then it will
		//never allow the code to flow
		if (m_dUpdatePeriod < 0)
			return false;

		Uint32 CurrentTime = SDL_GetTicks();

		//the number of milliseconds the update period can vary per required
		//update-step. This is here to make sure any multiple clients of this class
		//have their updates spread evenly
		static const double UpdatePeriodVariator = 10.0;

		if (CurrentTime >= m_dwNextUpdateTime)
		{
			m_dwNextUpdateTime = (Uint32)(CurrentTime + m_dUpdatePeriod + RandInRange(-UpdatePeriodVariator, UpdatePeriodVariator));

			return true;
		}

		return false;
	}
};

#endif
