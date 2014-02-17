/**
 ******************************************************************************* 
 * @file access_internals.c
 *  @brief Implements access low-level operations (Etherbone API)
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
 
#include "access_internals.h"

/**
* read callback function. It is necessary to Etherbone library.
* You can get more information in http://www.ohwr.org/projects/etherbone-core
**/

// The code of this function is based on eb-read tool code (its comments has also been included)
// Please, see http://www.ohwr.org/projects/etherbone-core if you want to get more information

static void read_callback_caloe(eb_user_data_t user, eb_device_t dev, eb_operation_t op, eb_status_t status) {
	int* stop = (int*)user;
	*stop = 1;

	if (status != EB_OK) {
		
			if(VERBOSE_CALOE)
				fprintf(stderr, "ERROR: Etherbone cycle failed! \n");
    		
    		return;
		//exit(1);
	}
	else {
			data = 0;
			for (; op != EB_NULL; op = eb_operation_next(op)) {
				data <<= (eb_operation_format(op) & EB_DATAX) * 8;
				data |= eb_operation_data(op);

				if (eb_operation_had_error(op)) {
        			
					if(VERBOSE_CALOE) 
						fprintf(stderr, "ERROR: Segmentation fault reading %s %s bits from address 0x%"EB_ADDR_FMT"\n",eb_width_data(eb_operation_format(op)),
					
					eb_format_endian(eb_operation_format(op)), eb_operation_address(op));
				}
			}
	}

}

/**
* write callback function. It is necessary to Etherbone library.
* You can get more information in http://www.ohwr.org/projects/etherbone-core
**/

// The code of this function is based on eb-write tool code (its comments has also been included)
// Please, see http://www.ohwr.org/projects/etherbone-core if you want to get more information

static void write_callback_caloe(eb_user_data_t user, eb_device_t dev, eb_operation_t op, eb_status_t status) {
	int* stop = (int*)user;
	*stop = 1;

	if (status != EB_OK) {
		if(VERBOSE_CALOE) 
			fprintf(stderr, "ERROR: Etherbone cycle failed! \n");

		return;
    		
		//exit(1);
	}
	else {
		data = 0;
		for (; op != EB_NULL; op = eb_operation_next(op)) {

			if (eb_operation_had_error(op)) {
		
				if(VERBOSE_CALOE) 
					fprintf(stderr, "ERROR: wishbone segfault %s %s %s bits to address 0x%"EB_ADDR_FMT"\n",
						eb_operation_is_read(op)?"reading":"writing",
						eb_width_data(eb_operation_format(op)),
						eb_format_endian(eb_operation_format(op)),
						eb_operation_address(op));
			}

		}
	}
}


/// This struct has got from Etherbone repository. You can get more information in http://www.ohwr.org/projects/etherbone-core.

static int print_id(struct bus_record* br) {
	if (br->i == -1) {
		return fprintf(stdout, "root");
	} else if (br->parent->i == -1) {
		return fprintf(stdout, "%d", br->i + 1);
	} else {
		int more = print_id(br->parent);
		return more + fprintf(stdout, ".%d", br->i + 1);
	}
}

/// This function is based on Etherbone tools (eb-ls). You can get more information in http://www.ohwr.org/projects/etherbone-core.

// The code of this function is based on eb-ls tool code (its comments has also been included)
// Please, see http://www.ohwr.org/projects/etherbone-core if you want to get more information

static void verbose_product(const struct sdb_product* product) {
	fprintf(stdout, "  product.vendor_id:        %016"PRIx64"\n", product->vendor_id);
	fprintf(stdout, "  product.device_id:        %08"PRIx32"\n",  product->device_id);
	fprintf(stdout, "  product.version:          %08"PRIx32"\n",  product->version);
	fprintf(stdout, "  product.date:             %08"PRIx32"\n",  product->date);
	fprintf(stdout, "  product.name:             "); fwrite(&product->name[0], 1, sizeof(product->name), stdout); fprintf(stdout, "\n");
	fprintf(stdout, "\n");
}

/// This function is based on Etherbone tools (eb-ls). You can get more information in http://www.ohwr.org/projects/etherbone-core.

// The code of this function is based on eb-ls tool code (its comments has also been included)
// Please, see http://www.ohwr.org/projects/etherbone-core if you want to get more information

static void verbose_component(const struct sdb_component* component, struct bus_record* br) {
	fprintf(stdout, "  sdb_component.addr_first: %016"PRIx64, component->addr_first);
	if (component->addr_first < br->parent->addr_first || component->addr_first > br->parent->addr_last) {
		fprintf(stdout, " !!! out of range\n");
	} else {
		fprintf(stdout, "\n");
	}

	fprintf(stdout, "  sdb_component.addr_last:  %016"PRIx64, component->addr_last);

	if (component->addr_last < br->parent->addr_first || component->addr_last > br->parent->addr_last) {
		fprintf(stdout, " !!! out of range\n");
	} else if (component->addr_last < component->addr_first) {
		fprintf(stdout, " !!! precedes addr_first\n");
	} else {
		fprintf(stdout, "\n");
	}

	verbose_product(&component->product);
}

/**
* scan callback function. It is necessary to Etherbone library.
* You can get more information in http://www.ohwr.org/projects/etherbone-core
**/

// The code of this function is based on eb-ls tool code (its comments has also been included)
// Please, see http://www.ohwr.org/projects/etherbone-core if you want to get more information

