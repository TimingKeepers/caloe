/**
 ******************************************************************************* 
 * @file Parameter.h
 *  @brief Parameter classes header file
 * 
 *  - ParamAccess: User parameters for an access
 *  - ParamOperation: User parameters for an operation (some accesses)
 *  - ParamConfig: Needed parameters for an access
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

#ifndef PARAMETERS_CALOE_H
#define PARAMETERS_CALOE_H
 
#include <vector>
#include <string>
#include <iostream>

#include <stdio.h>

using namespace std;

namespace caloe {

/// Access need not user parameters / User does not give anything

#define PARAM_NONE 0x00

/// Access needs mask from user / User gives mask parameter

#define PARAM_MASK 0x10

/// Access needs offset from user / User gives offset parameter 

#define PARAM_OFFSET 0x08

/// Access needs value from user / User gives value parameter

#define PARAM_VALUE 0x04

/// Access needs IP netaddress from user / User gives netaddress parameter

#define PARAM_NETADDRESS 0x02

/// Access needs port from user / User gives port parameter

#define PARAM_PORT 0x01

/**
 * @brief User parameters for Access
 */

class ParamAccess {
	private:
		
		/// It is used to specify parameters of Access given for user (see defines)
		
		char parameters;

		/// User netaddress IP
		
		string netaddress;
		
		/// User port
		
		unsigned int port;
		
		/// User offset (is an index to ParamConfig offset vector)
		
		unsigned int offset;
		
		/// User mask (is an index to ParamConfig mask vector)
		
		unsigned int mask;
		
		/// User value
		
		int value;

	public:
	
		/** @brief ParamAccess default constructor **/
		
		ParamAccess();
		
		/** @brief ParamAccess constructor from another ParamAccess instance 
		 *
		 *  @param vuart Instance to be copied 
		 **/
		 
		ParamAccess(const ParamAccess & pa);
		
		/** @brief ParamAccess Asignment operator 
		 *
		 *  @param pa Intance to be copied
		 * 
		 *  @return ParamAccess reference of new instance 
		 **/
		 
		ParamAccess operator=(const ParamAccess & pa);
		
		/** @brief Set IP netaddress parameter from user
		 *
		 *  @param netaddress user IP netaddress 
		 */
		 
		void setIP(string netaddress);
		
		/** @brief Set port parameter from user
		 *
		 *  @param port user port 
		 **/
		 
		void setPort(unsigned int port);
		
		/** @brief Set offset parameter from user
		 *
		 *  @param offset user offset (index)
		 **/
		 
		void setOffset(unsigned int offset);
		
		/** @brief Set mask parameter from user
		 *
		 *  @param mask user mask (index)
		 **/
		 
		void setMask(unsigned int mask);
		
		/** @brief Set value parameter from user 
		 *
		 *  @param value user value
		 **/
		 
		void setValue(int value);
		
		/** @brief Get user IP netaddress parameter **/
		 
		string getIP() const;
		
		/** @brief Get user port parameter **/
		
		unsigned int getPort() const;
		
		/** @brief Get user offset parameter (index) **/
		
		unsigned int getOffset() const;
		
		/** @brief Get user mask parameter (index) **/
		
		unsigned int getMask() const;
		
		/** @brief Get parameter metadata  **/
		
		char getParametersMask() const;
		
		/** @brief Get user value parameter **/
		
		int getValue() const;
		
		/** @brief Reset user parameter metadata **/
		
		void reset();
		
		/** @brief Print ParamAccess information
		 * 
		 *  @param os Output stream
		 * 
		 *  @param pa ParamAccess instance to print
		 * 
		 *  @return Updated output stream
		 */
		 
		friend ostream & operator<<(ostream & os, ParamAccess & pa);
		
		/** @brief Fill ParamAccess information from input stream
		 * 
		 *  @param is Input stream
		 * 
		 *  @param pa ParamAccess instance to be filled
		 * 
		 *  @return Updated input stream
		 */
		 
		friend istream & operator>>(istream & is, ParamAccess & pa);
		
		/** @brief ParamAccess destructor **/
		
		~ParamAccess();
	
};

/**
 * @brief User parameters for Operation. It contains a ParamAccess vector
 */
 
class ParamOperation {
	private:
		
		/// ParamAccess foreach Access of Operation
		
		vector <ParamAccess> parameter_values;
	
	public:
	
		/** @brief ParamOperation default constructor **/
		
