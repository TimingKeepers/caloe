/**
 ******************************************************************************* 
 * @file Vuart.cpp
 *  @brief Vuart class source file
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
 
#include "Vuart.h"

Vuart::Vuart() {
	// it loads vuart configuration file
	vuart.loadCfgFile("vuart.cfg","vuart");
}

Vuart::Vuart(const Vuart & vuart) {
	this->vuart = vuart.vuart;
}

Vuart::Vuart(string config_path) {
	vuart.loadCfgFile(config_path,"vuart");
}

Vuart & Vuart::operator=(const Vuart & vuart) {
	this->vuart = vuart.vuart;
	
	return *this;
}

void Vuart::print() {
	// it prints operation table of vuart device
	cout << vuart;
}

bool Vuart::isReady(string ip) {
	bool ready;

	ParamOperation params;
	ParamAccess param;
	vector<eb_data_t> res;

	/*
	 * Set IP as user input parameter
	 */
	 
	param.setIP(ip);

	params.addParameter(param);

	/*
	 * Search and execute vuart_ready operation to check if vuart is ready
	 */
	 
	res = vuart.execute("vuart_ready",params);
	
	/*
	 * Read operation result and set flag 
	 */
	 
	int data = res.at(0);

	if (data == 0) {
		ready = true;
	}
	else {
		ready = false;
	}

	/*
	 * Return flag 
	 */
	 
	return ready;
}

char Vuart::read(string ip, bool & valid) {
	bool ready;
	int value;

	ParamOperation params;
	ParamAccess param;
	vector<eb_data_t> res;

	/*
	 * Set IP as user input parameter
	 */
	 
	param.setIP(ip);

	params.addParameter(param);

	/*
	 * Search and execute vuart_read to get a character from vuart
	 */
	 
	res = vuart.execute("vuart_read",params);

	value = res.at(0);

	/*
	 * Read operation result and set flag (to confirm a valid data or not)
	 */
	 
	if (value & 0x100) {
		valid = true;
	}
	else {
		valid = false;
	}

	/*
	 * Return read value 
	 */
	 
	return value;
}

void Vuart::write(string ip, char value) {
	ParamOperation params;
	ParamAccess param;

	/*
	 * Set IP and value to write as user input parameter
	 */
	param.setIP(ip);
	param.setValue(value);

	params.addParameter(param);

	/*
	 * Search and execute vuart_write to write a character to vuart
	 */
	 
	vuart.execute("vuart_write",params);
}

string Vuart::readString(string ip, unsigned long period) {
	char c;
	bool valid;
	string s;
	
	c = read(ip,valid);
	
	while(valid) {
		s.push_back(c);
		c = read(ip,valid);
		
		if(period != 0)
			usleep(period);
	}
	
	return s;
}
		 
void Vuart::writeString(string ip, string s, unsigned long period) {
	string::iterator it;
	
	for(it = s.begin() ; it != s.end() ; it++) {
		while(!isReady(ip)) {}
		write(ip,*it);
			
		if(period != 0)
			usleep(period);
	}
}

void Vuart::flush(string ip) {
	bool valid = false;
	
	this->read(ip,valid);
	
	while(valid) {
		this->read(ip,valid);
	}
}

string Vuart::execute_cmd(string ip, string cmd) {
	char data;
	bool valid = false;
	string res;
	
	string::iterator it;
	
	for(it = cmd.begin() ; it != cmd.end() ; it++) {
		if(this->isReady(ip)) {
			this->write(ip,*it);
		}
	}
	
	if(this->isReady(ip))
		this->write(ip,0xd);
	
	data = this->read(ip,valid);
	
	while(valid) {
		res.push_back(data);
		data = this->read(ip,valid);
	}
	
	return res;
}

Vuart::~Vuart() {}