static void scan_callback_caloe(eb_user_data_t user, eb_device_t dev, const struct sdb_table* sdb, eb_status_t status) {
	struct bus_record br;
	int devices;
	int bad,wide;
	int norecurse = 0;
	int verbose = 0;
	const union sdb_record* des;
	int timeout;

	br.parent = (struct bus_record*)user;
	br.parent->stop = 1;

	if (status != EB_OK) {
	
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR: failed to retrieve SDB: %s\n", eb_status(status));
    
		return;
		//exit(1);
	}

	if (verbose) {
		fprintf(stdout, "SDB Bus "); print_id(br.parent); fprintf(stdout, "\n");
		fprintf(stdout, "  sdb_magic:                %08"PRIx32"\n", sdb->interconnect.sdb_magic);
		fprintf(stdout, "  sdb_records:              %d\n",   sdb->interconnect.sdb_records);
		fprintf(stdout, "  sdb_version:              %d\n",   sdb->interconnect.sdb_version);
		verbose_component(&sdb->interconnect.sdb_component, &br);

		if (sdb->interconnect.sdb_component.addr_first > br.parent->addr_first)
			br.parent->addr_first = sdb->interconnect.sdb_component.addr_first;
		if (sdb->interconnect.sdb_component.addr_last < br.parent->addr_last)
			br.parent->addr_last   = sdb->interconnect.sdb_component.addr_last;
	}

	devices = sdb->interconnect.sdb_records - 1;
	for (br.i = 0; br.i < devices; ++br.i) {
		int bad, wide;
		const union sdb_record* des;

		des = &sdb->record[br.i];
		bad = 0;

		if (verbose) {
			fprintf(stdout, "Device ");
			print_id(&br);

			switch (des->empty.record_type) {
				case sdb_record_device:
					fprintf(stdout, "\n");
					fprintf(stdout, "  abi_class:                %04"PRIx16"\n",  des->device.abi_class);
					fprintf(stdout, "  abi_ver_major:            %d\n",           des->device.abi_ver_major);
					fprintf(stdout, "  abi_ver_minor:            %d\n",           des->device.abi_ver_minor);
					fprintf(stdout, "  wbd_endian:               %s\n",           (des->device.bus_specific & SDB_WISHBONE_LITTLE_ENDIAN) ? "little" : "big");
					fprintf(stdout, "  wbd_width:                %"PRIx8"\n",   des->device.bus_specific & SDB_WISHBONE_WIDTH);

					verbose_component(&des->device.sdb_component, &br);
					bad = 0;
				break;

				case sdb_record_bridge:
					fprintf(stdout, "\n");
					fprintf(stdout, "  sdb_child:                %016"PRIx64, des->bridge.sdb_child);
					if (des->bridge.sdb_child < des->bridge.sdb_component.addr_first || des->bridge.sdb_child > des->bridge.sdb_component.addr_last-64) {
						fprintf(stdout, " !!! not contained in wbd_{addr_first,addr_last}\n");
					} else {
						fprintf(stdout, "\n");
					}

					verbose_component(&des->bridge.sdb_component, &br);
					bad = des->bridge.sdb_component.addr_first < br.parent->addr_first ||
					des->bridge.sdb_component.addr_last  > br.parent->addr_last   ||
					des->bridge.sdb_component.addr_first > des->bridge.sdb_component.addr_last ||
					des->bridge.sdb_child                < des->bridge.sdb_component.addr_first ||
					des->bridge.sdb_child                > des->bridge.sdb_component.addr_last-64;

				break;

				case sdb_record_integration: /* !!! fixme */
				case sdb_record_empty:
				default:
					fprintf(stdout, " not present (%x)\n", des->empty.record_type);
				break;
			}

		} else {
			wide = print_id(&br);
			if (wide < 15)
				fwrite("                     ", 1, 15-wide, stdout); /* align the text */

			switch (des->empty.record_type) {
				case sdb_record_bridge:
					fprintf(stdout, "%016"PRIx64":%08"PRIx32"  %16"EB_ADDR_FMT"  ",
						des->device.sdb_component.product.vendor_id,
						des->device.sdb_component.product.device_id,
						(eb_address_t)des->device.sdb_component.addr_first);
					fwrite(des->device.sdb_component.product.name, 1, sizeof(des->device.sdb_component.product.name), stdout);
					fprintf(stdout, "\n");
				break;

				case sdb_record_device:
					fprintf(stdout, "%016"PRIx64":%08"PRIx32"  %16"EB_ADDR_FMT"  ",
						des->device.sdb_component.product.vendor_id,
						des->device.sdb_component.product.device_id,
						(eb_address_t)des->device.sdb_component.addr_first);
					fwrite(des->device.sdb_component.product.name, 1, sizeof(des->device.sdb_component.product.name), stdout);
					fprintf(stdout, "\n");
				break;

				case sdb_record_integration: /* !!! fixme */
				case sdb_record_empty:
				default:
					fprintf(stdout, "---\n");
				break;
			}
		}

		if (!norecurse && !bad && des->empty.record_type == sdb_record_bridge) {
			br.stop = 0;
			br.addr_first = des->bridge.sdb_component.addr_first;
			br.addr_last  = des->bridge.sdb_component.addr_last;

			eb_sdb_scan_bus(dev, &des->bridge, &br, &scan_callback_caloe);

			timeout = TIMEOUT_LIMIT;      

			while (timeout > 0) {
				int telapsed = eb_socket_run(eb_device_socket(dev),timeout);

				if(br.stop)
					break;

				timeout -= telapsed;
			}
	  
			if(!br.stop) {
				if(VERBOSE_CALOE)	
					fprintf(stderr, "ERROR: Timeout expired! \n");
			}
		}
	}
}

