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
const int MININT = (std::numeric_limits<int>::min)();
const int MAXINT = (std::numeric_limits<int>::max)();
const double MAXDOUBLE = (std::numeric_limits<double>::max)();
const double MINDOUBLE = (std::numeric_limits<double>::min)();
const float MAXIMFLOAT = std::numeric_limits<float>::max();
const float MINFLOAT = (std::numeric_limits<float>::min)();
// Constants numèriques
const double PI = 3.14159;
const double DOSPI = PI * 2;
const double PIMITJOS = PI / 2;
const double PIQUARTS = PI / 4;
// Perquè em fa falta per representar el jugador
// M'estalvio una divisió flotant en cada loop...
const double QUATREDIVPI = 4 / PI;

const double FREGAMENT = -0.1;
const double DOSFREGAMENT = FREGAMENT * FREGAMENT;
const double GRAVETAT = -10.0;

const double PAS = 0.1;
