/**
 ******************************************************************************* 
 * @file Utils.h
 *  @brief Useful common functions for CALoE library
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

#ifndef UTILS_CALOE_H
#define UTILS_CALOE_H

#include <vector>
#include <string>

using namespace std;

namespace caloe {

/** @brief Splits a string into several substrings based on a separator character
 *
 *  @param s String to be split
 *  @param del Separator character
 *
 *  @return A String vector that contains all found substrings
*/
vector<string> split(string s, char del);

/** @brief Joins a string in several strings from a vector
 *
 *  @param vs String vector to be joined
 *
 *  @return string with all substrings joined
*/
string join(vector<string> vs);

}

#endif
