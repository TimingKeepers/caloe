 # ****************************************************************************** 
 # @file Makefile
 #  @brief CALoE Library Makefile. It builds Dio device.
 #
 #  Copyright (C) 2013
 #
 #  CALoE library makefile.
 #
 #  @author Miguel Jimenez Lopez <klyone@ugr.es>
 #
 #  @bug none!
 #
 # ******************************************************************************
 #  This library is free software; you can redistribute it and/or
 #  modify it under the terms of the GNU Lesser General Public
 #  License as published by the Free Software Foundation; either
 #  version 3 of the License, or (at your option) any later version.
 #
 #  This library is distributed in the hope that it will be useful,
 #  but WITHOUT ANY WARRANTY; without even the implied warranty of
 #  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 #  Lesser General Public License for more details.
 #  
 #  You should have received a copy of the GNU Lesser General Public
 #  License along with this library. If not, see <http//www.gnu.org/licenses/>.
 # ******************************************************************************
 

all: Dio.o

Dio.o:
	@echo "dio: Building Dio device..."
	@g++ -c -o Dio.o Dio.cpp

clean:
	@echo "dio: Cleanup..."
	@-rm *.o *~

