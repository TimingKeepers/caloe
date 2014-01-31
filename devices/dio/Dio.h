/**
 ******************************************************************************* 
 * @file Dio.h
 *  @brief Dio high-level operation header file
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

using namespace std;
using namespace caloe;

/** @brief High-level device for DIO **/

class Dio {
	private:
	
		/// Device for Dio (contains low-level operations)
		Device dio;

		/** @brief Configure a Dio channel as output
		 *
		 *  @param ip IP netaddress
		 * 
		 *  @param ch Index of Dio channel 
		 */
		 
		void configChOutput(string ip, int ch);
		
		/** @brief Configure a Dio channel as input
		 *
		 *  @param ip IP netaddress
		 * 
		 *  @param ch Index of Dio channel 
		 */
		 
		void configChInput(string ip, int ch);
		
		/** @brief Configure a Dio channel with resistor termination
		 *
		 *  @param ip IP netaddress
		 * 
		 *  @param ch Index of Dio channel 
		 */
		 
		void configChResistor(string ip, int ch);
		
		/** @brief Configure a Dio channel without resistor termination
		 *
		 *  @param ip IP netaddress
		 * 
		 *  @param ch Index of Dio channel 
		 */
		 
		void configChnResistor(string ip, int ch);

	public:
		
		/**@brief Dio default constructor **/
		
		Dio();
		
		/** @brief Dio constructor from other Dio instance 
		 *
		 *  @param dio Instance to copy
		 **/
		 
		Dio(const Dio & dio);

		/** @brief Dio constructor with configuration file path
		 *
		 *  @param config_path configuration file path
		 **/
		 
		Dio(string config_path);
		
		/** @brief Dio Asignment operator 
		 *
		 *  @param dio Intance to copy
		 * 
		 *  @return New Dio instance 
		 **/
		 
		Dio & operator=(const Dio & dio);
		
		/** @brief Print Dio operation table **/
		
		void print();
		
		/** @brief Scan Device memory map
		 * 
		 * @param ip IP Netaddress
		 * 
		 */
		 
		void scan(string ip);
		
		/** @brief Configure a DIO channel
		 *
		 *  @param ip IP netaddress
		 * 
		 *  @param ch Index of Dio channel
		 * 
		 *  @param mode Dio channel mode (i: Input without resistor, I: Input with resistor, d: output without resistor, D: output with resistor) 
		 */
		 
		void configCh(string ip,int ch, char mode);
		
		/** @brief Show Dio channel configuration
		 * 
		 *  @param ip IP Netaddress
		 */
		 
		void showConfigChs(string ip);
		
		/** @brief Generate an inmediate pulse in one channel
		 *  
		 * @param ip IP Netaddress
		 * 
		 * @param ch Index of Dio channel
		 * 
		 * @param len_pulse Pulse width (in cycles)
		 */
		 
		void pulseImm(string ip, int ch, int len_pulse);
		
		/** @brief Check if Dio is ready to generate other programmable pulse
		 * 
		 * @param ip IP Netaddress
		 * 
		 * @param ch Index of Dio channel
		 * 
		 * @return true if Dio is ready and false otherwise
		 */
		 
		bool isDioReady(string ip, int ch);
		
		/** @brief Generate a programmable pulse in one channel
		 *  
		 * @param ip IP Netaddress
		 * 
		 * @param ch Index of Dio channel
		 * 
		 * @param len_pulse Pulse width (in cycles)
		 * 
		 * @param t_trig time when pulse will be trigged (in seconds and nanoseconds)
		 */
		 
		void pulseProg(string ip, int ch, int len_pulse, timespec t_trig);
		
		/** @brief Check if one channel Fifo is full
		 *  
		 * @param ip IP Netaddress
		 * 
		 * @param ch Index of Dio channel
		 * 
		 * @return true if it is full and false otherwise
		 */
		 
		bool isFifoFull(string ip, int ch);
		
		/** @brief Check if one channel Fifo is empty
		 *  
		 * @param ip IP Netaddress
		 * 
		 * @param ch Index of Dio channel
		 * 
		 * @return true if it is empty and false otherwise
		 */
		 
		bool isFifoEmpty(string ip, int ch);
		
		/** @brief Check number of elements in one channel Fifo
		 *  
		 * @param ip IP Netaddress
		 * 
		 * @param ch Index of Dio channel
		 * 
		 * @return number of elements Fifo contains
		 */
		 
		int fifoSize(string ip, int ch);
		
		/** @brief Get top element of one channel Fifo
		 *  
		 * @param ip IP Netaddress
		 * 
		 * @param ch Index of Dio channel
		 * 
		 * @param empty It is used to indicate if Fifo is empty already
		 * 
		 * @return Fifo top element
		 */
		 
		timespec fifoTop(string ip, int ch, bool & empty);
		
		/** @brief All values of one channel Fifo
		 *  
		 * @param ip IP Netaddress
		 * 
		 * @param ch Index of Dio channel
		 * 
		 * @return all values of one channel Fifo
		 */
		 
		vector<timespec> fifoValues(string ip, int ch);
		
		
		/** @brief Check if one channel Fifo is full
		 *  
		 * @param ip IP Netaddress
		 * 
		 * @return all values from all Fifos
		 */
		 
		vector< vector<timespec> > AllFifoValues(string ip);
		
		/** @brief Dio destructor **/
		
		~Dio();
};
