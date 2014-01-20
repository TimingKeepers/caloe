 # /**
 # ******************************************************************************* 
 # * @file Makefile
 # *  @brief Caloe Project Makefile
 # *
 # *  Copyright (C) 2013
 # *
 # *  @author Miguel Jimenez Lopez <klyone@ugr.es>
 # *
 # *  @bug ---
 # *
 # *******************************************************************************
 # *  This library is free software; you can redistribute it and/or
 # *  modify it under the terms of the GNU Lesser General Public
 # *  License as published by the Free Software Foundation; either
 # *  version 3 of the License, or (at your option) any later version.
 # *
 # *  This library is distributed in the hope that it will be useful,
 # *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 # *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 # *  Lesser General Public License for more details.
 # *  
 # *  You should have received a copy of the GNU Lesser General Public
 # *  License along with this library. If not, see <http://www.gnu.org/licenses/>.
 # *******************************************************************************
 
all: libcaloe.a

Netcon.o: Netcon.h Netcon.cpp
	@echo "lib: Compiling Netcon..."
	@g++ -g -o Netcon.o -c Netcon.cpp

Access.o: Access.h Access.cpp Netcon.h Netcon.cpp Parameters.cpp Parameters.h
	@echo "lib: Compiling Access..."
	@g++ -g -o Access.o -c Access.cpp

Operation.o: Operation.h Operation.cpp Access.h Access.cpp
	@echo "lib: Compiling Operation..."
	@g++ -g -o Operation.o -c Operation.cpp
	
Device.o: Device.h Device.cpp Operation.h Operation.cpp
	@echo "lib: Compiling Device..."
	@g++ -g -o Device.o -c Device.cpp
	
System.o: System.h System.cpp Device.h Device.cpp
	@echo "lib: Compiling System..."
	@g++ -g -o System.o -c System.cpp
	
Parameters.o: Parameters.h Parameters.cpp Utils.h Utils.cpp
	@echo "lib: Compiling Parameter..."
	@g++ -g -o Parameters.o -c Parameters.cpp
	
Utils.o: Utils.h Utils.cpp
	@echo "lib: Compiling Utils..."
	@g++ -g -o Utils.o -c Utils.cpp
	
access_internals.o: access_internals.h access_internals.c
	@echo "lib: Compiling access_internals..."
	@gcc -o access_internals.o -c access_internals.c
	
libcaloe.a: access_internals.o Netcon.o Utils.o Parameters.o Access.o Operation.o Device.o System.o 
	@echo "lib: Generating libcaloe..."
	@ar rs libcaloe.a access_internals.o Netcon.o Utils.o Parameters.o Access.o Operation.o Device.o System.o 
	
clean:
	@echo "lib: Cleanup..."
	@-rm *.o *~ *.a