void build_network_con_caloe(char * ipname_server, network_connection *nc) {
	nc->netaddress = malloc(sizeof(char)*strlen(ipname_server));
	strcpy(nc->netaddress,ipname_server);
	nc->port = NULL;
}

void build_network_con_full_caloe(char * ipname_server, unsigned int port, network_connection *nc) {
	build_network_con_caloe(ipname_server,nc);
	nc->port = malloc(sizeof(unsigned int));
	*(nc->port) = port;

}

void copy_network_con_caloe(network_connection * dest, network_connection * src) {
	if(src->netaddress != NULL) {
		int len = strlen(src->netaddress);
		dest->netaddress = malloc(sizeof(char)*len);
		strcpy(dest->netaddress, src->netaddress);
	}
	else {
		dest->netaddress = NULL;
	}

	if(src->port == NULL) {
		dest->port = NULL;
	}
	else {
		dest->port = malloc(sizeof(unsigned int));
		*(dest->port) = *(src->port);
	}
}

void free_network_con_caloe(network_connection * nc) {
	if(nc->netaddress != NULL)
		free(nc->netaddress);

	if(nc->port != NULL)
		free(nc->port);
}

void print_network_con_caloe(network_connection * nc) {
	if(nc->netaddress == NULL) {
		printf("Network address: Not address yet!!\n");
	}
	else {
		printf("Network address: %s\n",nc->netaddress);
	}

	if(nc->port == NULL) {
		printf("Port: 60368 (DEFAULT)\n");
	}
	else {
		printf("Port: %d \n",*(nc->port));
	}
}

void build_access_caloe(unsigned long int address, int offset, long int value,long int mask, mask_oper_caloe mask_oper, int is_config, access_type_caloe mode, align_access_caloe align, network_connection * net, access_caloe * access) {
	access->address = (eb_address_t) address;
	access->offset = (eb_address_t) offset;
	access->value = (eb_data_t) value;
	access->mask = (eb_data_t) mask;
	access->mask_oper = mask_oper;
	access->is_config = is_config;
	access->mode = mode;
	access->align = align;
	copy_network_con_caloe(&access->networkc,net);
}

void free_access_caloe(access_caloe * access) {
	free_network_con_caloe(&(access->networkc));
}

void print_access_caloe(access_caloe * access) {
	printf("\n------------ACCESS---------------\n");
	printf("Address: 0x%x\n",(unsigned int)access->address);
	printf("\n");
	printf("Offset: %d\n",(long int)access->offset);
	printf("\n");
	printf("Value: 0x%x\n",(unsigned int)access->value);
	printf("\n");
	printf("Mask: 0x%x\n",(unsigned int)access->mask);
	printf("\n");

	if(access->mask_oper == MASK_OR) {
		printf("Mask Operation: OR\n");
	}
	else {
		printf("Mask Operation: AND\n");
	}

	if(access->is_config == 0) {
		printf("Configuration space: NO\n");
	}
	else {
		printf("Configuration space: YES\n");
	}	

	printf("\n");

	switch(access->mode) {
		case READ: 
		printf("Operation: READ\n");
		break;
		case WRITE: 
		printf("Operation: WRITE\n");
		break;
		case READ_WRITE:
		printf("Operation: WRITE AFTER READ\n");
		break;
		case SCAN: 
		printf("Operation: SCAN\n");
		break;
		default:
		break;
	}

	printf("\n");

	switch(access->align) {
		case SIZE_1B: 
		printf("Size: 1 byte\n");
		break;
		case SIZE_2B: 
		printf("Size: 2 bytes\n");
		break;
		case SIZE_4B: 
		printf("Size: 4 bytes\n");
		break;
		case SIZE_8B: 
		printf("Size: 8 bytes\n");
		break;
		default:
		break;
	}
	
	printf("\n");
	print_network_con_caloe(&(access->networkc));
	printf("\n--------------------------------------\n");
}

// The code of this function is based on eb-read tool code (its comments has also been included)
// Please, see http://www.ohwr.org/projects/etherbone-core if you want to get more information

