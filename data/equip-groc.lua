-- ********************** ELs JUGADORS ***************************
NomEquip = "Brausil"

-- Quants punts avalua l'equip per provar de xutar a porta
ProvesPerXut = 6

-- Numero de jugadors (comptant el porter)
NumJugadors = 8

-- Primer jugador, en principi sera el porter

Porter = { 
	Nom = "Arkonada", Numero = 1, Imatge = "porter2.lua", 
	-- Per les animacions
	Animacions = 1,	
	-- De què juga el jugador: 1-> porter 2-> defensa 3->migcapista 4->davanter
	JugaDe=1,
	-- Característiques generals: Força màxima i Massa
	Forca = 20 ,  Massa = 5, 
	
	-- Velocitat màxima (amb pilota, sense pilota, de rotació
	VelocitatAmbPilota = 6, VelocitatSensePilota = 4, Rotacio = 0.3, 
	
	-- Xuts: força de xut, precissió dels xuts (0-1) i possibilitat de xutar encara que surti que no marcarà (0-1)
	ForcaXut = 10, Punteria = 0.9, PossibleXutBoig = 0.05, 
	
	-- Passades: Força, distància màxima de passada, provabilitat d'esperar el passe o anar-lo a buscar 
	-- ZonaComfort serveix per saber si el jugador està marcat
	ForcaPassada = 8,  MinimaDistanciaPassada = 270, EsperaOHiVa=0.5, ZonaComfort=132,
	
	
	-- Dribblings: Força i a quina distancia comença el davanter a girar si puja la banda (el darrer encara no va)
	ForcaDribbling=3, AtacantsGirenSiSonA = 475,
	
	-- PORTER (A quina distancia segueix la pilota, i a quina distancia surt a per la pilota)
	DistanciaSeguiment=27, DistanciaSortirAPerPilota = 300, DistanciaAgafarPilota = 18, ReccioPorter=1.5,
	
	-- Posicions estatiques de defensa i atac
	DefensaX =  1113, DefensaY = 347, AtacX = 1113, AtacY = 347
}

-- Segon jugador

Jugador1 = { 
	Nom = "Pau1", Numero = 2, Imatge = "groc.lua", 
	-- Per les animacions
	Animacions = 2,	
	-- De què juga el jugador: 1-> porter 2-> defensa 3->migcapista 4->davanter
	JugaDe=2,
	-- Característiques generals: Força màxima i Massa
	Forca = 20 ,  Massa = 5, 
	
	-- Velocitat màxima (amb pilota, sense pilota, de rotació
	VelocitatAmbPilota = 6, VelocitatSensePilota = 4, Rotacio = 0.3, 
	
	-- Xuts
	ForcaXut=10, Punteria = 0.9, PossibleXutBoig = 0.05, 
	
	-- Passades (ZonaComfort **)
	ForcaPassada = 8,  MinimaDistanciaPassada = 120, EsperaOHiVa=0.5, ZonaComfort=132,
	
	-- Dribblings
	ForcaDribbling=3, AtacantsGirenSiSonA = 475,
	
	-- Posicions de defensa i atac
	DefensaX = 1038, DefensaY = 129, AtacX = 500, AtacY = 344
}

Jugador2 = { 
	Nom = "Pau2", Numero = 3, Imatge = "groc.lua", 
	-- Per les animacions
	Animacions = 2,	
	-- De què juga el jugador: 1-> porter 2-> defensa 3->migcapista 4->davanter
	JugaDe=2,
	-- Característiques generals: Força màxima i Massa
	Forca = 20 ,  Massa = 5, 
	
	-- Velocitat màxima (amb pilota, sense pilota, de rotació
	VelocitatAmbPilota = 6, VelocitatSensePilota = 4, Rotacio = 0.3, 
	
	-- Xuts
	ForcaXut=10, Punteria = 0.9,  PossibleXutBoig = 0.05, 
	
	-- Passades (ZonaComfort **)
	ForcaPassada = 8,  MinimaDistanciaPassada = 120, EsperaOHiVa=0.5, ZonaComfort=132,
	
	-- Dribblings
	ForcaDribbling=3, AtacantsGirenSiSonA = 475,
	
	-- Posicions de defensa i atac
	DefensaX = 1038, DefensaY = 535, AtacX = 500, AtacY = 535
}

Jugador3 = { 
	Nom = "Pau3", Numero = 4, Imatge = "groc.lua", 
	-- Per les animacions
	Animacions = 2,	
	-- De què juga el jugador: 1-> porter 2-> defensa 3->migcapista 4->davanter
	JugaDe=2,
	-- Característiques generals: Força màxima i Massa
	Forca = 20 ,  Massa = 5, 
	
	-- Velocitat màxima (amb pilota, sense pilota, de rotació
	VelocitatAmbPilota = 6, VelocitatSensePilota = 4, Rotacio = 0.3, 
	
	-- Xuts
	ForcaXut=10, Punteria = 0.9,  PossibleXutBoig = 0.05, 
	
	-- Passades (ZonaComfort **)
	ForcaPassada = 8,  MinimaDistanciaPassada = 120, EsperaOHiVa=0.5, ZonaComfort=132,
	
	-- Dribblings
	ForcaDribbling=3, AtacantsGirenSiSonA = 475,
	
	-- Posicions de defensa i atac
	DefensaX = 1003, DefensaY = 345, AtacX = 500, AtacY = 344
}

Jugador4 = { 
	Nom = "Pau4", Numero = 5, Imatge = "groc.lua", 
	-- Per les animacions
	Animacions = 2,	
	-- De què juga el jugador: 1-> porter 2-> defensa 3->migcapista 4->davanter
	JugaDe=3,
	-- Característiques generals: Força màxima i Massa
	Forca = 20 ,  Massa = 5, 
	
	-- Velocitat màxima (amb pilota, sense pilota, de rotació
	VelocitatAmbPilota = 6, VelocitatSensePilota = 4, Rotacio = 0.3, 
	
	-- Xuts
	ForcaXut=10, Punteria = 0.9,  PossibleXutBoig = 0.05, 
	
	-- Passades (ZonaComfort **)
	ForcaPassada = 8,  MinimaDistanciaPassada = 120, EsperaOHiVa=0.5, ZonaComfort=132,
	
	-- Dribblings
	ForcaDribbling=3, AtacantsGirenSiSonA = 475,
	
	-- Posicions de defensa i atac
	DefensaX = 850, DefensaY = 247, AtacX = 412, AtacY = 247
}

Jugador5 = { 
	Nom = "Pau5", Numero = 6, Imatge = "groc.lua", 
	-- Per les animacions
	Animacions = 2,	
	-- De què juga el jugador: 1-> porter 2-> defensa 3->migcapista 4->davanter
	JugaDe=3,
	-- Característiques generals: Força màxima i Massa
	Forca = 20 ,  Massa = 5, 
	
	-- Velocitat màxima (amb pilota, sense pilota, de rotació
	VelocitatAmbPilota = 6, VelocitatSensePilota = 4, Rotacio = 0.3, 
	
	-- Xuts
	ForcaXut=10, Punteria = 0.9,  PossibleXutBoig = 0.05, 
	
	-- Passades (ZonaComfort **)
	ForcaPassada = 8,  MinimaDistanciaPassada = 120, EsperaOHiVa=0.5, ZonaComfort=132,
	
	-- Dribblings
	ForcaDribbling=3, AtacantsGirenSiSonA = 475,
	
	-- Posicions de defensa i atac
	DefensaX = 850, DefensaY = 416, AtacX = 412, AtacY = 416
}

Jugador6 = { 
	Nom = "Pau6", Numero = 7, Imatge = "groc.lua", 
	-- Per les animacions
	Animacions = 2,	
	-- De què juga el jugador: 1-> porter 2-> defensa 3->migcapista 4->davanter
	JugaDe=4,
	-- Característiques generals: Força màxima i Massa
	Forca = 20 ,  Massa = 5, 
	
	-- Velocitat màxima (amb pilota, sense pilota, de rotació
	VelocitatAmbPilota = 6, VelocitatSensePilota = 4, Rotacio = 0.3, 
	
	-- Xuts
	ForcaXut=10, Punteria = 0.9,  PossibleXutBoig = 0.05, 
	
	-- Passades
	ForcaPassada = 8,  MinimaDistanciaPassada = 120, EsperaOHiVa=0.5, ZonaComfort=132,
	
	-- Dribblings
	ForcaDribbling=3, AtacantsGirenSiSonA = 475,
	
	-- Posicions de defensa i atac
	DefensaX = 700, DefensaY = 247, AtacX = 127, AtacY = 344
}

Jugador7 = { 
	Nom = "Pau7", Numero = 8, Imatge = "groc.lua", 
	-- Per les animacions
	Animacions = 2,	
	-- De què juga el jugador: 1-> porter 2-> defensa 3->migcapista 4->davanter
	JugaDe=4,
	-- Característiques generals: Força màxima i Massa
	Forca = 20 ,  Massa = 5, 
	
	-- Velocitat màxima (amb pilota, sense pilota, de rotació
	VelocitatAmbPilota = 6, VelocitatSensePilota = 4, Rotacio = 0.3, 
	
	-- Xuts
	ForcaXut=10, Punteria = 0.9,  PossibleXutBoig = 0.05, 
	
	-- Passades (ZonaComfort **)
	ForcaPassada = 8,  MinimaDistanciaPassada = 120, EsperaOHiVa=0.5, ZonaComfort=132,
	
	-- Dribblings
	ForcaDribbling=3, AtacantsGirenSiSonA = 475,
	
	-- Posicions de defensa i atac
	DefensaX = 700, DefensaY = 416, AtacX = 137, AtacY = 500
}

-- Pel càlculs de punts de suport
PuntsSuport = { x = 10, y = 6 }
PuntsSweet  = { x = 13, y = 6 }
