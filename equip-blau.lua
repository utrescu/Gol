-- ********************** ELs JUGADORS ***************************
NomEquip = "Blauets"

-- Quants punts avalua l'equip per provar de xutar a porta
ProvesPerXut = 6

-- Numero de jugadors (comptant el porter)
NumJugadors = 8

-- Primer jugador, en principi sera el porter

Porter = { 
	Nom = "Yashin", Numero = 1, Imatge = "porter1.lua", 
	-- Per les animacions
	Animacions = 1,	
	-- De què juga el jugador: 1->porter 2->defensa 3->migcapista 4->davanter
	JugaDe=1,
	-- Característiques generals: Força màxima i Massa 
	Forca = 20 ,  Massa = 5, 
	
	-- Velocitat màxima (amb pilota, sense pilota, de rotació
	VelocitatAmbPilota = 6, VelocitatSensePilota = 5, Rotacio = 0.4, 
	
	-- Xuts: força de xut, precissió dels xuts (0-1) i possibilitat de xutar encara que surti que no marcarà(0-1)
	ForcaXut=10, Punteria = 0.999, PossibleXutBoig = 0.05, 
	
	-- Passades: Força, distàcia màxima de passada, provabilitat d'esperar el passe o anar-lo a buscar 
	-- (ZonaComfort serveix per saber si el jugador està marcat -no va encara- **)
	ForcaPassada = 8,  MinimaDistanciaPassada = 270, EsperaOHiVa=0.5, ZonaComfort=132,
	
	-- Dribblings: Força i a quina distancia comença el davanter a girar si puja la banda (el darrer encara no va)
	ForcaDribbling=3, AtacantsGirenSiSonA = 475,
	
	-- PORTER (A quina distancia segueix la pilota, a quina distancia surt a per la pilota, 
	-- i a quina distancia el porter agafa la pilota) i temps de recuperacio del porter
	DistanciaSeguiment=27, DistanciaSortirAPerPilota = 300, DistanciaAgafarPilota = 18, ReccioPorter = 1,
	
	-- Posicions estatiques de defensa i atac
	DefensaX =  81, DefensaY = 347, AtacX = 81, AtacY = 347
}

-- Segon jugador

Jugador1 = { 
	Nom = "Pep", Numero = 2, Imatge = "blau.lua", 
	-- Per les animacions
	Animacions = 3,	
	-- De què juga el jugador: 1->porter 2->defensa 3->migcapista 4->davanter
	JugaDe=2,
	-- Característiques generals: Força màxima i Massa
	Forca = 20 ,  Massa = 5, 
	
	-- Velocitat màxima (amb pilota, sense pilota, de rotació
	VelocitatAmbPilota = 6, VelocitatSensePilota = 5, Rotacio = 0.4, 
	
	-- Xuts
	ForcaXut=10, Punteria = 0.99,  PossibleXutBoig = 0.05, 
	
	-- Passades (ZonaComfort **)
	ForcaPassada = 8,  MinimaDistanciaPassada = 100, EsperaOHiVa=0.5, ZonaComfort=132,
	
	-- Dribblings
	ForcaDribbling=3, AtacantsGirenSiSonA = 475,
	
	-- Posicions de defensa i atac
	DefensaX = 176, DefensaY = 209, AtacX = 618, AtacY = 76
}

Jugador2 = { 
	Nom = "Pep", Numero = 3, Imatge = "blau.lua", 
	-- Per les animacions
	Animacions = 3,	
	-- De què juga el jugador: 1-> porter 2-> defensa 3->migcapista 4->davanter
	JugaDe=2,
	-- Característiques generals: Força màxima i Massa
	Forca = 20 ,  Massa = 5, 
	
	-- Velocitat màxima (amb pilota, sense pilota, de rotació
	VelocitatAmbPilota = 6, VelocitatSensePilota = 5, Rotacio = 0.4, 
	
	-- Xuts
	ForcaXut=10, Punteria = 0.99,  PossibleXutBoig = 0.05, 
	
	-- Passades (ZonaComfort **)
	ForcaPassada = 8,  MinimaDistanciaPassada = 100, EsperaOHiVa=0.5, ZonaComfort=132,
	
	-- Dribblings
	ForcaDribbling=3, AtacantsGirenSiSonA = 475,

	-- Posicions de defensa i atac
	DefensaX = 177, DefensaY = 400, AtacX = 618, AtacY = 550
}

Jugador3 = { 
	Nom = "Pep", Numero = 4, Imatge = "blau.lua", 
	-- Per les animacions
	Animacions = 3,	
	-- De què juga el jugador: 1-> porter 2-> defensa 3->migcapista 4->davanter
	JugaDe=2,
	-- Característiques generals: Força màxima i Massa
	Forca = 20 ,  Massa = 5, 
	
	-- Velocitat màxima (amb pilota, sense pilota, de rotació
	VelocitatAmbPilota = 6, VelocitatSensePilota = 5, Rotacio = 0.4, 
	
	-- Xuts
	ForcaXut=10, Punteria = 0.99,  PossibleXutBoig = 0.05, 
	
	-- Passades (ZonaComfort **)
	ForcaPassada = 8,  MinimaDistanciaPassada = 100, EsperaOHiVa=0.5, ZonaComfort=132,
	
	-- Dribblings
	ForcaDribbling=3, AtacantsGirenSiSonA = 475,
	
	-- Posicions de defensa i atac
	DefensaX = 227, DefensaY = 346, AtacX = 618, AtacY = 348
}

Jugador4 = { 
	Nom = "Pep", Numero = 5, Imatge = "blau.lua", 
	-- Per les animacions
	Animacions = 3,	
	-- De què juga el jugador: 1-> porter 2-> defensa 3->migcapista 4->davanter
	JugaDe=3,
	-- Característiques generals: Força màxima i Massa
	Forca = 20 ,  Massa = 5, 
	
	-- Velocitat màxima (amb pilota, sense pilota, de rotació
	VelocitatAmbPilota = 6, VelocitatSensePilota = 5, Rotacio = 0.4, 
	
	-- Xuts
	ForcaXut=10, Punteria = 0.99,  PossibleXutBoig = 0.05, 
	
	-- Passades (ZonaComfort **)
	ForcaPassada = 8,  MinimaDistanciaPassada = 100, EsperaOHiVa=0.5, ZonaComfort=132,
	
	-- Dribblings
	ForcaDribbling=3, AtacantsGirenSiSonA = 475,
	
	-- Posicions de defensa i atac
	DefensaX = 381, DefensaY = 337, AtacX = 843, AtacY = 294
}

Jugador5 = { 
	Nom = "Pep", Numero = 6, Imatge = "blau.lua", 
	-- Per les animacions
	Animacions = 3,	
	-- De què juga el jugador: 1-> porter 2-> defensa 3->migcapista 4->davanter
	JugaDe=4,
	-- Característiques generals: Força màxima i Massa
	Forca = 20 ,  Massa = 5, 
	
	-- Velocitat màxima (amb pilota, sense pilota, de rotació
	VelocitatAmbPilota = 6, VelocitatSensePilota = 5, Rotacio = 0.4, 
	
	-- Xuts
	ForcaXut=10, Punteria = 0.99,  PossibleXutBoig = 0.05, 
	
	-- Passades (ZonaComfort **)
	ForcaPassada = 8,  MinimaDistanciaPassada = 100, EsperaOHiVa=0.5, ZonaComfort=132,
	
	-- Dribblings
	ForcaDribbling=3, AtacantsGirenSiSonA = 475,
	
	-- Posicions de defensa i atac
	DefensaX = 536, DefensaY = 139, AtacX = 1030, AtacY = 129
}

Jugador6 = { 
	Nom = "Pep", Numero = 7, Imatge = "blau.lua", 
	-- Per les animacions
	Animacions = 3,	
	-- De què juga el jugador: 1-> porter 2-> defensa 3->migcapista 4->davanter
	JugaDe=4,
	-- Característiques generals: Força màxima i Massa
	Forca = 20 ,  Massa = 5, 
	
	-- Velocitat màxima (amb pilota, sense pilota, de rotació
	VelocitatAmbPilota = 6, VelocitatSensePilota = 5, Rotacio = 0.4, 
	
	-- Xuts
	ForcaXut=10, Punteria = 0.99,  PossibleXutBoig = 0.05, 
	
	-- Passades (ZonaComfort **)
	ForcaPassada = 8,  MinimaDistanciaPassada = 100, EsperaOHiVa=0.5, ZonaComfort=132,
	
	-- Dribblings
	ForcaDribbling=3, AtacantsGirenSiSonA = 475,
	
	-- Posicions de defensa i atac
	DefensaX = 536, DefensaY = 550, AtacX = 1038, AtacY = 535
}

Jugador7 = { 
	Nom = "Pepinho", Numero = 8, Imatge = "blau2.lua", 
	-- Per les animacions
	Animacions = 3,	
	-- De què juga el jugador: 1-> porter 2-> defensa 3->migcapista 4->davanter
	JugaDe=4,
	-- Característiques generals: Força màxima i Massa
	Forca = 20 ,  Massa = 5, 
	
	-- Velocitat màxima (amb pilota, sense pilota, de rotació
	VelocitatAmbPilota = 6, VelocitatSensePilota = 6, Rotacio = 0.4, 
	
	-- Xuts
	ForcaXut=11, Punteria = 0.999, PossibleXutBoig = 0.03, 
	
	-- Passades (ZonaComfort **)
	ForcaPassada = 8,  MinimaDistanciaPassada = 100, EsperaOHiVa=0.5, ZonaComfort=132,
	
	-- Dribblings
	ForcaDribbling=3, AtacantsGirenSiSonA = 475,
	
	-- Posicions de defensa i atac
	DefensaX = 510, DefensaY = 345, AtacX = 1000, AtacY = 344
}
-- Pel càlculs de punts de suport
PuntsSuport = { x = 10, y = 6 }
PuntsSweet  = { x = 13, y = 6 }
