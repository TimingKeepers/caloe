/**
 ******************************************************************************* 
 * @file access_internals.h
 *  @brief Access: Store information about read/write/scan operation we want to 
 *  perform.
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

#ifndef ACCESS_INTERNALS_CALOE_H
#define ACCESS_INTERNALS_CALOE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../etherbone/api/etherbone.h"
#include "../etherbone/api/glue/version.h"

/// Defines execute caloe execution mode (0: Use Etherbone API, 1: Use eb-tools in Etherbone repo)
#define EXECUTE_CALOE_MODE 0

/// Indicates everything was successfully
#define ALL_OK 0
/// You are trying to execute an invalid operation (see table of operations)
#define INVALID_OPERATION -1
/// It fails when it tries to open network socket
#define ERROR_OPEN_SOCKET -2
/// It fails when it tries to connect with device
#define ERROR_OPEN_DEVICE -3
/// It fails when it tries to get memory map with SDB scan command
#define ERROR_SDB_SCAN -4
/// It fails when it tries to execute Etherbone read/write cycle
#define ERROR_OPEN_CYCLE -5
/// It fails when it tries to close network socket
#define ERROR_CLOSE_SOCKET -6
/// It fails when it tries to close device connection
#define ERROR_CLOSE_DEVICE -7
/// It fails when it tries to close Etherbone read/write cycle
#define ERROR_CLOSE_CYCLE -8
/// It fails when it tries to access device with a invalid word size @see align_caloe structure and values
#define ERROR_SIZE_NOT_SUPPORTED -9
/// It fails when it can not detect big/little endian configuration in device
#define ERROR_UNKNOWN_ENDIAN -10
/// It fails when timeout is reached
#define ERROR_TIMEOUT -11
/// It fails when operation can not run
#define ERROR_OPERATION_RUN -12
/// It fails when parser can not understand configuration file corretly
#define ERROR_PARSE_CONFIG_FILE -13

/// Timeout (ms) to read/write operations (-1: NOT LIMITED)
#define TIMEOUT_LIMIT -1

/// Time gap between access (usecs, 0: no-wait)
#define SLEEP_ACCESS 0

/// Verbose mode (0: disabled, 1: enabled)
#define VERBOSE_CALOE 0

/// Data buffer to read/write operations with Etherbone library
static eb_data_t data;

/**
* @brief Stores network connection parameters.
*/

typedef struct network_connection {
	char * netaddress; /**< IP network address */
	unsigned int * port;	/**< Etherbone port */
} network_connection;

/**
* @brief Number of bytes to read/write.
*
*/
typedef enum align_access_caloe {SIZE_1B /**< Byte aligned */,
				 SIZE_2B /**< 2-byte aligned */,
				 SIZE_4B /**< 4-byte aligned */,
				 SIZE_8B /**< 8-byte aligned */
				} align_access_caloe;

/**
* @brief Memory operation type.
*
*/
typedef enum access_type_caloe {READ /**< Read memory address.*/,
				WRITE /**< Write memory address. */,
				SCAN /**< Scan device memory map **/,
				READ_WRITE /**< Read a memory address and then write a value in it. It's useful if you want to change a few bits and hold the rest. */
				} access_type_caloe;


/**
 * @brief Mask operation type.
 */
typedef enum mask_oper_caloe { MASK_AND, /**< Perform an AND bit operation */
						       MASK_OR /**< Perform an OR bit operation */
						 	   } mask_oper_caloe;

/**
*
* @brief Contains all information to do a memory access with read/write operations
*
**/

typedef struct access_caloe {
	eb_address_t address; /**< Memory Address to read/write */
	eb_address_t offset; /**< Offset to add base address */
	eb_data_t value; /**< Value to read/write */
	eb_data_t mask; /**< Mask to apply value before write or after read */
	mask_oper_caloe mask_oper; /**< Mask operation. If it is 0 OR operation is applied, otherwise AND operation is used */
	int is_config; /**< It indicates if memory address is refered to Etherbone configuration space with 1 or not with 0 */
	access_type_caloe mode; /**< Access type to perform */
	align_access_caloe align;/**< Memory width */
	network_connection networkc; /**< Network parameters */
} access_caloe;


/** This struct has got from Etherbone repository. You can get more information in http://www.ohwr.org/projects/etherbone-core.
*/

struct bus_record {
  int i;
  int stop;
  eb_address_t addr_first, addr_last;
  struct bus_record* parent;
};

