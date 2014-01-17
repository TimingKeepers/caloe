/**
 ******************************************************************************* 
 * @file Vuart.h
 *  @brief Vuart class header file
 * 
 *  Contains high-level operations of Vuart device.
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
 
#include "../../lib/Device.h"

#include <iostream>
#include <unistd.h>

using namespace std;
using namespace caloe;

/**
 * @brief This class allows to access uart terminal with Etherbone UDP packets in UDP|TCP/IP network 
 */
 
class Vuart {
	private:
		/// Device operations
		Device vuart;

	public:
		/** @brief Vuart Default constructor **/
		
		Vuart();
		
		/** @brief Vuart constructor from another Vuart instance 
		 *
		 *  @param vuart Instance to be copied 
		 **/
		 
		Vuart(const Vuart & vuart);
		
		/** @brief Vuart constructor with configuration file path
		 *
		 *  @param config_path configuration file path
		 **/

		Vuart(string config_path);
		
		/** @brief Vuart Asignment operator 
		 *
		 *  @param vuart Intance to be copied
		 * 
		 *  @return Vuart reference of new instance 
		 **/
		 
		Vuart & operator=(const Vuart & vuart);
		
		/** @brief Prints vuart operation table **/
		
		void print();
		
		/** @brief Check if vuart is ready to be written 
		 *
		 *  @param ip Etherbone server Netaddress IP 
		 * 
		 *  @return it returns true if Vuart is ready to write again or false otherwise
		 *
		 **/
		
		bool isReady(string ip);
		
		/** @brief Read a character from vuart 
		 * 
		 *  @param ip Etherbone server Netaddress IP 
		 * 
		 *  @param valid It used to confirm if returned value is valid or not
		 * 
		 *  @return Read character from vuart
		 **/
		
		char read(string ip, bool & valid);
		
		/** @brief Write a character to vuart 
		 *
		 *  @param ip Etherbone server Netaddress IP 
		 * 
		 *  @param value Value to write in vuart
		 * 
		 **/
		
		void write(string ip, char value);
		
		/**  @brief Read a data string from vuart
		 * 
		 *   @param ip Etherbone server Netaddress IP
		 * 
		 *   @param period Number of usecs to wait between character read (0: none)
		 * 
		 *   @return a data string from vuart
		 * 
		 **/
		 
		string readString(string ip,unsigned long period);
		
		/**  @brief Write a data string to vuart
		 * 
		 *   @param ip Etherbone server Netaddress IP 
		 * 
		 *   @param s Data string to write
		 * 
		 *   @param period Number of usecs to wait between character write (0: none)
		 * 
		 **/
		 
		void writeString(string ip, string s,unsigned long period);
		
		/**  @brief Flush virtual uart buffer
		 * 
		 *   @param ip Etherbone server Netaddress IP 
		 * 
		 **/
		
		void flush(string ip);
		
		/**  @brief Execute one virtual uart command and wait for response
		 * 
		 *   @param ip Etherbone server Netaddress IP 
		 * 
		 *   @param s cmd Command to execute
		 * 
		 *   @return Command results in string variable
		 * 
		 **/
		
		string execute_cmd(string ip, string cmd);
		
		/** @brief Vuart destructor **/
		
		~Vuart();
};