int read_caloe(access_caloe * access) {
	int stop;
  
	eb_width_t address_width, data_width;
	eb_format_t endian;
  
	eb_socket_t socket;
	eb_status_t status;
	eb_device_t device;
	eb_width_t line_width;
	eb_format_t line_widths;
	eb_format_t device_support;
	eb_format_t read_sizes;
	eb_format_t format;
	eb_cycle_t cycle;
	eb_data_t mask;
	int shift;

	int timeout;
  
	if(access->mode != READ) {
	  
		if(VERBOSE_CALOE)
			fprintf(stderr,"ERROR: Invalid read operation \n");
      
		return INVALID_OPERATION;
	}
  
	eb_format_t size;
	int attempts, probe, config;

	address_width = EB_ADDRX;
	data_width = EB_DATAX;
	size = EB_DATAX;
	endian = 0; /* auto-detect */
	attempts = 3;
	probe = 1;
	config = access->is_config;
	eb_address_t address = access->address + access->offset;

	char * netaddress = (access->networkc).netaddress;
	int port = ((access->networkc).port == NULL ? 60368 : *((access->networkc).port));
  
	char net[50];
  
	sprintf(net,"%s/%d",netaddress,port);

	switch(access->align) {
		case SIZE_1B: size = 1;
		break;

		case SIZE_2B: size = 2;
		break;

		case SIZE_4B: size = 4;
		break;

		case SIZE_8B: size = 8;
		break;
	}
  
	/* How big can the data be? */
	mask = ~(eb_data_t)0;
	mask >>= (sizeof(eb_data_t)-size)*8;

	if ((status = eb_socket_open(EB_ABI_CODE, 0, address_width|data_width, &socket)) != EB_OK) {
	
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR %d: Could not connect Etherbone socket \n",(int) status);
    
		return ERROR_OPEN_SOCKET;
	}
  
	if ((status = eb_device_open(socket, net, EB_ADDRX|EB_DATAX, attempts, &device)) != EB_OK) {
	  
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR %d: Could not connect Etherbone device \n", (int) status);
    
		return ERROR_OPEN_DEVICE;
	}
  
	line_width = eb_device_width(device);
	
	if (probe) {
   
		struct sdb_device info;
	
		if ((status = eb_sdb_find_by_address(device, address, &info)) != EB_OK) {
		
			if(VERBOSE_CALOE)
				fprintf(stderr, "ERROR %d: SDB scan failed! \n",(int) status);
      
			return ERROR_SDB_SCAN;
		}
    
		if ((info.bus_specific & SDB_WISHBONE_LITTLE_ENDIAN) != 0)
			device_support = EB_LITTLE_ENDIAN;
		else
			device_support = EB_BIG_ENDIAN;
		
		device_support |= info.bus_specific & EB_DATAX;
	} else {
		device_support = endian | EB_DATAX;
	}
  
	if (endian == 0) {
		/* Select the probed endian. May still be 0 if device not found. */
		endian = device_support & EB_ENDIAN_MASK;
	}
  
	/* Final operation endian has been chosen. If 0 the access had better be a full data width access! */
	format = endian;
  
	/* We need to pick the operation width we use.
	* It must be supported both by the device and the line.
	*/
	line_widths = ((line_width & EB_DATAX) << 1) - 1; /* Link can support any access smaller than line_width */
	read_sizes = line_widths & device_support;
    
	/* We cannot work with a device that requires larger access than we support */
	if (read_sizes == 0) {
	  
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR: Device could not access with size requested \n");
    
		return ERROR_SIZE_NOT_SUPPORTED;
	}
  
	/* Begin the cycle */
	if ((status = eb_cycle_open(device, &stop, &read_callback_caloe, &cycle)) != EB_OK) {
	  
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR %d: Could not create a new Etherbone operation cycle \n",(int) status);
    
		return ERROR_OPEN_CYCLE;
	}
  
	/* Can the operation be performed with fidelity? */
	if ((size & read_sizes) == 0) {
		eb_format_t fragment_sizes;
		eb_format_t fragment_size;
		eb_format_t complete_size;
   
		/* What will we do? Prefer to fragment if possible; reading is evil. */
    
		/* Fragmented reading is possible if there is a bit in read_sizes smaller than a bit in size */
		fragment_sizes = size;
		fragment_sizes |= fragment_sizes >> 1;
		fragment_sizes |= fragment_sizes >> 2; /* Filled in all sizes under max */
		
		if ((fragment_sizes & read_sizes) != 0) {
			int stride, chunk, count;
      
			/* We can do a fragmented read. Pick largest read possible. */
			complete_size = fragment_sizes ^ (fragment_sizes >> 1); /* This many bytes to read */
			/* (the above code sets complete_size = size, but works also if size were a mask) */
      
			/* Filter out only those which have a good read size */
			fragment_sizes &= read_sizes;
			/* Then pick the largest bit */
			fragment_sizes |= fragment_sizes >> 1;
			fragment_sizes |= fragment_sizes >> 2;
			fragment_size = fragment_sizes ^ (fragment_sizes >> 1);
      
			/* We read fragments */
			format |= fragment_size;
   
			/* Each operation reads this many bytes */
			chunk = format & EB_DATAX;
			count = complete_size / chunk;
      
			/* Read the low bits first */
			switch (format & EB_ENDIAN_MASK) {
				case EB_BIG_ENDIAN:
					stride = chunk;
				break;
				case EB_LITTLE_ENDIAN:
					address += chunk*(count-1);
					stride = -chunk;
				break;
				default:
					if(VERBOSE_CALOE)
						fprintf(stderr, "ERROR: Must know ENDIAN to fragment read \n");
					return ERROR_UNKNOWN_ENDIAN;
			}
      
			for (; count > 0; --count) {
        
				if (config)
					eb_cycle_read_config(cycle, address, format, 0);
				else
					eb_cycle_read(cycle, address, format, 0);
				address += stride;
			}
      
			shift = 0;
		} else {
			eb_address_t aligned_address;
      
			/* All bits in read_sizes are larger than all bits in size */
			/* We will need to do a larger operation than the read requested. */
      
			/* Pick the largest sized read possible. */
			fragment_size = fragment_sizes ^ (fragment_sizes >> 1);
			/* (the above code sets fragment_size = size, but works also if size were a mask) */
      
			/* Now pick the smallest bit in read_sizes. */
			complete_size = read_sizes & -read_sizes;
      
			/* We have our final operation format. */
			format |= complete_size;
  
			/* Align the address */
			aligned_address = address & ~(eb_address_t)(complete_size-1);
      
			/* How far do we need to shift the offset? */
			switch (format & EB_ENDIAN_MASK) {
				case EB_BIG_ENDIAN:
					shift = (complete_size-fragment_size) - (address - aligned_address);
				break;
				case EB_LITTLE_ENDIAN:
					shift = (address - aligned_address);
				break;
				default:
					if(VERBOSE_CALOE)
						fprintf(stderr, "ERROR: Must know ENDIAN to fill partial read \n");
					return ERROR_UNKNOWN_ENDIAN;
			}
      
			/* Issue the read */
			if (config)
				eb_cycle_read_config(cycle, aligned_address, format, 0);
			else
				eb_cycle_read(cycle, aligned_address, format, 0);
		}
	} else {
		/* There is a size requested that the device and link supports */
		format |= (size & read_sizes);
    
		/* If the access it full width, an endian is needed. Print a friendlier message than EB_ADDRESS. */
		if ((format & line_width & EB_DATAX) == 0 && (format & EB_ENDIAN_MASK) == 0) {
		
			if(VERBOSE_CALOE)
				fprintf(stderr, "ERROR: ENDIAN is required \n");
        
			return ERROR_UNKNOWN_ENDIAN;
		}
    
		if (config)
			eb_cycle_read_config(cycle, address, format, 0);
		else
			eb_cycle_read(cycle, address, format, 0);
    
		shift = 0;
	}
  
	eb_cycle_close(cycle);
    
	stop = 0;
	timeout = TIMEOUT_LIMIT;
  
	while(timeout > 0) {
		int telapsed = eb_socket_run(socket,timeout);
	
		if(stop)
			break;

		timeout -= telapsed;
	}
  
	if(!stop) {	
	
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR: Timeout expired! \n");
    
		return ERROR_TIMEOUT;
	}
  
	data >>= shift*8;
	data &= mask;
  
	if ((status = eb_device_close(device)) != EB_OK) {
	  
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR %d: failed to close Etherbone device \n", (int) status);
    
		return ERROR_CLOSE_DEVICE;
	}
  
	if ((status = eb_socket_close(socket)) != EB_OK) {
    
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR %d: failed to close Etherbone socket \n", (int) status);
    
		return ERROR_CLOSE_SOCKET;
	}

	//printf("Mask: 0x%x\n",access->mask);

	if(access->mask_oper == MASK_OR) {
		data = access->mask | data;
		//printf("OR\n");
	}
	else {
		data = access->mask & data;
		//printf("AND\n");
	}

	access->value = data;

	//fprintf(stdout, "%0*"EB_DATA_FMT"\n", size*2, access->value);

	//printf("READ IN 0x%x VALUE 0x%x \n\n",(unsigned int) address,(unsigned int) access->value);


	return ALL_OK;
}

