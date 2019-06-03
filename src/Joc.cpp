/***************************************************************************
 *   Copyright (C) 2006 by Xavier Sala   *
 *   utrescu@xaviersala.net    *
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

#include "Joc.h"
#include "Pantalla.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>

#include "LUAManager.h"

using namespace std;

// #define FPS_ON

#ifdef FPS_ON
class CalculaFPS
{
public:
	CalculaFPS();
	double show_delta();
	bool update();
	void Change();

private:
	double now_time, then_time;
	float time_per_fps_disp;
};

CalculaFPS::CalculaFPS()
{
	then_time = clock();
	now_time = clock();
	time_per_fps_disp = CLOCKS_PER_SEC * 5;
}

double CalculaFPS::show_delta()
{
	return (now_time - then_time) / CLOCKS_PER_SEC;
}

bool CalculaFPS::update()
{
	// then_time = now_time;
	now_time = clock();
	return (now_time - then_time > time_per_fps_disp);
}

void CalculaFPS::Change()
{
	then_time = clock();
	return;
}

#endif // FPS_ON End

Joc::Joc(void)
{
	srand((unsigned)time(NULL));
	// 1. Iniciar el SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_OPENGL) < 0)
	{
		fprintf(stderr, "Error en la inicialització de Video: %s\n",
				SDL_GetError());
		SDL_Quit();
	}

	SDL_WM_SetIcon(SDL_LoadBMP(getFullFileName("gol.bmp").c_str()), NULL);
	// SDL_WM_SetIcon(icon, NULL);
	SDL_WM_SetCaption("Amateur Evolution Soccer", getFullFileName("gol.ico").c_str());

	atexit(SDL_Quit);

	LUAManager *lm = new LUAManager("general.lua");

	ResolucioX = lm->getTaulaNumeroint("Resolucio", "x");
	ResolucioY = lm->getTaulaNumeroint("Resolucio", "y");
	// Per saber en quina proporció s'ha de modificar tot.
	// Considerem "NORMAL" a 800x600 i per tant qualsevol altre
	// requerirà Escalar.
	Escala = ResolucioX / 800.0;

	Colors = lm->getTaulaNumeroint("Resolucio", "colors");

	int PantallaSencera = lm->getNumeroint("PantallaSencera");

	if (PantallaSencera == 1)
	{
		FonsPantalla = SDL_SetVideoMode(ResolucioX,
										ResolucioY,
										Colors,
										SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
	}
	else
	{
		FonsPantalla = SDL_SetVideoMode(ResolucioX,
										ResolucioY,
										Colors,
										SDL_HWSURFACE | SDL_DOUBLEBUF);
	}

	if (FonsPantalla == NULL)
	{
		printf("Posar el video a %dx%d no ha estat possible: %s\n", ResolucioX, ResolucioY, SDL_GetError());
	}
	else
	{
		// 2. el so
		// Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,MIX_DEFAULT_FORMAT,1,4096);
		// Podria carregar la msica

		// SoundLoad("public.wav",SO_PUBLIC);
		// SoundLoad("pito.wav",SO_PITO);
		// SoundLoad("gol.wav",SO_GOL);
		// SoundLoad("xut.wav",SO_XUT);

		// Amagar el cursor
		// SDL_ShowCursor(0);
	}

	if (TTF_Init() == -1)
	{
		printf("Error al carregar les fonts!\n");
	}

	delete lm;
	lm = NULL;
}

Joc::~Joc(void)
{
	SDL_FreeSurface(FonsPantalla);
}

// La funció principal del joc
bool Joc::Inicia(void)
{
	int retorn = PantallaIntro();
	if (retorn == 1)
		PantallaJoc();
	return false;
}

int Joc::PantallaJoc()
{
	SDL_Event event;
	SDLKey tecla = (SDLKey)0;
	int done = 0, Ataco = 0, Punts = 0, MovimentPantalla = 0;
	//	Uint32 timeActual;
	double Costat, Dalt;
	Punt3 xut;
	// ------ FrameRates ----
	SDL_initFramerate(&manager);

	Costat = 0.0;
	Dalt = 0.0;

#ifdef FPS_ON
	int ComptaFrames = 0;
	CalculaFPS *T;
	T = new CalculaFPS();
#endif

	// ----- Definir els Objectes del joc ---
	Pantalla Camp;

	// 1. Carregar pantalla (a l'escala determinada)

	Camp.Carrega(FonsPantalla, Escala);

	// Posa't al mig
	Camp.PosicionaMig();
	// Inicialitzacions bàsiques
	done = 0;
	// Msica
	// PlaySoundRepetit(SO_PUBLIC);
	SDL_setFramerate(&manager, 40);
	while (done == 0)
	{

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				done = 2;
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					done = 2;
					break;
				case SDLK_DOWN:
					xut.y = 0.0;
					break;
				case SDLK_UP:
					xut.y = 0.0;
					break;
				case SDLK_LEFT:
					xut.x = 0.0;
					break;
				case SDLK_RIGHT:
					xut.x = 0.0;
					break;
				}

				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					xut.x = -5;
					break;
				case SDLK_RIGHT:
					xut.x = 5;
					break;
				case SDLK_UP:
					xut.y = -5;
					break;
				case SDLK_DOWN:
					xut.y = 5;
					break;
				case SDLK_s:
					// passada en la direcció
					// Camp.getPilota()->Xuta2(xut.x,xut.y,0,4);
					xut.h = 0.0;
					Camp.getPilota()->Xuta(xut, 4);
					break;
				case SDLK_a:
					// Bombeja
					// Camp.getPilota()->Xuta2(xut.x,xut.y,10,4);
					xut.h = 10.0;
					Camp.getPilota()->Xuta(xut, 4);
					break;
				case SDLK_d:
					// Xut fort
					xut.x *= 2;
					xut.y *= 2;
					xut.h = 2.0;
					// Camp.getPilota()->Xuta2(xut.x*2,xut.y*2,4,4);
					Camp.getPilota()->Xuta(xut, 6);
				}
				break;
			} // switch tecla
		}	 // while event

		if (done == 0)
		{

			// 1. Moure la gent de la pantalla i la pantalla per l'scroll
			// ------------  MOURE A TOTHOM  ------------
			Camp.Mou(0, 0);
			Camp.Pinta();
#ifdef FPS_ON
			ComptaFrames++;
			if (T->update() == true)
			{
				printf("Frames: %d, Delta: %f, FPS:%f\n", ComptaFrames, T->show_delta(), ComptaFrames / T->show_delta());
				T->Change();
				ComptaFrames = 0;
			}
#endif
			SDL_framerateDelay(&manager);
			SDL_Flip(FonsPantalla);
		} // if done==0
	}	 // while

	// Faig neteja dels vectors perquè hem acabat la pantalla
	return done;
}

int Joc::PantallaIntro()
{
	return 1;
}

bool Joc::IntersectRect(SDL_Rect *dest, const SDL_Rect *src1, const SDL_Rect *src2)
{
	int px0, py0, px1, py1;
	int cx0, cy0, cx1, cy1;
	int rx0, ry0, rx1, ry1;

	// fill in default (NULL) result rectangle

	dest->x = 0;
	dest->y = 0;
	dest->w = 0;
	dest->h = 0;

	// get coordinates of the rectangles

	px0 = src1->x;
	py0 = src1->y;
	px1 = src1->x + src1->w - 1;
	py1 = src1->y + src1->h - 1;

	cx0 = src2->x;
	cy0 = src2->y;
	cx1 = src2->x + src2->w - 1;
	cy1 = src2->y + src2->h - 1;

	// check if the rectangles intersect

	if (/*(cx0 < px0) && */ (cx1 < px0))
		return false;

	if ((cx0 > px1) /*&& (cx1 > px1)*/)
		return false;

	if (/*(cy0 < py0) && */ (cy1 < py0))
		return false;

	if ((cy0 > py1) /*&& (cy1 > py1)*/)
		return false;

	// intersect x

	if (cx0 <= px0)
		rx0 = px0;
	else
		rx0 = cx0;

	if (cx1 >= px1)
		rx1 = px1;
	else
		rx1 = cx1;

	// intersect y

	if (cy0 <= py0)
		ry0 = py0;
	else
		ry0 = cy0;

	if (cy1 >= py1)
		ry1 = py1;
	else
		ry1 = cy1;

	// fill in result rect

	dest->x = rx0;
	dest->y = ry0;
	dest->w = (rx1 - rx0) + 1;
	dest->h = (ry1 - ry0) + 1;

	return true;
};
