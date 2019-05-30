-- ********************** EL CAMP ***************************
-- Definicions basiques: Fitxer i escalat de la imatge
ImatgeCamp = "camp.png"
Zoom   = 2.0;

-- Definim el terreny de joc de la imatge (sense zoom)
TerrenyDeJoc = { x = 62, y = 24, w = 1137, h = 649 }

-- Lloc des del que es saquen les fores de porteria (sense zoom)
Fores = { xlocal = 108, xvisitant = 1092, dalty = 261, baixy = 433 }

-- ************** LES PORTERIES ******************************
ImatgePorteria = "porteria.png"

-- Lloc on han d'apareixer les porteries (la x s'obté del terreny de joc)

Porteria1 = { y = 234, altura = 47 }
Porteria2 = { y = 234, altura = 47 }

-- Definicio dels pals (segons la seva Y en el dibuix i l'amplada)
Pal = { dalt = 279, baix = 406, ample = 4 }

-- Definicio de la zona de gol HY
ZonaGol = { dalt = 283, baix = 400 }