// The code of this function is based on eb-write tool code (its comments has also been included)
// Please, see http://www.ohwr.org/projects/etherbone-core if you want to get more information

int write_caloe(access_caloe * access) {

	int stop;
  
	eb_width_t address_width, data_width;
	eb_format_t endian;
  
	eb_socket_t socket;
	eb_status_t status;
	eb_device_t device;
	eb_width_t line_width;
	eb_format_t line_widths;
	eb_format_t device_support;
	eb_format_t write_sizes;
	eb_format_t format;
	eb_cycle_t cycle;
	eb_data_t mask;
	int shift, shift_step;
	eb_address_t address_end;
	eb_data_t data_mask, partial_data;
	eb_data_t original_data;
	eb_address_t aligned_address;
  
	if(access->mode != WRITE) {
	  
		if(VERBOSE_CALOE)
			fprintf(stderr,"ERROR: Invalid write operation \n");
      
		return INVALID_OPERATION;
	}
  
	eb_format_t size;
	int attempts, probe, config;

	/* Default arguments */
	address_width = EB_ADDRX;
	data_width = EB_DATAX;
	size = EB_DATAX;
	endian = 0; /* auto-detect */
	attempts = 3;
	probe = 1;
	config = access->is_config;
	eb_address_t address = access->address + access->offset;
	eb_data_t data = access->value;
	char * netaddress = access->networkc.netaddress;
	int port = (access->networkc.port == NULL ? 60368 : *(access->networkc.port));

	int timeout;
  
	char net[50];
  
	sprintf(net,"%s/%d",netaddress,port);

	//printf("Mask: 0x%x\n",access->mask);

	if(access->mask_oper == MASK_OR) {
		data = access->mask | data;
		//printf("OR\n");
	}
	else {
		data = access->mask & data;
		//printf("AND\n");
	}

	switch(access->align) {
		case SIZE_1B: size = 1;
		break;

		case SIZE_2B: size = 2;
		break;

		case SIZE_4B: size = 4;
		break;

		case SIZE_8B: size = 8;
		break;
	}
  
	/* How big can the data be? */
	mask = ~(eb_data_t)0;
	mask >>= (sizeof(eb_data_t)-size)*8;

	if ((status = eb_socket_open(EB_ABI_CODE, 0, address_width|data_width, &socket)) != EB_OK) {
	  
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR %d: Could not connect Etherbone socket \n", (int) status);
    
		return ERROR_OPEN_SOCKET;
	}
  
	if ((status = eb_device_open(socket, net, EB_ADDRX|EB_DATAX, attempts, &device)) != EB_OK) {
	  
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR %d: Could not connect Etherbone device \n", (int) status);
    
		return ERROR_OPEN_DEVICE;
	}
  
	line_width = eb_device_width(device);
 
	if (probe) {
   
		struct sdb_device info;
		if ((status = eb_sdb_find_by_address(device, address, &info)) != EB_OK) {
	  
			if(VERBOSE_CALOE)
				fprintf(stderr, "ERROR %d: SDB scan failed! \n", (int) status);
      
			return ERROR_SDB_SCAN;
		}
    
		if ((info.bus_specific & SDB_WISHBONE_LITTLE_ENDIAN) != 0)
			device_support = EB_LITTLE_ENDIAN;
		else
			device_support = EB_BIG_ENDIAN;
    
		device_support |= info.bus_specific & EB_DATAX;
	} else {
		device_support = endian | EB_DATAX;
	}
  
	if (endian == 0) {
		/* Select the probed endian. May still be 0 if device not found. */
		endian = device_support & EB_ENDIAN_MASK;
	}
  
	/* Final operation endian has been chosen. If 0 the access had better be a full data width access! */
	format = endian;
  
	/* We need to pick the operation width we use.
	* It must be supported both by the device and the line.
	*/
	line_widths = ((line_width & EB_DATAX) << 1) - 1; /* Link can support any access smaller than line_width */
	write_sizes = line_widths & device_support;
    
	/* We cannot work with a device that requires larger access than we support */
	if (write_sizes == 0) {
	
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR: Device could not access with size requested \n");
    
		return ERROR_SIZE_NOT_SUPPORTED;
	}
  
	/* Begin the cycle */
	if ((status = eb_cycle_open(device, &stop, &write_callback_caloe, &cycle)) != EB_OK) {
	  
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR %d: Could not create a new Etherbone operation cycle \n",(int) status);
    
		return ERROR_OPEN_CYCLE;
	}

	/* Can the operation be performed with fidelity? */
	if ((size & write_sizes) == 0) {
		eb_format_t fragment_sizes;
		eb_format_t fragment_size;
		eb_format_t complete_size;
    
		/* What will we do? Prefer to fragment if possible; reading is evil. */
    
		/* Fragmented writing is possible if there is a bit in write_sizes smaller than a bit in size */
		fragment_sizes = size;
		fragment_sizes |= fragment_sizes >> 1;
		fragment_sizes |= fragment_sizes >> 2; /* Filled in all sizes under max */
		if ((fragment_sizes & write_sizes) != 0) {
      
			/* We can do a fragmented write. Pick largest write possible. */
			complete_size = fragment_sizes ^ (fragment_sizes >> 1); /* This many bytes to write */
			/* (the above code sets complete_size = size, but works also if size were a mask) */
      
			/* Filter out only those which have a good write size */
			fragment_sizes &= write_sizes;
			/* Then pick the largest bit */
			fragment_sizes |= fragment_sizes >> 1;
			fragment_sizes |= fragment_sizes >> 2;
			fragment_size = fragment_sizes ^ (fragment_sizes >> 1);
      
			/* We write fragments */
			format |= fragment_size;
      
			switch (format & EB_ENDIAN_MASK) {
				case EB_BIG_ENDIAN:
					shift = (complete_size - fragment_size)*8;
					shift_step = -fragment_size*8;
				break;
				case EB_LITTLE_ENDIAN:
					shift = 0;
					shift_step = fragment_size*8;
				break;
				default:
      
					if(VERBOSE_CALOE)
						fprintf(stderr, "ERROR: Must know ENDIAN to fragment read \n");
        
					return ERROR_UNKNOWN_ENDIAN;
			}	
      
			data_mask = ~(eb_data_t)0;
			data_mask >>= (sizeof(eb_data_t)-fragment_size)*8;
      
			for (address_end = address + complete_size; address != address_end; address += fragment_size) {
				partial_data = (data >> shift) & data_mask;
        
				if (config)
					eb_cycle_write_config(cycle, address, format, partial_data);
				else
					eb_cycle_write(cycle, address, format, partial_data);
				shift += shift_step;
			}
		} else {
      
			/* All bits in write_sizes are larger than all bits in size */
			/* We will need to do a larger operation than the write requested. */
      
			/* Pick the largest sized write possible. */
			fragment_size = fragment_sizes ^ (fragment_sizes >> 1);
			/* (the above code sets fragment_size = size, but works also if size were a mask) */
      
			/* Now pick the smallest bit in write_sizes. */
			complete_size = write_sizes & -write_sizes;
      
			/* We have our final operation format. */
			format |= complete_size;
      
			/* Align the address */
			aligned_address = address & ~(eb_address_t)(complete_size-1);
      
			/* How far do we need to shift the offset? */
			switch (format & EB_ENDIAN_MASK) {
				case EB_BIG_ENDIAN:
					shift = (complete_size-fragment_size) - (address - aligned_address);
				break;
				case EB_LITTLE_ENDIAN:
					shift = (address - aligned_address);
				break;
				default:
      
					if(VERBOSE_CALOE)
						fprintf(stderr, "ERROR: Must know ENDIAN to fill partial read \n");
        
					return ERROR_UNKNOWN_ENDIAN;
			}
			
			mask <<= shift*8;
			data <<= shift*8;
      
			/* Issue the read */
			if (config)
				eb_cycle_read_config(cycle, aligned_address, format, &original_data);
			else
				eb_cycle_read(cycle, aligned_address, format, &original_data);
      
			eb_cycle_close(cycle);
      
			stop = 0;
			timeout = TIMEOUT_LIMIT;
      
			while(timeout > 0) {
				int telapsed = eb_socket_run(socket, TIMEOUT_LIMIT);
		
				if(stop)
					break;

				timeout -= telapsed;
			}
	  

			if(!stop) {	
	
				if(VERBOSE_CALOE)
					fprintf(stderr, "ERROR: Timeout expired! \n");
    
				return ERROR_TIMEOUT;
			}
      
			/* Restart the cycle */
			eb_cycle_open(device, &stop, &write_callback_caloe, &cycle);
      
			/* Inject the data */
			data |= original_data & ~mask;
			if (config)
				eb_cycle_write_config(cycle, aligned_address, format, data);
			else
				eb_cycle_write(cycle, aligned_address, format, data);
		}
	} else {
		/* There is a size requested that the device and link supports */
		format |= (size & write_sizes);
    
		/* If the access it full width, an endian is needed. Print a friendlier message than EB_ADDRESS. */
		if ((format & line_width & EB_DATAX) == 0 && (format & EB_ENDIAN_MASK) == 0) {
		
			if(VERBOSE_CALOE)
				fprintf(stderr, "ERROR: ENDIAN is required \n");
        
			return ERROR_UNKNOWN_ENDIAN;
		}

		if (config)
			eb_cycle_write_config(cycle, address, format, data);
		else
			eb_cycle_write(cycle, address, format, data);
	}
  
	eb_cycle_close(cycle);
  
	stop = 0;
	timeout = TIMEOUT_LIMIT;
  
	while(timeout > 0) {
		int telapsed = eb_socket_run(socket,timeout);
	
		if(stop)
			break;

		timeout -= telapsed;
	}

	if(!stop) {	
	
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR %d: Timeout expired! \n",(int) status);
    
		return ERROR_TIMEOUT;
	}
  
	if ((status = eb_device_close(device)) != EB_OK) {
    
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR %d: failed to close Etherbone device \n",(int) status);
    
		return ERROR_CLOSE_DEVICE;
	}
  
	if ((status = eb_socket_close(socket)) != EB_OK) {
    
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR %d: failed to close Etherbone socket \n",(int) status);
    
		return ERROR_CLOSE_SOCKET;
	}

	//printf("WRITE IN 0x%x VALUE 0x%x \n\n",(unsigned int) address,(unsigned int) data);

	return ALL_OK;
}