#ifdef __cplusplus 
	extern "C" {
#endif

/**
*
* Network connection constructor
*
* @param ipname_server Server IP name (it must be preceded of udp/ or tcp/ prefix)
* @param nc Network connection instance to create
*
* @notes This function asumes port is default for Etherbone protocol
* @warning Format of Net address: <tcp|udp>/<ip>/<port>
*
*
**/

void build_network_con_caloe(char * ipname_server, network_connection *nc);

/**
*
* Full Network connection constructor (all parameters is given)
*
* @param ipname_server Server IP name (it must be preceded of udp/ or tcp/ prefix)
* @param port Server port
* @param nc Network connection instance to create
*
* @warning Format of Net address: <tcp|udp>/<ip>/<port>
*
**/

void build_network_con_full_caloe(char * ipname_server, unsigned int port, network_connection *nc);

/**
*
* Network connection constructor from another network connection
*
* @param dest Network connection to create
* @param src Network connection to show
*
*
**/

void copy_network_con_caloe(network_connection * dest, network_connection * src);

/**
*
* Network connection destructor
*
* @param nc Network connection to destroy
*
*
**/

void free_network_con_caloe(network_connection * nc);

/**
*
* Prints all information of network connection
*
* @param nc Network connection instance to print
*
*
**/

void print_network_con_caloe(network_connection * nc);


/**
*
* Access constructor
*
* @param address Address to access
* @param offset Offset to add base address
* @param value Value (it can be returned value in read operations or value to write in write operations)
* @param mask Mask to apply
* @param mask_oper Mask's operation
* @param is_config It indicates if you want to access Etherbone configuration space
* @param mode Access type
* @param align Number of bytes to access
* @param net Network connection instance (it contains server ip address and port)
* @param access Access instance to create
*
*
**/

void build_access_caloe(unsigned long int address, int offset, long int value, long int mask, mask_oper_caloe mask_oper, int is_config, access_type_caloe mode, align_access_caloe align, network_connection * net, access_caloe * access);

/**
*
* Access destructor
*
* @param access Access instance to destroy
*
*
**/

void free_access_caloe(access_caloe * access);


/**
*
* Prints all information of access
*
* @param access Access instance to print
*
*
**/

void print_access_caloe(access_caloe * access);

/**
*
* It implements read access over Etherbone library
*
* @param access It contains all information about access (value field contains returned value)
*
* @return Error code if error or zero otherwise
*
* @note Value field of access structure will contain returned value from memory address
*
**/

int read_caloe(access_caloe * access);

/**
*
* It implements write access over Etherbone library
*
* @param acces It contains all information about access (value field contains value to write)
*
* @return Error code if error or zero otherwise
*
* @note Value field of access is written into memory address
*
**/

int write_caloe(access_caloe * access);

/**
*
* It implements write after read access over Etherbone library. write after read 
* is a special kind of write that reads actual value in register, apply a mask to it and write 
* result into register again.
*
* @param access It contains all information about access (value field contains value to write after read operation is performed)
*
* @return Error code if error or zero otherwise
*
*
**/

int write_after_read_caloe(access_caloe * access);

/**
*
* It implements scan access over Etherbone library
*
* @param access It contains all information about access (only IP address and port are necessary)
*
* @return Error code if error or zero otherwise
*
* @note Only IP address and port are necessary
*
**/

int scan_caloe(access_caloe * access);

/**
*
* It implements one access (read, write, write after read or scan). This function calls write_caloe, read_caloe, write_write_after_read_caloe or scan_caloe
*
* @param access It contains all information about access
*
* @return Error code if error or zero otherwise
*
*
**/

int execute_native_caloe(access_caloe * access);

/**
*
* It implements one access (read, write, write after read or scan). This function calls etherbone tools (eb-read, eb-write and eb-ls)
*
* @param access It contains all information about access
* 
* @return Error code if error or zero otherwise
*
*
**/

int execute_tools_caloe(access_caloe * access);

/**
*
* It implements one access (read, write, write after read or scan). This function calls etherbone tools (eb-read, eb-write and eb-ls) or use Etherbone API library
* instead of EXECUTE_CALOE_MODE macro (1 = with eb-tools, 0 = with Etherbone API directly)
*
* @param access It contains all information about access
* 
* @return Error code if error or zero otherwise
*
*
**/

int execute_caloe(access_caloe * access);

#endif

#ifdef __cplusplus
}
#endif
