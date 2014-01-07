/**
 ******************************************************************************* 
 * @file Device.cpp
 *  @brief Device Class Source File
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
 
#include "Device.h"

namespace caloe {

Device::Device() {
}

Device::Device(const Device & dev) {
	name = dev.name;
	list_operation = dev.list_operation;
}

Device Device::operator=(const Device & dev) {
	name = dev.name;
	list_operation = dev.list_operation;

	return *this;
}

string Device::getName() const {
	return name;
}

void Device::setName(string name) {
	this->name = name;
}

void Device::addOperation(const Operation & op) {
	pair< map<string,Operation>::iterator, bool > ret;
	
	// Try to insert operation in device
	ret = list_operation.insert(make_pair(op.getName(),op));
	
	// If operation exists, print an error message...
	if(! (ret.second)) {
		cout << "ERROR: Operation "<< op.getName() <<" already exists!"<<endl;
		cout << "IGNORING..."<<endl;
	}
}

void Device::reset(string name) {
	map<string,Operation>::iterator it;
	
	// Search operation in device
	it = list_operation.find(name);

	// If operation is found...
	if(it != list_operation.end()) {
		//cout << "OPERATION "<<name<<" found!"<<endl;
		
		// Execute operation
		(it->second).reset();
	}
	else { // If operation is not found, print an error message...
		cout << "ERROR: Operation "<<name<<" not found!"<<endl;
	}
}

vector<eb_data_t> Device::execute(string name, ParamOperation & params) {
	map<string,Operation>::iterator it;
	vector<eb_data_t> res;

	// Search operation in device
	it = list_operation.find(name);

	// If operation is found...
	if(it != list_operation.end()) {
		//cout << "OPERATION "<<name<<" found!"<<endl;
		
		// Execute operation
		res = (it->second).execute(params);
	}
	else { // If operation is not found, print an error message...
		cout << "ERROR: Operation "<<name<<" not found!"<<endl;
	}
	
	return res;
}

void Device::loadCfgFile(string path,string name_dev) {
	ifstream ifs;
	string line;
	int found;
	string BOPER("BOPERATION");
	
	char fc;

	this->name = name_dev;
	
	ifs.open (path.c_str(), ifstream::in);
	
	while (ifs.good() && !ifs.eof()) {	
		
		line = "";

		ifs >> line;
		
		fc = *(line.begin());
		
		if(!line.empty() && fc != '#') {
			if((found = line.find(BOPER)) != -1) {
				Operation o;
				o.loadOperationCfgFile(ifs);
				addOperation(o);
			}
		}
		else {
			if(fc == '#')
				getline(ifs,line);
		}
		
	}

	ifs.close();
}

ostream & operator<<(ostream & os, Device & dev) {
	
	map <string,Operation>::iterator it;
	
	os <<endl<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
	
	os <<"Device Name: "<< dev.name<<endl;
	
	for(it = dev.list_operation.begin() ; it != dev.list_operation.end() ; it++) {
		os << it->second <<endl;
	}
	
	os <<endl<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;

	return os;
}

istream & operator>>(istream & is, Device & dev) {
	
	cout << "Device Name: ";
	
	do {
		getline(is,dev.name);
	} while(dev.name.size() == 0);
	
	cout << "List of Operation: "<<endl<<endl;
	
	char cont;
	
	do {
		Operation o;
		
		is >> o;
		
		dev.addOperation(o);
		
		cout <<"add another operation? (y/n): ";
		is >> cont;
		
	} while (cont == 'y');
	
	return is;
}

Device::~Device() {}

}