int write_after_read_caloe(access_caloe * access) {
	eb_data_t mask = access->mask;
	mask_oper_caloe mask_oper = access->mask_oper;

	int rcode;

	if(access->mode != READ_WRITE) {
			if(VERBOSE_CALOE)
				fprintf(stderr,"ERROR: Invalid write after read operation \n");
      		
      		return INVALID_OPERATION;
  	}

	access->mode = READ;

	if((rcode = read_caloe(access)) != ALL_OK)
		return rcode;

	access->mask = 0x00;
	access->mask_oper = MASK_OR;

	access->mode = WRITE;

	if((rcode = write_caloe(access)) != ALL_OK)
		return rcode;

	access->mode = READ_WRITE;

	access->mask = mask;
	access->mask_oper = mask_oper;

	return ALL_OK;
}

// The code of this function is based on eb-ls tool code (its comments has also been included)
// Please, see http://www.ohwr.org/projects/etherbone-core if you want to get more information

int scan_caloe(access_caloe * access) {
  
	if(access->mode != SCAN) {
		if(VERBOSE_CALOE)
		fprintf(stderr,"ERROR: Invalid scan operation \n");
      
		return INVALID_OPERATION;
	}
  
  
	struct bus_record br;
	eb_socket_t socket;
	eb_status_t status;
	eb_device_t device;
	int verbose = 0;
  
	int attempts;
  
	br.parent = 0;
	br.i = -1;
	br.stop = 0;
	br.addr_first = 0;
	br.addr_last = ~(eb_address_t)0;
  
	eb_width_t address_width = EB_ADDRX;
	eb_width_t data_width = EB_DATAX;
	attempts = 3;

	char * netaddress = access->networkc.netaddress;
  
	int port = (access->networkc.port == NULL ? 60368 : *(access->networkc.port));

	int timeout;
  
	char net[50];
  
	sprintf(net,"%s/%d",netaddress,port);

	if ((status = eb_socket_open(EB_ABI_CODE, 0, address_width|data_width, &socket)) != EB_OK) {
	
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR %d: Could not connect Etherbone socket \n",status);
    
		return ERROR_OPEN_SOCKET;
	}
  
	if ((status = eb_device_open(socket, net, EB_ADDRX|EB_DATAX, attempts, &device)) != EB_OK) { 
    
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR: Could not connect Etherbone device \n");
    
		return ERROR_OPEN_DEVICE;
	}
  
	br.addr_last >>= (sizeof(eb_address_t) - (eb_device_width(device) >> 4))*8;
  
	if ((status = eb_sdb_scan_root(device, &br, &scan_callback_caloe)) != EB_OK) {
    
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR: Failed to scan remote device: %s\n", eb_status(status));
    
		return ERROR_SDB_SCAN;
	}

	if (!verbose)
		fprintf(stdout, "BusPath        VendorID         Product   BaseAddress(Hex)  Description\n");

	br.stop = 0;
	timeout = TIMEOUT_LIMIT;  

	while (timeout > 0) {
		int telapsed = eb_socket_run(socket,timeout);
    
		if(br.stop)
			break;

		timeout -= telapsed;
	}

	if(!br.stop) {	
	  
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR: Timeout expired! \n");
    
		return ERROR_TIMEOUT;
	}
  
	if ((status = eb_device_close(device)) != EB_OK) {
	  
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR: failed to close Etherbone device \n");
    
		return ERROR_CLOSE_DEVICE;
	}
  
	if ((status = eb_socket_close(socket)) != EB_OK) {
    
		if(VERBOSE_CALOE)
			fprintf(stderr, "ERROR: failed to close Etherbone socket \n");
    
		return ERROR_CLOSE_SOCKET;
	}
	

	return ALL_OK;
}

