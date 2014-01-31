/**
 ******************************************************************************* 
 * @file Device.h
 *  @brief Device Class Header File
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

#ifndef DEVICE_CALOE_H
#define DEVICE_CALOE_H
 
#include "Operation.h"

#include <map>
#include <utility> //pair

using namespace std;

namespace caloe {

/** @brief Contains an hash table of the operation asociated with the device **/

class Device {
	private:
	
		/// Device name
		
		string name;
		
		/// Operation hash table
		
		map<string,Operation> list_operation;

	public:
	
		/**@brief Device default constructor **/
		
		Device();
		
		/** @brief Device constructor from other Device instance 
		 *
		 *  @param dev Instance to copy 
		 **/
		 
		Device(const Device & dev);
		
		/** @brief Device Asignment operator 
		 *
		 *  @param dev Intance to copy
		 * 
		 *  @return New Device instance 
		 **/
		 
		Device operator=(const Device & dev);
		
		/** @brief Get Device name **/
		
		string getName() const;
		
		/** @brief Set Devie name
		 * 
		 * @param name Device name
		 */
		 
		void setName(string name);
		
		/** @brief Add new operation to the device table 
		 * 
		 * @param op New operation to add
		 */
		 
		void addOperation(const Operation & op);
		
		/** @brief Reset an operation asociated to the device
 		 * 
 		 * @param name Operation name
 		 * 
		 */
		 
		void reset(string name);
		
		/** @brief Execute an operation asociated to the device
 		 * 
 		 * @param name Operation name
 		 * 
 		 * @param params User parameters for the operation
 		 * 
 		 * @return a vector with read values by operation
 		 * 
		 */
		 
		vector<eb_data_t> execute(string name,ParamOperation & params);
		
		/** @brief Load a device from the input configuration file
		 *  
		 * @param path absolute/relative path of the configuration file
		 * 
		 * @param name_dev Device name
		 * 
		 */
		 
		void loadCfgFile(string path,string name_dev);
		
		/** @brief Print Device information
		 * 
		 *  @param os Output stream
		 * 
		 *  @param dev Device instance to print
		 * 
		 *  @return Updated output stream
		 */
		 
		friend ostream & operator<<(ostream & os, Device & dev);
		
		/** @brief Fill Device information from the input stream
		 * 
		 *  @param is Input stream
		 * 
		 *  @param nc Device instance to fill
		 * 
		 *  @return Updated input stream
		 */
		 
		friend istream & operator>>(istream & is, Device & dev);
		
		/**@brief Device destructor **/
		
		~Device();
};

}

#endif