		ParamOperation();
		
		/** @brief ParamOperation constructor from another ParamOperation instance 
		 *
		 *  @param po Instance to be copied 
		 **/
		 
		ParamOperation(const ParamOperation & po);
		
		/** @brief ParamOperation Asignment operator 
		 *
		 *  @param po Intance to be copied
		 * 
		 *  @return ParamOperation reference of new instance 
		 **/
		 
		ParamOperation operator=(const ParamOperation & po);
		
		/** @brief Add new user parameter to ParamOperation
		 * 
		 *  @param pa ParamAccess to be added
		 * 
		 *  @note You must reset ParamAccess if you used it before
		 * 
		 */
		 
		void addParameter(const ParamAccess & pa);
		
		/** @brief Get ParamAccess vector for an Operation **/
		 
		vector<ParamAccess> getParamAccess() const;
		
		/** @brief Print ParamOperation information
		 * 
		 *  @param os Output stream
		 * 
		 *  @param po ParamOperation instance to print
		 * 
		 *  @return Updated output stream
		 */
		 
		friend ostream & operator<<(ostream & os, ParamOperation & po);
		
		/** @brief Fill ParamOperation information from input stream
		 * 
		 *  @param is Input stream
		 * 
		 *  @param po ParamOperation instance to be filled
		 * 
		 *  @return Updated input stream
		 */ 
		 
		friend istream & operator>>(istream & is, ParamOperation & po);
		
		/** @brief ParamOperation destructor **/
		
		~ParamOperation();
};

/**
 * @brief Needed parameters for Access. User must fill ParamAccess with these parameter to execute Access
 */
 
class ParamConfig {
	private:
	
		/// It is used to specify needed parameters of Access (see defines)
		
		char parameters;

		/// Offset vector (user can choose one of them with an index in ParamAccess)
		
		vector<int> offsets;
		
		/// Mask vector (user can choose one of them with an index in ParamAccess)
		
		vector<int> masks;
		
		/// Split method (used to parse offset/mask vector from input configuration file)
		vector<string> split(string s, char del);
		
	public:
	
		/** @brief ParamConfig default constructor **/
		
		ParamConfig();
		
		/** @brief ParamConfig constructor from another ParamConfig instance 
		 *
		 *  @param pc Instance to be copied 
		 **/
		 
		ParamConfig(const ParamConfig & pc);
		
		/** @brief ParamConfig Asignment operator 
		 *
		 *  @param pc Intance to be copied
		 * 
		 *  @return ParamConfig reference of new instance 
		 **/
		 
		ParamConfig operator=(const ParamConfig & pc);
		
		/** @brief Set Offset vector
		 *  
		 *  @param offsets Offset vector for Access
		 */
		 
		void setOffsetsParam(vector<int> offsets);
		
		/** @brief Set mask vector
		 *  
		 *  @param masks Mask vector for Access
		 */
		 
		void setMasksParam(vector<int> masks);
		
		/** @brief Set mask vector from string
		 *  
		 *  @param masks Mask vector for Access
		 */
		
		void setMasksString(string masks);
		
		/** @brief Set Offset vector from string
		 *  
		 *  @param offsets Offset vector for Access
		 */
		 
		void setOffsetsString(string offsets);
		
		/** @brief Set IP netaddress parameter **/
		
		void setIPParam();
		
		/** @brief Set value parameter **/
		
		void setValueParam();
		
		/** @brief Set port parameter **/
		
		void setPortParam();
		
		/** @brief Get ParamConfig metadata **/
		 
		char getParametersMask() const;
		
		/** @brief Get Offset vector **/
		
		vector<int> getOffsetsParam();
		
		/** @brief Get Mask vetor **/
		
		vector<int> getMasksParam();
		
		/** @brief Print ParamConfig information
		 * 
		 *  @param os Output stream
		 * 
		 *  @param pc ParamConfig instance to print
		 * 
		 *  @return Updated output stream
		 */
		 
		friend ostream & operator<<(ostream & os, ParamConfig & pc);
		
		/** @brief Fill ParamConfig information from input stream
		 * 
		 *  @param is Input stream
		 * 
		 *  @param pc ParamConfig instance to be filled
		 * 
		 *  @return Updated input stream
		 */
		 
		friend istream & operator>>(istream & is, ParamConfig & pc);
		
		/** @brief ParamConfig destructor **/
		
		~ParamConfig();
};

}

#endif
