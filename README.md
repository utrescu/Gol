# Gol

Es tracta d'un antic joc de futbol que vaig fer fa uns quants anys.

En realitat dir-li joc és ser molt generós perquè els jugadors juguen sols i no s'hi pot interactuar però ...

## Estratègies implementades

Hi ha dos tipus d'estratègies:

- d'Equip:
  - Tornar a casa (per quan fan gols i al principi)
  - Defensa (si perden la possessió de la pilota, els jugadors tornen a les seves posicions defensives i actuen en conseqüència)
  - Atac (els jugadors se'n van endavant quan aconsegueixen la posició de la pilota)
- de Jugadors
  - Si tenen la possessió de la pilota hi ha un jugador que va a la posició d'atac més bona (calculada d'un conjunt de punts)
  - Si algú creu que està millor que el que té la pilota li demana que li passi la pilota
  - El que té la pilota si no pot passar o xutar, dribbla o condueix endavant
  - Si un jugador és el més proper a la pilota la intenta aconseguir

## Generar el programa

Primer s'ha de generar el fitxer configure:

```bash
./autogen.sh
```

Ho podria haver deixat fet, però he optat per tenir el directori el més net possible

### Requeriments

Bàsicament necessita les llibreries gràfiques de SDL i

- SDL 1.2
- SDL_ttf
- SDL_image
- SDL_gfx
- lua

### Configure the software

Si el procés anterior ha funcionat, ja es pot compilar el codi. Però abans s'ha de
comprovar que es té tot el programari: compilador, llibreries, etc.. que fan falta.
L'script `configure` és qui se n'encarrega d'aquesta tasca

```bash
./configure
```

Si tot acaba bé es crearà automàticament un fitxer Makefile

```bash
...
checking for sqrt... no
checking that generated files are newer than configure... done
configure: creating ./config.status
config.status: creating Makefile
config.status: creating src/Makefile
config.status: creating config.h
config.status: executing depfiles commands
config.status: executing libtool commands
$
```

### Compilar

El procés de compilació i d'execució es fa a través de Make, per tant només cal:

```bash
make
```

Al acabar hi haurà l'executable en el directori `src`:

```bash
$ ls src/gol
src/gol
```

## TODO

FALTA PER FER

- El jugador que ha de controlar l'humà encara no està fet.
- Falten molts gràfics per fer, (en les fores de banda fan servir l'animació del xut i els porters no s'estiren mai ... )
  I si trobo algun artista fer gràfics nous.
- Falta implementar els xuts i passes alts i el cops de cap
- No estan fets els córners
- Falten les estirades del porter
- No es compta el temps. O sigui que pot durar eternament (és el millor per fer proves).
- Falta fer un editor per col·locar els jugadors abans del partit (amb la IA actual la col·locació és molt important). Els jugadors haurien de mostrar unes característiques específiques.
- Falta afegir el sistema de faltes
- Millorar les estratègies
- He vist que pels 1-to-1 amb el jugador humà (player) seria bo implementar un sistema d'estadístiques N-Gram

POSSIBLES PROBLEMES O PROBLEMES

- Els porters són ultra bons, és difícil fer-los gols (però a vegades els entra una "pàjara" o la caguen en el passe i els hi colen cada cosa ...
- A vegades en les sacades de fores passen coses rares, encara estic buscant-ho.
  Ara em sembla que falla algunes vegades quan el sacador de banda, no entra la pilota al camp
- Quan un equip perd la pilota, passa a estratègia defensiva, i crec que això fa que perdin la possibilitat de fer rebots. La meva idea és afegir una estratègia d'equip ( migcampisme ) que faci la transició més lenta.
- Falta afegir-ne alguna més, especialment defensives, però ja ara marquen poc ... Suposo que els xuts alts em donaran més possibilitats.
