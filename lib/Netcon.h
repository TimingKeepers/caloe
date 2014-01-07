/**
 ******************************************************************************* 
 * @file Netcon.h
 *  @brief Network connection class header file
 * 
 *  - IP: IP netaddress
 *  - port: Network port
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

#ifndef NETCON_CALOE_H
#define NETCON_CALOE_H
 
#include <iostream>
#include <string>

using namespace std;

namespace caloe {

/** @brief Network connection parameters to connect with Etherbone server **/

class Netcon {
	private:
	
		/// IP netaddress
		
		string ip;
		
		/// Network port
		
		unsigned int port;

	public:
	
		/** @brief Netcon default constructor **/
		
		Netcon();
		
		/** @brief Netcon constructor with arguments 
		 *
		 *  @param ip Netaddress IP
		 * 
		 *  @param port Network port
		 *  
		 */
		 
		Netcon(string ip, unsigned int port);
		
		/** @brief Netcon constructor from another Netcon instance 
		 *
		 *  @param nc Instance to be copied 
		 **/
		 
		Netcon(const Netcon & nc);
		
		/** @brief Netcon Asignment operator 
		 *
		 *  @param nc Intance to be copied
		 * 
		 *  @return Netcon reference of new instance 
		 **/
		 
		Netcon operator=(const Netcon & nc);
		
		/** @brief Get IP netaddress **/
		
		string getIP() const;
		
		/** @brief Get network port **/
		
		unsigned int getPort() const;
		
		/** @brief Set IP netaddress
		 * 
		 * @param ip IP netaddress
		 */
		 
		void setIP(string ip);
		
		/** @brief Set network port
		 * 
		 * @param port Network port 
		 */
		 
		void setPort(unsigned int port);
		
		/** @brief Print Netcon information
		 * 
		 *  @param os Output stream
		 * 
		 *  @param nc Netcon instance to print
		 * 
		 *  @return Updated output stream
		 */
		 
		friend ostream & operator<<(ostream & os, Netcon & nc);
		
		/** @brief Fill Netcon information from input stream
		 * 
		 *  @param is Input stream
		 * 
		 *  @param nc Netcon instance to be filled
		 * 
		 *  @return Updated input stream
		 */
		 
		friend istream & operator>>(istream & is, Netcon & nc);
		
		/** @brief Netcon destructor **/
		
		~Netcon();
};

}

#endif
