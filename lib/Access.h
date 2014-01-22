/**
 ******************************************************************************* 
 * @file Access.h
 *  @brief Access Class Header File
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

#ifndef ACCESS_CALOE_H
#define ACCESS_CALOE_H
 
#include "Netcon.h"
#include "Parameters.h"
#include <fstream>

#include "access_internals.h"

using namespace std;

namespace caloe {

/**@brief Wrapper of access_caloe internals **/

class Access {
	private:
		
		/// Init Memory address to access
		
		eb_address_t address_init;
		
		/// Memory address to access
		
		eb_address_t address; 
		
		/// Offset to be applied to address
		
		eb_address_t offset; 
		
		/// Value to write or read value
		
		eb_data_t value; 
		
		/// Mask to be applied to value field
		
		eb_data_t mask; 
		
		/// Mask operation to apply (OR/AND)
		
		mask_oper_caloe mask_oper;
		
		/// Indicate if it is Etherbone configuration space (true) or not (false)
		
		bool is_config; 
		
		/// Access mode (READ, WRITE, SCAN, READ_WRITE)
		
		access_type_caloe mode;
		
		/// Data Align (1B, 2B, 4B, 8B)
		
		align_access_caloe align;
		
		/// Autoincrement/decrement for address field
		
		int autoincr;
		
		/// Network connection parameters
		
		Netcon networkc;

	public:
	
		/**@brief Access default constructor **/
		
		Access();
		
		/**@brief Access constructor with arguments **/
		
		Access(eb_address_t address, eb_address_t address_init, eb_address_t offset, eb_data_t value, eb_data_t mask, mask_oper_caloe mask_oper, bool is_config, access_type_caloe mode, align_access_caloe align,int autoincr, Netcon networkc);
		
		/** @brief Access constructor from another Access instance 
		 *
		 *  @param access Access to be copied 
		 **/
		 
		Access(const Access & access);
		
		/** @brief Access Asignment operator 
		 *
		 *  @param access Intance to be copied
		 * 
		 *  @return Access reference of new instance 
		 **/
		 
		Access operator=(const Access & access);
		
		/** @brief Get Init Memory address **/
		
		eb_address_t getAddressInit() const;
		
		/** @brief Get Memory address **/
		
		eb_address_t getAddress() const;
		
		/** @brief Get Offset **/
		
		eb_address_t getOffset() const;
		
		/** @brief Get Value **/
		
		eb_data_t getValue() const;
		
		/** @brief Get Mask **/
		
		eb_data_t getMask() const;
		
		/** @brief Get Operation mask **/
		
		mask_oper_caloe getMaskOper() const;
		
		/** @brief Check if access is refered to Etherbone configuration space **/
		
		bool getIsConfig() const;
		
		/** @brief Get Access mode **/
		
		access_type_caloe getMode() const;
		
		/** @brief Get Data align **/
		
		align_access_caloe getAlign() const;
		
		/** @brief Get autoincrement/decrement for access **/
		
		int getAutoincr() const;
		
		/** @brief Get network connection parameters **/
		
		Netcon getNetcon() const;
		
		/** @brief Set init memory address
		 * 
		 * @param address_init Init Memory address 
		 **/
		 
		void setAddressInit(eb_address_t address_init);
		
		/** @brief Set memory address
		 * 
		 * @param address Memory address 
		 **/
		 
		void setAddress(eb_address_t address);
		
		/** @brief Set Offset
		 * 
		 * @param offset Offset
		 **/
		 
		void setOffset(eb_address_t offset);
		
		/** @brief Set value
		 * 
		 * @param value Value
		 **/
		 
		void setValue(eb_data_t value);
		
		/** @brief Set mask
		 * 
		 * @param mask Mask
		 **/
		 
		void setMask(eb_data_t mask);
		
		/** @brief Set operation mask
		 * 
		 * @param mask_oper operation mask
		 **/
		 
		void setMaskOper(mask_oper_caloe mask_oper);
		
		/** @brief Mark if access is refered to Etherbone configuration space or not
		 * 
		 * @param is_config If is refered to Etherbone config space (true) or not (false)
		 **/
		 
		void setIsConfig(bool is_config);
		
		/** @brief Set access mode
		 * 
		 * @param mode Access mode
		 **/
		 
		void setMode(access_type_caloe mode);
		
		/** @brief Set data align
		 * 
		 * @param align Data align
		 **/
		 
		void setAlign(align_access_caloe align);
		
		/** @brief Set autoincrement/decrement parameter 
		 * 
		 * @param autoincr Autoincrement/decrement to address
		 **/
		 
		void setAutoincr(int autoincr);
		
		/** @brief Set network connection parameters
		 * 
		 * @param networkc Network connection parameters
		 **/
		 
		void setNetCon(Netcon networkc);
		
		/** @brief Reset access field
		 * 
		 **/
		 
		void reset();
		
		/** @brief Execute access
		 * 
		 * @return ALL_OK if success or error code otherwise
		 **/
		 
		int execute();
		
		/** @brief it loads an access from input configuration file
		 * 
		 * @param file Input stream
		 * 
		 * @return Needed parameters for access
		 **/
		 
		ParamConfig loadAccessCfgFile(ifstream & file);
		
		/** @brief Print Access information
		 * 
		 *  @param os Output stream
		 * 
		 *  @param access Access instance to print
		 * 
		 *  @return Updated output stream
		 */
		 
		friend ostream & operator<<(ostream & os, Access & access);
		
		/** @brief Fill Access information from input stream
		 * 
		 *  @param is Input stream
		 * 
		 *  @param access Access instance to be filled
		 * 
		 *  @return Updated input stream
		 */
		 
		friend istream & operator>>(istream & is, Access & access);
		
		/**@brief Access destructor **/
		
		~Access();
};
}

#endif
