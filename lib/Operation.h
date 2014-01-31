/**
 ******************************************************************************* 
 * @file Operation.h
 *  @brief Operation Class header file
 * 
 *  Operation contains a list of Access to perform
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

#ifndef OPERATION_CALOE_H
#define OPERATION_CALOE_H
 
#include "Access.h"

#include <vector>

using namespace std;

namespace caloe {

/** @brief Contains a list of Access **/

class Operation {
	private:
	
		/// Operation name
		
		string name;
		
		/// Operation Docstring
		
		string doc;
		
		/// Access list
		
		vector< Access > list_access;
		
		/// Needed parameter of each access
		
		vector < ParamConfig > list_param;

	public:
		
		/**@brief Operation default constructor **/
		
		Operation();
		
		/**@brief Operation constructor with arguments
		 * 
		 * @param name Operation name
		 * 
		 * @param doc Operation docstring
		 * 
		 */
		 
		Operation(string name, string doc);
		
		/** @brief Operation constructor from another Operation instance 
		 *
		 *  @param op Instance to copy
		 **/
		 
		Operation(const Operation & op);
		
		/** @brief Operation Asignment operator 
		 *
		 *  @param op Intance to copy
		 * 
		 *  @return New Operation instance 
		 **/
		 
		Operation operator=(const Operation & op);
		
		/** @brief Get Operation name
		 * 
		 *  @return Operation name
		 */
		 
		string getName() const;
		
		/** @brief Get Operation docstring
		 * 
		 *  @return Operation docstring
		 */
		 
		string getDoc() const;
		
		/** @brief Set Operation name 
		 * 
		 * @param name Operation name
		 */
		 
		void setName(string name);
		
		/** @brief Set Operation docstring 
		 * 
		 * @param doc Operation docstring
		 */
		 
		void setDoc(string doc);
		
		/** @brief Add new Access to Operation 
		 * 
		 * @param access Access to add
		 * 
		 * @param param Specify if Access needs any parameter from user
		 * 
		 */
		 
		void addAccess(const Access & access, const ParamConfig & param);
		
		/** @brief Reset all accesses of the operation (for autoincrement/decrement accesses, it restores initial address)
		 * 
		 **/
		 
		void reset();
		
		/** @brief Execute an Operation 
		 * 
		 * @param params Needed user parameters
		 * 
		 * @return Read operation values
		 */
		 
		vector<eb_data_t> execute(ParamOperation & params);
		
		/** @brief Load an Operation from input configuration file
		 * 
		 * @param file Input stream asociated to configuration file
		 *
		 */
		 
		void loadOperationCfgFile(ifstream & file);
		
		/** @brief Print Operation information
		 * 
		 *  @param os Output stream
		 * 
		 *  @param op Operation instance to print
		 * 
		 *  @return Updated output stream
		 */
		 
		friend ostream & operator<<(ostream & os, Operation & op);
		
		/** @brief Fill Operation information from input stream
		 * 
		 *  @param is Input stream
		 * 
		 *  @param op Operation instance to fill
		 * 
		 *  @return Updated input stream
		 */
		 
		friend istream & operator>>(istream & is, Operation & op);
		
		/** @brief Operation destructor **/
		
		~Operation();
};
}

#endif
