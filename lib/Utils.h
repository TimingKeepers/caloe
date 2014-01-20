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

/** @brief Splits a string into a number based on a separator character
 *
 *  @param s String to split
 *  @param del Separator character
 *
 *  @return A vector of separated string based on separator character
*/
vector<string> split(string s, char del);

/** @brief Join into a string several strings from a vector
 *
 *  @param vs Vector to join
 *
 *  @return All joined string
*/
string join(vector<string> vs);

}

#endif
