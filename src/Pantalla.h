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

#pragma once
#include "Pilota.h"
#include "Porteria.h"
#include "Marcador.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "filepath.h"

class Equip;
class Regio;

/*
        ___________________________ 
       |             |             | 
       |___          |          ___|  
       |_  |         |         |  _| 
      .| | |.       ,|.       .| | |. 
      || | | )     ( | )     ( | | || 
      '|_| |'       `|'       `| |_|'  
       |___|         |         |___| 
       |             |             |  
       |_____________|_____________|  
	
*/

class Pantalla
{
public:
    Pantalla(void);
    ~Pantalla(void);

    SDL_Surface *getSuperficie() { return p; }
    // // Mou la pantalla per fer scroll
    void Mou(int pix, int piy);
    void Pinta(void);
    // Carregar imatges i tal...
    void Carrega(SDL_Surface *pp, double Escala);

    // Posiciona el camp amb la pilota "of course"
    void Posiciona(int x, int y);
    void PosicionaMig();
    Pilota *getPilota();

    Marcador *getMarcador();

    SDL_Rect getMidaCamp();
    double getLlargadaCamp() { return ZonaTerreny.w; }
    SDL_Rect getZonaJoc() { return ZonaTerreny; }
    Regio *getTerrenyDeJoc() { return TerrenyDeJoc; }
    SDL_Rect getZonaVisible() { return ZonaImatge; }

    Punt3 getSacadaFora();
    int getQuiHaDeSacar() { return Sacador; }

    bool isPartitEnJoc() { return EnJoc; };
    void setPartitEnJoc(bool sino)
    {
        EnJoc = sino;
        EsGolALaPorteria = -1;
    }
    bool isGol() { return (EsGolALaPorteria >= 0); }
    bool isFora() { return !PosarLaPilota.isZero(); }
    bool isEnMansDelPorter() { return LaTeElPorter; }
    void setLaTeElPorter(bool la) { LaTeElPorter = la; }

    double getZoomX() { return ZoomX; }
    double getZoomY() { return ZoomY; }

private:
    // --------------- GRAFICS ---------------------------------------
    // Pantalla a on pintarem els dibuixos
    SDL_Surface *p;
    // Imatge que tindrem de fons.
    SDL_Surface *Fons;

    // Mida Real de la imatge (menys l'amplada per millorar càlculs
    int MaxImatgeX;
    int MaxImatgeY;
    // Imatges de les Porteries
    Porteria *Marca[2];
    // Posicions des de les que es treu la pilota
    Punt3 Sacada[2][2];
    // -----------------------------------------------------------------
    // ----------- Definicions sobre el camp i l'scroll ----------------
    // Rectangle de la imatge a veure, també servirà de referència sobre on estem dins de la imatge.
    SDL_Rect ZonaImatge;
    // Rectangle de destí de la imatge. Només per pintar
    SDL_Rect ZonaPantalla;
    // Ens dirà quines coordenades són les que pertanyen al terreny de joc
    // O sigui és per saber si la pilota és fora del camp
    SDL_Rect ZonaTerreny;
    Regio *TerrenyDeJoc;
    Regio *TotElCamp;
    Punt3 PosarLaPilota;
    // Diu quin és l'escalat que hem aplicat a la pantalla
    double ZoomX;
    double ZoomY;

    // Si la pilota ha sortit del terreny, ja no s'està en joc
    bool EnJoc;
    // Han marcat a la porteria indicada 0 o 1, -1 és no gol
    int EsGolALaPorteria;
    int Sacador;
    // Si la pilota la té agafada el porter, la gent no ha d'anar a per ella
    bool LaTeElPorter;
    // Uint32 *tempsAnterior;
    bool IntersectRect(SDL_Rect *dest, const SDL_Rect *src1, const SDL_Rect *src2);

    // -------------------------------------------------------------------
    // ----------- Objectes dins del camp --------------------------------
    // L'objectiu del joc ...
    Pilota *bimba;
    // Els contendents
    Equip *equip[2];
    // Marcador
    Marcador resultat;
    // Regions???
};
