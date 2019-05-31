AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = $(ACLOCAL_FLAGS)

bin_PROGRAMS = gol

gol_SOURCES = gol.cpp Joc.cpp Pantalla.cpp Pilota.cpp Porteria.cpp Jugador.cpp \
				Frames.cpp Marcador.cpp Animacio.cpp Equip.cpp Entitat.cpp \
				Entitatmobil.cpp Jugadorbase.cpp Porter.cpp steeringbehaviors.cpp \
				missatger.cpp EntitatsGestor.cpp JugadorEstats.cpp \
				PorterEstats.cpp EquipEstats.cpp Regio.cpp FrameCounter.cpp \
			    CalculadoraPuntsSuport.cpp LUAManager.cpp Renderator.cpp filepath.cpp

# set the include path found by configure
AM_CPPFLAGS =  $(LIBSDL_CFLAGS)  @LUA_INCLUDE@ $(all_includes) -DDATADIR=\"$(datadir)\"

# the library search path.
gol_LDFLAGS = $(all_libraries) $(LIBSDL_RPATH) -llua
gol_LDADD = $(LIBSDL_LIBS)
noinst_HEADERS = Joc.h Pantalla.h Pilota.h Porteria.h Jugador.h Animacio.h \
				 Equip.h Entitat.h general.h Entitatmobil.h punt3.h estat.h Missatge.h \
				 Autolist.h maquinaestats.h Jugadorbase.h Porter.h steeringbehaviors.h \
				 missatger.h EntitatsGestor.h JugadorEstats.h PorterEstats.h EquipEstats.h \
                 Regio.h JugadorMissatges.h FrameCounter.h Regulator.h filepath.h \
				 CalculadoraPuntsSuport.h C2DMatrix.h PorterMissatges.h constants.h
