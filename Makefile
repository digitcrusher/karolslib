# Makefile
# Karolslib Source Code
# Available on Github
#
# Copyright (C) 2017 Karol "digitcrusher" Łacina
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
SD=./src
BD=./build
AR=ar
ARFLAGS=rcs
ARINCLUDE=
ARLIBS=/usr/lib/X11R6/lib/libX11.a
CC=g++
CCFLAGS=-Wall -std=c++11 -ggdb -O0
CCINCLUDE=./
CCLIBS=

all: clean karolslib

karolslib: karolslib.o math.o module.o terminal.o utils.o
	$(AR) $(ARFLAGS) $(BD)/libkarolslib.a $(BD)/karolslib.o $(BD)/math.o $(BD)/module.o $(BD)/terminal.o $(BD)/utils.o $(CCLIBS)

karolslib.o:
	$(CC) $(CCFLAGS) -I $(CCINCLUDE) $(SD)/karolslib.cpp -c -o $(BD)/karolslib.o

math.o:
	$(CC) $(CCFLAGS) -I $(CCINCLUDE) $(SD)/math.cpp -c -o $(BD)/math.o

module.o:
	$(CC) $(CCFLAGS) -I $(CCINCLUDE) $(SD)/module.cpp -c -o $(BD)/module.o

terminal.o:
	$(CC) $(CCFLAGS) -I $(CCINCLUDE) $(SD)/terminal.cpp -c -o $(BD)/terminal.o

utils.o:
	$(CC) $(CCFLAGS) -I $(CCINCLUDE) $(SD)/utils.cpp -c -o $(BD)/utils.o

clean:
	rm -f $(BD)/libkarolslib.a $(BD)/karolslib.o $(BD)/math.o $(BD)/module.o $(BD)/terminal.o $(BD)/utils.o
