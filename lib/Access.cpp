/**
 ******************************************************************************* 
 * @file Access.cpp
 *  @brief Access Class Source File
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
 
#include "Access.h"

namespace caloe {

Access::Access() {
	address = 0x00;
	address_init = 0x00;
	offset = 0x00;
	value = 0x00;
	mask = 0x00;
	mask_oper = MASK_OR;
	is_config = false;
	mode = SCAN;
	align = SIZE_4B;
	autoincr = 0;
}

Access::Access(eb_address_t address, eb_address_t address_init, eb_address_t offset, eb_data_t value, eb_data_t mask, mask_oper_caloe mask_oper, bool is_config, access_type_caloe mode, align_access_caloe align,int autoincr, Netcon networkc) {
	this->address = address;
	this->address_init = address_init;
	this->offset = offset;
	this->value = value;
	this->mask = mask;
	this->mask_oper = mask_oper;
	this->is_config = is_config;
	this->mode = mode;
	this->align = align;
	this->autoincr = autoincr;
	this->networkc = networkc;
}

Access::Access(const Access & access) {
	address = access.address;
	address_init = access.address_init;
	offset = access.offset;
	value = access.value;
	mask = access.mask;
	mask_oper = access.mask_oper;
	is_config = access.is_config;
	mode = access.mode;
	align = access.align;
	autoincr = access.autoincr;
	networkc = access.networkc;
}

Access Access::operator=(const Access & access) {
	address = access.address;
	address_init = access.address_init;
	offset = access.offset;
	value = access.value;
	mask = access.mask;
	mask_oper = access.mask_oper;
	is_config = access.is_config;
	mode = access.mode;
	align = access.align;
	autoincr = access.autoincr;
	networkc = access.networkc;

	return *this;
}

eb_address_t Access::getAddress() const {
	return address;
}

eb_address_t Access::getAddressInit() const {
	return address_init;
}

eb_address_t Access::getOffset() const {
	return offset;
}

eb_data_t Access::getValue() const {
	return value;
}

eb_data_t Access::getMask() const {
	return mask;
}

mask_oper_caloe Access::getMaskOper() const {
	return mask_oper;
}

bool Access::getIsConfig() const {
	return is_config;
}

access_type_caloe Access::getMode() const {
	return mode;
}

align_access_caloe Access::getAlign() const {
	return align;
}

int Access::getAutoincr() const {
	return autoincr;
}

Netcon Access::getNetcon() const {
	return networkc;
}

void Access::setAddress(eb_address_t address) {
	this->address = address;
}

void Access::setAddressInit(eb_address_t address_init) {
	this->address_init = address_init;
}

void Access::setOffset(eb_address_t offset) {
	this->offset = offset;
}

void Access::setValue(eb_data_t value) {
	this->value = value;
}

void Access::setMask(eb_data_t mask) {
	this->mask = mask;
}

void Access::setMaskOper(mask_oper_caloe mask_oper) {
	this->mask_oper = mask_oper;
}

void Access::setIsConfig(bool is_config) {
	this->is_config = is_config;
}

void Access::setMode(access_type_caloe mode) {
	this->mode = mode;
}

void Access::setAlign(align_access_caloe align) {
	this->align = align;
}

void Access::setAutoincr(int autoincr) {
	this->autoincr = autoincr;
}

void Access::setNetCon(Netcon networkc) {
	this->networkc = networkc;
}

void Access::reset() {
	address = address_init;
}

int Access::execute() {
	access_caloe access;
	network_connection nc;
	int is_config_int;
	int rcode = ALL_OK;

	char aux[50];

	strcpy(aux,networkc.getIP().c_str());

	build_network_con_full_caloe(aux,networkc.getPort(),&nc);

	if(is_config) {
		is_config_int=1;
	}
	else {
		is_config_int = 0;
	}

	build_access_caloe(address,offset,value,mask,mask_oper,is_config_int,mode,align,&nc,&access);
	
	rcode = execute_caloe(&access);

	if(mode == READ) {
		value = access.value;
	}

	free_access_caloe(&access);
	
	int align_v;
	
	switch(align) {
		case SIZE_1B: align_v = 1;
					  break;
		case SIZE_2B: align_v = 2;
					  break;
		case SIZE_4B: align_v = 4;
					  break;
		case SIZE_8B: align_v = 8;
					  break;
	};
	
	address += (autoincr*align_v);

	return rcode;
}

ParamConfig Access::loadAccessCfgFile(ifstream & file) {
	ParamConfig param;
	
	string line;
	int found;
	
	string EACT("EACTION");
	string NET("NET");
	string PORT("PORT");
	string MASK("MASK");
	string MSKNEG("MSKNEG");
	string MSKPOS("MSKPOS");
	string ALIGN("ALIGN");
	string MODE("MODE");
	string ADDRESS("ADDRESS");
	string VALUE("VALUE");
	string OFFSET("OFFSET");
	string AUTO("AUTO");
	
	bool end = false;
	char lc;
	char fc;
	
	while (file.good() && !file.eof() && !end) {
		
		line = "";

		file >> line;
	
		lc = *(line.end()-1);
		fc = *(line.begin());
		
		if(!line.empty() && fc != '#') {
			if((found = line.find(EACT)) != -1) {
				end = true;
			}
			
			if((found = line.find(NET)) != -1) {
				
				if(lc == 'P') {
					param.setIPParam();
					//cout << "NETP found!"<<endl;
				}
				else {
					file >> line;
					networkc.setIP(line);
					//cout << "NET found! "<<line<<endl;
				}
			}
			
			if((found = line.find(PORT)) != -1) {
				if(lc == 'P') {
					param.setPortParam();
					//cout << "PORTP found!"<<endl;
				}
				else {
					unsigned int port;
					file >> line;
					sscanf (line.c_str(),"%u",&port);
					networkc.setPort(port);
					//cout << "PORT found! "<<port<<endl;
				}
			}
			
			if((found = line.find(MASK)) != -1) {
				if(lc == 'P') {
					file >> line;
					line.erase(line.begin());
					line.erase(line.end()-1);
					param.setMasksString(line);
					//cout << "MASKP found! "<<line<<endl;
				}
				else {
					int mask;
					file >> line;
					sscanf (line.c_str(),"%x",&mask);
					this->mask = mask;
					//cout << "MASK found! "<<hex<<mask<<endl;
				}
			}
			
			if((found = line.find(MSKNEG)) != -1) {
				this->mask_oper = MASK_AND;
				//cout << "MASK AND found! "<<endl;
			}
			
			if((found = line.find(MSKPOS)) != -1) {
				this->mask_oper = MASK_OR;
				//cout << "MASK OR found! "<<endl;
			}
			
			if((found = line.find(ALIGN)) != -1) {
				unsigned int align;
				align_access_caloe align_v;
				
				file >> line;
				
				sscanf (line.c_str(),"%u",&align);
				
				switch(align) {
					case 1: align_v = SIZE_1B;
					break;
					
					case 2: align_v = SIZE_2B;
					break;
					
					case 4: align_v = SIZE_4B;
					break;
					
					case 8: align_v = SIZE_8B;
					break;
					
					default: align_v = SIZE_4B;
					break;
				}
				
				this->align = align_v;
				//cout << "ALIGN found! "<<align<<endl;
			}
			
			if((found = line.find(MODE)) != -1) {
				char mode;
				access_type_caloe mode_v;
				
				file >> line;
				
				mode = line.at(0);
				
				switch(mode) {
					case 'R': mode_v = READ;
					break;
					case 'W': mode_v = WRITE;
					break;
					case 'S': mode_v = SCAN;
					break;
					case 'C': mode_v = WCHAINED;
					break;
					default: mode_v = SCAN;
					break;
				}
				
				this->mode = mode_v;
				//cout << "MODE found! "<<mode<<endl;
			}
			
			if((found = line.find(ADDRESS)) != -1) {
				int address;
				file >> line;
				sscanf (line.c_str(),"%x",&address);
				
				this->address = address;
				this->address_init = address;
				//cout << "ADDRESS found! "<<hex<<address<<endl;
			}
			
			if((found = line.find(VALUE)) != -1) {
				if(lc == 'P') {
					param.setValueParam();
					//cout << "VALUEP found! "<<endl;
				}
				else {
					int value;
					file >> line;
					sscanf (line.c_str(),"%x",&value);
					
					this->value = value;
					//cout << "VALUE found! "<<hex<<value<<endl;
				}
			}
			
			if((found = line.find(OFFSET)) != -1) {
					file >> line;
					line.erase(line.begin());
					line.erase(line.end()-1);
					
					param.setOffsetsString(line);
					//cout << "OFFSET found! "<<line<<endl;
			}
			
			if((found = line.find(AUTO)) != -1) {
				int autoincr;
				file >> line;
				sscanf (line.c_str(),"%d",&autoincr);
				this->autoincr = autoincr;
				//cout << "AUTO found! "<<autoincr<<endl;
			}
			
		}
		else {
			if (fc == '#')
				getline(file,line);
		}
	}
	
	return param;
}


ostream & operator<<(ostream & os, Access & access) {
	os << "Address: 0x"<< hex << access.address<<endl;
	os << "Offset: 0x"<< hex << access.offset<<endl;
	os << "Value: 0x"<< hex << access.value<<endl;
	os << "Mask: 0x"<< hex << access.mask<<endl;

	switch(access.mask_oper) {
		case MASK_OR:
		os << "Mask oper: OR"<<endl;
		break;
		case MASK_AND:
		os << "Mask oper: AND"<<endl;
		break;
	}

	if(access.is_config) {
		os <<"=> Etherbone space configuration"<<endl;
	}
	
	switch(access.mode) {
		case READ:
			os << "Operation: READ"<<endl;
		break;
		case WRITE:
			os << "Operation: WRITE"<<endl;
		break;
		case SCAN:
			os << "Operation: SCAN"<<endl;
		break;
		case WCHAINED:
			os << "Operation: CHAINED WRITE"<<endl;
		break;
	}

	switch(access.align) {
		case SIZE_1B:
			os << "Align: 1 Byte"<<endl;
		break;
		case SIZE_2B:
			os << "Align: 2 Bytes"<<endl;
		break;
		case SIZE_4B:
			os << "Align: 4 Bytes"<<endl;
		break;
		case SIZE_8B:
			os << "Align: 8 Bytes"<<endl;
		break;
	}

	os << access.networkc;

	return os;
}

istream & operator>>(istream & is, Access & access) {
	cout << "address: ";
	is >> hex >>access.address;
	cout << "offset: ";
	is >> hex >> access.offset;
	cout << "value: ";
	is >> hex >> access.value;
	cout << "mask: ";
	is >> hex >> access.mask;

	char mo;

	cout << "mask_oper (o: or, a: and): ";
	is >> mo;

	while(mo != 'a' && mo != 'o') {
		is >> mo;
	}

	if(mo == 'a') {
		access.mask_oper = MASK_AND;
	}
	else {
		access.mask_oper = MASK_OR;
	}

	cout <<"config_space (y/n): ";

	char cs;

	is >> cs;

	while(cs != 'y' && cs != 'n') {
		is >> cs;
	}

	if(cs == 'y') {
		access.is_config = true;
	}
	else {
		access.is_config = false;
	}

	cout << "mode (w: write, r: read, c: chained write, s: scan): ";

	char mode;

	is >> mode;

	while(mode != 'w' && mode != 'r' && mode != 's' && mode != 'c') {
		is >> mode;
	}

	if(mode == 'r') {
		access.mode = READ;
	}
	else {
		if (mode == 'w') {
			access.mode = WRITE;
		}
		else {
			if (mode == 's') {
				access.mode = SCAN;
			}
			else {
				access.mode = WCHAINED;
			}
		}
	}

	cout << "align (1,2,4,8): ";

	char al;

	is >> al;

	while(al != '1' && al != '2' && al != '4' && al != '8') {
		is >> al;
	}

	if(al == '1') {
		access.align = SIZE_1B;
	}
	else {
		if (al == '2') {
			access.align = SIZE_2B;
		}
		else {
			if (al == '4') {
				access.align = SIZE_4B;
			}
			else {
				access.align = SIZE_8B;
			}
		}
	}

	is >> access.networkc;

	return is;
}

Access::~Access() {}

}
