/**
 ******************************************************************************* 
 * @file Utils.cpp
 *  @brief Implements useful common functions of Utils.h header file
 *
 *  Copyright (C) 2013
 *
 *  @author Miguel Jimenez Lopez <klyone@ugr.es>
 *
 *  @bug ---
 *
 *******************************************************************************
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *  
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *******************************************************************************
 */
 
#include "Utils.h"

namespace caloe {

vector<string> split(string s, char del) {
	vector<string> s_split;
	int it;
	int it2;
	int size = s.size();	

	for(it2 = it = 0 ; it < size ; it++) {
		// If you find del character, you can separate a new string
		if(s.at(it) == del) {
			// Get string
			string aux = s.substr(it2,it);
			
			// Add string
			s_split.push_back(aux);
			
			// Update iterators
			it2 = it+1;
		}
	}

	// Last string is special case
	string aux = s.substr(it2,it);
	
	// Add last string
	s_split.push_back(aux);

	return s_split;
}

}
