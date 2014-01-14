 # ****************************************************************************** 
 # @file Makefile
 #  @brief CALoE Library Makefile. It builds CALoE library and examples.
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
 

all: etherbone-submodule build-etherbone build-lib build-devices build-tools end

etherbone-submodule: 
	@echo "main: SUBMODULE Etherbone..."
	@git submodule init
	@git submodule update

build-etherbone: etherbone
	@echo "main: Building Etherbone library..."
	@make -C ./etherbone/api

build-devices: 
	@make -C ./devices

build-tools:
	@make -C ./tools

build-lib:
	@make -C ./lib

build-doxygen:
	@make -C ./doxygen

end:
	@echo "main: Compilation terminated successfully"

clean:
	@echo "main: Cleanup..."
	@-rm *~
	@make -C ./devices clean
	@make -C ./tools clean
	@make -C ./lib clean
	@make -C ./doxygen clean
	@make -C ./etherbone/api clean
	@make -C ./doc clean
