/***************************************************************************
 *   Copyright (C) 2006 by Xavier Sala   *
 *   utrescu@xaviersala.net   *
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
 
#include "Pantalla.h"
#include "Renderator.h"
#include <SDL/SDL_rotozoom.h>
#include "FrameCounter.h"
#include "Equip.h"
#include "EquipEstats.h"
#include "Regio.h"
#include "Porteria.h"

#include "Jugador.h"
#include "Porter.h"

#include "LUAManager.h"

// #define DEBUG_PANTALLA
// #define DEBUG_EQUIP
// #define DEBUG_RESULTATS

Pantalla::Pantalla(void)
{
	Fons=NULL;
	// Porteries
	Marca[EQUIP_LOCAL]=NULL;
	Marca[EQUIP_VISITANT]=NULL;

// 	tempsAnterior=0;
	p=NULL;
	MaxImatgeX=0;
	MaxImatgeY=0;
//	MidaCampX=0;
//	MidaCampY=0;
	ZoomX = 0.0;
	ZoomY = 0.0;
	
	EnJoc=false;
	LaTeElPorter=false;
}

Pantalla::~Pantalla(void)
{	
	// La pilota tampoc 
	delete bimba;
	// Eliminar les porteries 
	for (int i=0; i<2; i++)
	{
		// El marcador 
		if (Marca[i]!=NULL) delete Marca[i];
		// Eliminar els equips 
		if (equip[i]!=NULL) delete equip[i];
	}
	
	delete TerrenyDeJoc;
	delete TotElCamp;

	if (Fons!=NULL) 
	{
		SDL_FreeSurface(Fons);
		Fons=NULL;
	}
}

void Pantalla::Carrega(SDL_Surface *pp, double Escala)
{
	Punt3 punt;
	LUAManager *lm;
	char nom[15];

	p = pp;
	
	// Ens dira les coordenades de pantalla
	ZonaPantalla.x = 0;
	ZonaPantalla.w = p->w;
	ZonaPantalla.y = 0;	
	ZonaPantalla.h = p->h - ZonaPantalla.y;
	ZonaImatge=ZonaPantalla;
	
	lm = new LUAManager("camp.lua");
	
	sprintf(nom,"%s",lm->getCadena("ImatgeCamp").c_str());
	// char *nom = lm->getCadena("ImatgeCamp");

	SDL_Surface *temp = IMG_Load(nom);
	
// ----------------- SISTEMA ANTIC -------------------------------
// 	float Zoom = lm->getNumerofloat("Zoom");
// 	
// 	ZoomX = (temp->w*Zoom) / ZonaPantalla.w;
// 	ZoomY = (temp->h*Zoom) / ZonaPantalla.h;
// 	// --- Escalat de tot
// 	ZoomX = ZoomX * Escala;
// 	ZoomY = ZoomY * Escala;
// ------------------SISTEMA NOU ---------------------------------
// La idea és fer que el camp ocupi exactament tres pantalles
// per tant adapto el Zoom perquè sigui així

	ZoomX = (double) (p->w * 4)/(temp->w);
	ZoomY = (double) (p->h * 3)/(temp->h);

	// Antialias?
	Fons = zoomSurface(temp, ZoomX, ZoomY,1);
	SDL_FreeSurface(temp);
	
	// Marca on es la zona de joc 
	ZonaTerreny.x = lm->getTaulaNumeroint("TerrenyDeJoc","x")*ZoomX;                // 62*ZoomX);
	ZonaTerreny.y = lm->getTaulaNumeroint("TerrenyDeJoc","y")*ZoomY;                // 24*ZoomY);
	ZonaTerreny.w =(lm->getTaulaNumeroint("TerrenyDeJoc","w")*ZoomX)-ZonaTerreny.x; // 1137
	ZonaTerreny.h =(lm->getTaulaNumeroint("TerrenyDeJoc","h")*ZoomY)-ZonaTerreny.y; // 649
	
	// Crear les regions ... A MILLORAR!! 
	//
	// 1. Podriem fer regions de 179x125 o sigui dividir per 6 i 5 i fer-ho 
	//    com en el llibre 
	// 2. o be que cada jugador tingui la seva regio i dins d'ella hi hagi 
	//    les posicions d'atac, defensa, etc.. 
	//    llavors amb una simple funcio podriem canviar la seva posicio per 
	//    defecte segons el que faci ... Regio->Defensa(), Regio->Atac() ... 
	//    Aixo no m'obligaria a fer els dibuixos del camp perfectes de mida ja 
	//    que no importaria com siguessin. 
	
	TotElCamp = new Regio(0,0,0,Fons->w, Fons->h);
	TerrenyDeJoc = new Regio(0,ZonaTerreny);

	// Pilota (La poso al mig, no se el radi, i la massa 0.5 
	bimba = new Pilota(Punt3(460.0,460.0,0.0),0.0,0.5, Escala);
		
	// Pel moviment de pantalla des de 0 a ZonaImatge 
	MaxImatgeX = Fons->w - ZonaImatge.w;
	MaxImatgeY = Fons->h - ZonaImatge.h;

	//Inicialitzar el marcador 
	resultat.Inicia();
	
	// Definim el lloc des del que es tiraran les fores
	// El local
	punt.x = lm->getTaulaNumeroint("Fores","xlocal")*ZoomX;
	punt.y = lm->getTaulaNumeroint("Fores","dalty")*ZoomY;
	Sacada[EQUIP_LOCAL][0]=punt;
	punt.y = lm->getTaulaNumeroint("Fores","baixy")*ZoomY;
	Sacada[EQUIP_LOCAL][1]=punt;
	// El visitant
	punt.x = lm->getTaulaNumeroint("Fores","xvisitant")*ZoomX;
	punt.y = lm->getTaulaNumeroint("Fores","dalty")*ZoomY;
	Sacada[EQUIP_VISITANT][0]=punt;
	punt.y = lm->getTaulaNumeroint("Fores","baixy")*ZoomY;
	Sacada[EQUIP_VISITANT][1]=punt;

	// Ara els altres objectes ...
	// ---------------------------------  PORTERIES ---------------------------------------------------
	sprintf(nom,"%s",lm->getCadena("ImatgePorteria").c_str());

	int paldalt = lm->getTaulaNumeroint("Pal","dalt");
	int palbaix = lm->getTaulaNumeroint("Pal","baix");
	int palample = lm->getTaulaNumeroint("Pal","ample");
	int zonagolx = lm->getTaulaNumeroint("ZonaGol","dalt");
	int zonagoly = lm->getTaulaNumeroint("ZonaGol","baix");
	
	// Crear les porteries:
	// Han d'estar just allà on comença i acaba el camp, per tant ho forço
	Marca[EQUIP_LOCAL]   =new Porteria(nom, EQUIP_LOCAL,   ZoomX, ZoomY, ZonaTerreny.x, 
					        lm->getTaulaNumeroint("Porteria1","y"), lm->getTaulaNumeroint("Porteria1","altura"),
						paldalt, palbaix, palample, zonagolx, zonagoly);
		
	Marca[EQUIP_VISITANT]= new Porteria(nom, EQUIP_VISITANT,   ZoomX, ZoomY, ZonaTerreny.x + ZonaTerreny.w, 
						lm->getTaulaNumeroint("Porteria2","y"), lm->getTaulaNumeroint("Porteria2","altura"),
						paldalt, palbaix, palample, zonagolx, zonagoly);

	//	new Porteria(nom,VISITANT ,ZoomX,ZoomY, 1138, 234, 47, 279, 406, 4, 283, 400 );
	
	// Crear els equips 
	equip[EQUIP_LOCAL]  = new Equip("equip-blau.lua", Marca[0],Marca[1],this,Equip::local, Escala);
	equip[EQUIP_VISITANT] = new Equip("equip-groc.lua", Marca[1],Marca[0],this,Equip::visitant, Escala);
	
	equip[EQUIP_LOCAL]->setContraris(equip[EQUIP_VISITANT]);
	equip[EQUIP_VISITANT]->setContraris(equip[EQUIP_LOCAL]);



	// Iniciar variables del joc
	LaTeElPorter=false;
	EnJoc=true;
	// Ja no hi ha cap gol
	EsGolALaPorteria=-1;
	Sacador=0; // Local

	delete lm;
}

// Ens dona la mida del terreny de joc 
SDL_Rect Pantalla::getMidaCamp()
{
	SDL_Rect temp;
	temp.x=0;
	temp.y=0;
	temp.w=Fons->w;
	temp.h=Fons->h;
	// Eliminar els equips 
	delete equip[EQUIP_LOCAL];
	delete equip[EQUIP_VISITANT];
	// Eliminar les porteries 
	delete Marca[EQUIP_LOCAL];
	delete Marca[EQUIP_VISITANT];
	
	delete bimba; 
	
	SDL_FreeSurface(Fons);
	return temp;
}

// Posiciona la pantalla al punt indicat i tambe
// la pilota.
void Pantalla::PosicionaMig()
{
	SDL_Rect tempPilota;
	Punt3 puntMig;

	ZonaImatge.x = (Fons->w/2) - (ZonaImatge.w/2);
	ZonaImatge.y = (Fons->h/2) - (ZonaImatge.h/2);
	// Quines son les coordenades que es veuen realment

	tempPilota = bimba->getPosicioImatge();

	//2na Opcio Posicionar sobre la pantalla virtual
	tempPilota.x = (Fons->w/2)-(tempPilota.w/2);
	tempPilota.y = (Fons->h/2)-(tempPilota.h/2);

	puntMig.x = Fons->w*0.5;
	puntMig.y = Fons->h*0.5;
	
	// bimba->setPosicio(puntMig);
	bimba->setPosicio(TerrenyDeJoc->getPuntMig());
	// Posicionem al Z-Order
	bimba->setZOrder((int) TerrenyDeJoc->getPuntMig().y+bimba->getAltura());
	// bimba->setPosicioImatge(tempPilota);
	bimba->AlliberaPilota();
	// Variables de control 
	
	// XAVIER: Potser no ha de ser true fins que centri algú...
	// EnJoc=true;
	// ----
	LaTeElPorter=false;
}

// Posiciona el camp al lloc indicat.
void Pantalla::Posiciona(int x, int y)
{
	ZonaImatge.x = x - (ZonaImatge.w/2);
	ZonaImatge.y = y - (ZonaImatge.h/2);

	if (ZonaImatge.x<0) ZonaImatge.x=0;
	if (ZonaImatge.x>MaxImatgeX) ZonaImatge.x=MaxImatgeX;

	if (ZonaImatge.y<0) ZonaImatge.y=0;
	if (ZonaImatge.y>MaxImatgeY) ZonaImatge.y=MaxImatgeY;

}

// // Mou la pantalla per fer scroll
void Pantalla::Mou(int pix, int piy)
{
//	SDL_Rect temp;
	int golejadors;

	// Moure la pilota 
	// SDL_Rect PosicioPilotaAnterior = bimba->getPosicioImatge();
	bool shamogut = bimba->Mou();
	if (shamogut==true)
	{
        // SDL_Rect PosicioPilotaActual = bimba->getPosicioImatge();
		Punt3 PosicioPilotaActual = bimba->getPosicio();
		
		// if (!IntersectRect(&temp, &PosicioPilotaActual, &ZonaTerreny))
		if (TerrenyDeJoc->isFora(PosicioPilotaActual))
		{
			// La Pilota ha sortit fora del terreny de joc
			// 1. Mirar si han fet gol??? 
			if (EnJoc==true)
			{
				int Perill = -1;
				// 1. Primer mirar si fa falta comprovar les porteries o no
				if (PosicioPilotaActual.x <= TerrenyDeJoc->getMinX())
				{
					Perill=EQUIP_LOCAL;
				}
				else if (PosicioPilotaActual.x >= TerrenyDeJoc->getMaxX())
				{
					Perill=EQUIP_VISITANT;
				}
				if (Perill>=0)
				{
					golejadors = (Perill+1)%2;
					// Cal mirar les porteries!! Pot ser un gol?
					int QueHaPassat= Marca[Perill]->XutAPorteria(bimba);
					switch(QueHaPassat)
					{
						case Porteria::GOL:
							
#ifdef DEBUG_RESULTATS
							// Evitar problemes en gols en propia porta
							if (equip[golejadors]->getJugadorAmbLaPilota()!=NULL)
							{
								printf("----------- GOL!!! del jugador %d-%s\n", equip[golejadors]->getJugadorAmbLaPilota()->getID());
							}
							else
							{
								printf("---------- Gol en Propia porta?\n");
							}
#endif
							EnJoc=false;
							EsGolALaPorteria=Perill;
							bimba->CapturaPilota(Marca[Perill]->getTerra(), true);
							
							resultat.HaMarcat(golejadors);
							// Fer tornar tothom a casa, uns perque han marcat i els 
							// altres perque han rebut el gol
							equip[golejadors]->HemMarcat();
							equip[golejadors]->getFSM()->canviaEstat(EquipEstatTornaPosicioInicial::Instance());
							equip[Perill]->getFSM()->canviaEstat(EquipEstatTornaPosicioInicial::Instance());
							equip[Perill]->PorterACasa();
							// Fer seguir la pantalla al jugador que ha marcat 
							// ZoomSegueix(local->getJugadorAmbLaPilota());

							// Variable que servirà per decidir qui ha de centrar de mig camp
							Sacador=Perill;
							break;
						case Porteria::PAL:
#ifdef DEBUG_RESULTATS
							if (equip[golejadors]->getJugadorAmbLaPilota()!=NULL)
							{
							printf("----------- Pal!!! del jugador %d\n", equip[golejadors]->getJugadorAmbLaPilota()->getID());
							}
							else
							{
								printf("-------- pilota al pal\n");
							}
#endif

							bimba->InverteixXut(2);
							EnJoc=true;
							break;
						case Porteria::FORA:
#ifdef DEBUG_RESULTATS
							if (equip[golejadors]->getJugadorAmbLaPilota()!=NULL)
							{
								printf("----------- Xut fora!!! del jugador %d\n", equip[golejadors]->getJugadorAmbLaPilota()->getID());
							}
							else
							{
								printf("----------- Còrner?\n");
							}
#endif
							EnJoc=false;
							bimba->CapturaPilota(Marca[Perill]->getTerra(), false);
							// Fer a la gent tornar
							equip[0]->PilotaFora();
							equip[1]->PilotaFora();
							PosarLaPilota = getSacadaFora();
							// Això serà perquè només xuti el porter
							LaTeElPorter=true;
							break;
						case Porteria::PERSOBRE:
#ifdef DEBUG_RESULTATS
							printf("----------- Surt per sobre!!! del jugador %d\n", equip[golejadors]->getJugadorAmbLaPilota()->getID());
#endif
							EnJoc=false;
							equip[0]->PilotaFora();
							equip[1]->PilotaFora();
							PosarLaPilota = getSacadaFora();
							// Això serà perquè només xuti el porter
							LaTeElPorter=true;
							break;
					}
				}
				else 
				{
					// Si no ha sortit pels costats es que no ha arribat a la 
					// porteria, per tant es fora de banda  
					EnJoc=false;
					// Fer a la gent tornar
#ifdef DEBUG_RESULTATS
							printf("----------- Fora de Banda \n");
#endif
					equip[0]->PilotaForaBanda();
					equip[1]->PilotaForaBanda();
					PosarLaPilota = bimba->getPosicio();
					// Potser hauria de dir-los que es reposicionin??? 
					
					// Xapussa per fer que ningú vulgui xutar la pilota
					// LaTeElPorter=true;
				}
				
			}
			else
			{
				// La pilota no està en joc... per tant reposicionar al lloc pel que 
				// ha sortit
				if (EsGolALaPorteria<0)
				{
					if (TotElCamp->isFora(PosicioPilotaActual) || bimba->getVectorVelocitat().isZero())
					{
						if (!PosarLaPilota.isZero())
						{
							bimba->setPosicio(PosarLaPilota);
							bimba->setVelocitat(Punt3(0.0,0.0,0.0));
							PosarLaPilota.Zero();
						}
					}
				}
			}
		}
	
		Posiciona(PosicioPilotaActual.x,PosicioPilotaActual.y);
	}
	else
	{
		// No s'ha mogut la pilota ??? Si la pilota esta en joc cap problema
		// però si no hi està ... 
		if (EnJoc==false && EsGolALaPorteria<0 && !PosarLaPilota.isZero())
		{
			// No es mou no cal que miri la velocitat ... nomes si ha sortit 
			// del camp 
			bimba->setPosicio(PosarLaPilota);
			bimba->setVelocitat(Punt3(0.0,0.0,0.0));
			Posiciona(PosarLaPilota.x,PosarLaPilota.y);
			PosarLaPilota.Zero();
		}
		
	}
	equip[EQUIP_LOCAL]->Mou();
	equip[EQUIP_VISITANT]->Mou();
	
}

/*
	Pinta la pantalla
    Com que volem fer un scroll, cal que pintem en trossos
*/
void Pantalla::Pinta(void)
{
//	Uint32 currTime, delta;
	// #### Pintar el fons
	SDL_BlitSurface(Fons, &ZonaImatge, p, &ZonaPantalla);

	// Mira si s'han de pintar les porteries
	Marca[EQUIP_LOCAL]->Mou(getZonaVisible());
	Marca[EQUIP_VISITANT]->Mou(getZonaVisible());

#ifdef DEBUG_EQUIP
	equip[EQUIP_LOCAL]->Pinta(p);
	equip[EQUIP_VISITANT]->Pinta(p);
#endif	

	Renderitza->Ordenar();

	std::vector<Entitat*>::const_iterator objectes = Renderitza->Membres().begin();

	for (objectes; objectes != Renderitza->Membres().end(); ++objectes)
	{
		// Ara els hauria de convertir al tipus adequat per poder-los pintar
		switch ((*objectes)->getTipusEntitat())
		{
			case 0:
				dynamic_cast<Pilota*>(*objectes)->Pinta(p,getZonaVisible());
				break;
			case 1:
				dynamic_cast<Porteria*>(*objectes)->Pinta(p, getZonaVisible());
				break;
			case 2:
				dynamic_cast<Jugador*>(*objectes)->Pinta(p);
				break;
			case 3:
				dynamic_cast<Porter*>(*objectes)->Pinta(p);
				break;
		}

		// No caldrà que faci les comprovacions posteriors ...
	}
	Renderitza->Buida();

	// #### Pintar els jugadors
	// equip[LOCAL]->Pinta(p);
	// equip[VISITANT]->Pinta(p);
	// #### Pintar la pilota 
	// bimba->Pinta(p, ZonaImatge);	
	// #### Pintar les porteries
	// Marca[LOCAL]->Pinta(p,ZonaImatge);
	// Marca[VISITANT]->Pinta(p,ZonaImatge);
	
	if (EnJoc==false)
	{
		// Ha sortit del terreny de joc, si es gol s'ha de pintar perque sembli 
		// que entra a dins
		if (EsGolALaPorteria >= 0)
		{
			Marca[EsGolALaPorteria]->Pinta(p,ZonaImatge);

#ifdef DEBUG_PANTALLA
			// Fes un requadre negre que marcarà que 
			// s'ha fet un gol
			SDL_Rect Posicio;
			Posicio.x=410;
			Posicio.y=0;
			Posicio.w=20;
			Posicio.h=20;
			SDL_FillRect(p,&Posicio,0x000000);
#endif	
		}

#ifdef DEBUG_PANTALLA
		// Fes un requadre vermell que marcarà que 
		// la pilota no està en joc
		SDL_Rect Posicio;
		Posicio.x=370;
		Posicio.y=0;
		Posicio.w=20;
		Posicio.h=20;
		SDL_FillRect(p,&Posicio,0xFF0000);
		// Requadre verd si la té el porter
		if (LaTeElPorter==true)
		{
			Posicio.x=350;
			SDL_FillRect(p,&Posicio,0xF5CE19);
		}
#endif	
	}
	// #### Pintar el marcador
	resultat.Pinta(p);
}

