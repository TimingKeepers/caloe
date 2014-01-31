/**
 ******************************************************************************* 
 * @file System.h
 *  @brief System class header file
 * 
 *  Top class in CALoE library. It contains several devices with their operations.
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

#ifndef SYSTEM_CALOE_H
#define SYSTEM_CALOE_H
 
#include "Device.h"

using namespace std;

namespace caloe {

/**
 * @brief Top class in CALoE library. It contains an hash table with all devices in your system. You must load their operation tables from input configuration files. 
 */

class System {
	private:
		/// Device hash table
		map<string,Device> list_device;

	public:
	
		/** @brief Default constructor **/
		
		System();
		
		/** @brief System constructor from other System instance 
		 *
		 *  @param sys Instance to copy
		 **/
		 
		System(const System & sys);
		
		/** @brief System Asignment operator 
		 *
		 *  @param sys Intance to copy
		 * 
		 *  @return New System instance 
		 **/
		 
		System operator=(const System & sys);
		
		/** @brief Add a new device to the system
		 * 
		 *  @param dev new Device to add
		 *
		 */
		 
		void addDevice(const Device & dev);
		
		/** @brief Reset an operation of a specific device (for autoincrement/decrement accesses, it restores initial address)
		 * 
		 * @param name_dev Device name
		 * 
		 * @param name_oper Operation name
		 *
		 */
		 
		void reset(string name_dev,string name_oper);
		
		/** @brief Execute an operation of one registered device in the system table
		 * 
		 * @param name_dev Device name
		 * 
		 * @param name_oper Operation name
		 * 
		 * @param params User needed parameters for Operation
		 * 
		 * @return A vector with all read data by the operation
		 */
		 
		vector<eb_data_t> execute(string name_dev, string name_oper, ParamOperation & params);
		
		/** @brief Load a device from an input configuration file and add it to the system table
		 * 
		 *  @param path Absolute/relative path of configuration file
		 * 
		 *  @param name_dev Device name
		 * 
		 */
		 
		void loadCfgFile(string path,string name_dev);
		
		/** @brief Print the system information
		 * 
		 *  @param os Output stream
		 * 
		 *  @param sys System instance to print
		 * 
		 *  @return Updated output stream
		 */
		 
		friend ostream & operator<<(ostream & os, System & sys);
		
		/** @brief Fill the system information from an input stream
		 * 
		 *  @param is Input stream
		 * 
		 *  @param sys System instance to fill
		 * 
		 *  @return Updated input stream
		 */
		 
		friend istream & operator>>(istream & is, System & sys);
		
		/** @brief System destructor **/
		
		~System();
};

}

#endif