int execute_native_caloe(access_caloe * access) {
	int rcode;
	
	switch(access->mode) {
			case READ:
				rcode = read_caloe(access);
			break;
			case WRITE:	
				rcode = write_caloe(access);
			break;
			case READ_WRITE:
				rcode = write_after_read_caloe(access);
			break;
			case SCAN: 
				rcode = scan_caloe(access);
			break;
			default:
				rcode = 0;
			break;
	}
	
	return rcode;

}

int execute_tools_caloe(access_caloe * access) {
	pid_t pid;
	char cmd[20];
	char cmd_l[50];
	
	char etherbone_dir[] = "../etherbone/api/tools/";
	char scan_cmd[] = "eb-ls";
	char read_cmd[] = "eb-read";
	char write_cmd[] = "eb-write";
	char value[50];
	char addr[50];
	char net[50];

	int status, died;

	int pipefd[2];
	pipe(pipefd);
	
	
	eb_data_t mask_aux = access->mask;
	mask_oper_caloe mask_oper_aux = access->mask_oper;
	
	char * netaddress = access->networkc.netaddress;
	int port = (access->networkc.port == NULL ? 60368 : *(access->networkc.port));
	eb_data_t v = access->value;
	int align_v;
	
	switch(access->align) {
		case SIZE_1B: align_v = 1;
					break;
		case SIZE_2B: align_v = 2;
					break;
		case SIZE_4B: align_v = 4;
					break;
		case SIZE_8B: align_v = 8;
					break;
	}
	
	char buffer[2048];
	int nbuf = 0;
	int nread;
		
	if(access->mode == WRITE) {
		
		if(mask_oper_aux == MASK_OR) {
			v = v | mask_aux;
		}
		else {
			v = v & mask_aux;
		} 
	}
	
	sprintf(value,"0x%x",v);
	sprintf(addr,"0x%x/%d",access->address,align_v);
	sprintf(net,"%s/%d",netaddress,port);

	switch(pid=fork()){
		case -1:
			  if(VERBOSE_CALOE)
				fprintf(stderr,"ERROR: Fork is failed!!\n");
              break;
		case 0: 
			close(pipefd[0]); 

			dup2(pipefd[1], 1); 

			close(pipefd[1]);
			
			if(access->mode == READ_WRITE)
				access->mode = READ;

			switch(access->mode) {
				case SCAN: 
					strcpy(cmd,scan_cmd);
					sprintf(cmd_l,"%s%s",etherbone_dir,cmd);
					//fprintf(stderr,"exec() %s %s\n",cmd,net);
					execl(cmd_l,cmd,net,NULL);
					break;
				case READ: 
					strcpy(cmd,read_cmd);
					sprintf(cmd_l,"%s%s",etherbone_dir,cmd);
					//fprintf(stderr,"exec() %s %s %s\n",cmd,net,addr);
					execl(cmd_l,cmd,net,addr,NULL);
					break;
				case WRITE:
					strcpy(cmd,write_cmd);
					sprintf(cmd_l,"%s%s",etherbone_dir,cmd);
					//fprintf(stderr,"exec() %s %s %s %s\n",cmd,net,addr,value);
					execl(cmd_l,cmd,net,addr,value,NULL);
					break;
				default:
					exit(-1);
			};
			
			break;
	
		default: 
			close(pipefd[1]);
					
			while ((nread = read(pipefd[0], &buffer[nbuf], 1024)) != 0) { nbuf += nread;}
	
			buffer[nbuf] = 0;
					
			close(pipefd[0]);
		
			died= wait(&status);
					
			if(access->mode == READ || access->mode == READ_WRITE) {
				sscanf(buffer,"%x",&v);
						
				if(mask_oper_aux == MASK_OR) {
					access->value = v | mask_aux;
				}
				else {
					access->value = v & mask_aux;
				}
						
				if(access->mode == READ_WRITE) {
					access->mode = WRITE;
					access->mask = 0x00;
					access->mask_oper = MASK_OR;
				
					execute_tools_caloe(access);
							
					access->mask = mask_aux;
					access->mask_oper = mask_oper_aux;
					access->mode = READ_WRITE;
				}
			}
			else {
				if(access->mode == SCAN)
					fprintf(stderr,"%s\n",buffer);
			}
			break;
     };
     
     return status;
}

int execute_caloe(access_caloe * access) {
	int rcode;
	
	if (! EXECUTE_CALOE_MODE)
		rcode = execute_native_caloe(access);
	else {
		rcode = execute_tools_caloe(access);
	}
	
	if(SLEEP_ACCESS != 0)
		usleep(SLEEP_ACCESS);
	
	return rcode;
}
