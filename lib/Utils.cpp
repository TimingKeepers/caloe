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
	string::iterator it;
	string s2;
	
	for(it = s.begin() ; it != s.end() ; it++) {
		if(*it != del) {
			s2.push_back(*it);
		}
		else {
			s_split.push_back(s2);
			s2.clear();
		}
	}
	
	if(!s2.empty())
		s_split.push_back(s2);
	
	return s_split;
}

string join(vector<string> vs) {
	vector<string>::iterator it;
	string::iterator it2;
	string s;
	
	for(it = vs.begin() ; it != vs.end() ; it++) {
		string s2 = *it;
		for(it2 = s2.begin() ; it2 != s2.end() ; it2++) {
			s.push_back(*it2);
		}
	}
	
	return s;
}

}