// Ens retorna un punter a la pilota
Pilota* Pantalla::getPilota()
{
	return bimba;
}

Marcador* Pantalla::getMarcador()
{
	return &resultat;
}


Punt3 Pantalla::getSacadaFora()
{
	int Quin=0;
	int Quin2=0;

	if (bimba->getPosicio().x > getTerrenyDeJoc()->getPuntMig().x) Quin=EQUIP_VISITANT;
				 			     else  Quin=EQUIP_LOCAL;
	if (bimba->getPosicio().y > getTerrenyDeJoc()->getPuntMig().y) Quin2=1;
							     else  Quin2=0;
	return Sacada[Quin][Quin2];
}


bool Pantalla::IntersectRect(SDL_Rect *dest,const SDL_Rect *src1,const SDL_Rect *src2)
{
	int px0,py0,px1,py1;
	int cx0,cy0,cx1,cy1;
	int rx0,ry0,rx1,ry1;
  
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

	if(/*(cx0 < px0) && */(cx1 < px0))
		return false;
  
	if((cx0 > px1) /*&& (cx1 > px1)*/)
		return false;
  
	if(/*(cy0 < py0) && */(cy1 < py0))
		return false;
  
	if((cy0 > py1) /*&& (cy1 > py1)*/)
		return false;
  
  // intersect x

	if(cx0 <= px0) rx0 = px0;
	else rx0 = cx0;
  
	if(cx1 >= px1) rx1 = px1;
	else rx1 = cx1;
  
  // intersect y

	if(cy0 <= py0) ry0 = py0;
	else ry0 = cy0;
  
	if(cy1 >= py1) ry1 = py1;
	else ry1 = cy1;
  
  // fill in result rect

	dest->x = rx0;
	dest->y = ry0;
	dest->w = (rx1-rx0)+1;
	dest->h = (ry1-ry0)+1;

	return true;
};

