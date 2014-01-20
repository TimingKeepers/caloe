/**
 ******************************************************************************* 
 * @file Parameter.cpp
 *  @brief Parameter classes source file
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
 
#include "Parameters.h"

namespace caloe {

ParamAccess::ParamAccess() {
	// Default values
	parameters = PARAM_NONE;
	port = 0x00;
	offset = 0;
	value = 0x00;
	mask = 0;
}

ParamAccess::ParamAccess(const ParamAccess & pa) {
	parameters = pa.parameters;
	netaddress = pa.netaddress;
	port = pa.port;
	offset = pa.offset;
	mask = pa.mask;
	value = pa.value;
}

ParamAccess ParamAccess::operator=(const ParamAccess & pa) {
	parameters = pa.parameters;
	netaddress = pa.netaddress;
	port = pa.port;
	offset = pa.offset;
	mask = pa.mask;
	value = pa.value;

	return *this;
}

void ParamAccess::setIP(string netaddress) {
	// Mark netaddress parameter bit
	parameters |= PARAM_NETADDRESS;
	// Copy netaddress
	this->netaddress = netaddress;
}

void ParamAccess::setPort(unsigned int port) {
	// Mark port parameter bit
	parameters |= PARAM_PORT;
	// Copy port
	this->port = port;
}

void ParamAccess::setOffset(unsigned int offset) {
	// Mark offset parameter bit
	parameters |= PARAM_OFFSET;
	// Copy offset
	this->offset = offset;
}

void ParamAccess::setMask(unsigned int mask) {
	// Mark mask parameter bit
	parameters |= PARAM_MASK;
	// Copy mask
	this->mask = mask;
}

void ParamAccess::setValue(int value) {
	// Mark value parameter bit
	parameters |= PARAM_VALUE;
	// Copy value
	this->value = value;
}

string ParamAccess::getIP() const {
	return netaddress;
}

unsigned int ParamAccess::getPort() const {
	return port;
}

unsigned int ParamAccess::getOffset() const {
	return offset;
}

unsigned int ParamAccess::getMask() const {
	return mask;
}

int ParamAccess::getValue() const {
	return value;
}

char ParamAccess::getParametersMask() const {
	return parameters;
}

void ParamAccess::reset() {
	// Reset metadata
	parameters = PARAM_NONE;
}


ostream & operator<<(ostream & os, ParamAccess & pa) {
	int params = (int) pa.parameters;

	os << "Parameters mask: "<<hex<<"0x"<<params<<endl;
	os <<"Netaddress: "<<pa.netaddress<<endl;
	os <<"Port: "<<pa.port<<endl;
	os <<"Mask: "<<pa.mask<<endl;
	os <<"Offset: "<<pa.offset<<endl;
	os <<"Value: "<<pa.value<<endl;
	return os;
}

istream & operator>>(istream & is, ParamAccess & pa) {
	cout <<"Parameters mask: ";
	is >> pa.parameters;

	cout <<"Netaddress: ";
	is >> pa.netaddress;

	cout <<"Port: ";
	is >> pa.port;

	cout << "Mask: ";
	is >> pa.mask;

	cout << "Offset: ";
	is >> pa.offset;

	cout << "Value: ";
	is >> pa.value;

	return is;
}

ParamAccess::~ParamAccess() {
}

ParamOperation::ParamOperation() {
}

ParamOperation::ParamOperation(const ParamOperation & po) {
	parameter_values = po.parameter_values;
}

ParamOperation ParamOperation::operator=(const ParamOperation & po) {
	parameter_values = po.parameter_values;

	return *this;
}

void ParamOperation::addParameter(const ParamAccess & pa) {
	// Add new ParamAccess at vector end
	parameter_values.push_back(pa);
}

vector<ParamAccess> ParamOperation::getParamAccess() const {
	return parameter_values;
}

ostream & operator<<(ostream & os, ParamOperation & po) {
	vector<ParamAccess>::iterator it;

	// For each ParamAccess...
	for(it = po.parameter_values.begin() ; it != po.parameter_values.end() ; it++) {
		// Print its information
		os << *it;
	}

	return os;
}

istream & operator>>(istream & is, ParamOperation & po) {
	char more='y';
	ParamAccess pa;

	do {
		// Fill new ParamAccess
		is >> pa;
		// Add new ParamAccess
		po.addParameter(pa);
		// If you want to add new ParamAcces, press 'y'...
		cout <<"add new ParamAccess (y/n)?"<<endl;
		is >> more;
	} while (more != 'y');

	return is;

}

ParamOperation::~ParamOperation() {}

ParamConfig::ParamConfig() {
	// Default values
	parameters = PARAM_NONE;
}

ParamConfig::ParamConfig(const ParamConfig & pc) {
	parameters = pc.parameters;
	offsets = pc.offsets;
	masks = pc.masks;
}

ParamConfig ParamConfig::operator=(const ParamConfig & pc) {
	parameters = pc.parameters;
	offsets = pc.offsets;
	masks = pc.masks;

	return *this;
}

void ParamConfig::setOffsetsParam(vector<int> offsets) {
	// Offset is needed for Access
	parameters |= PARAM_OFFSET;

	// Copy offset vector
	this->offsets = offsets;
}

void ParamConfig::setMasksParam(vector<int> masks) {
	// Mask is needed for Access
	parameters |= PARAM_MASK;

	// Copy mask vector
	this->masks = masks;
}

void ParamConfig::setMasksString(string masks) {
	// Mask is needed for Access
	parameters |= PARAM_MASK;
	vector<string> s2;
	vector<string>::iterator it;
	int mask;
	
	// Split in several string and convert them into integer mask
	
	s2 = split(masks,',');
		
	for(it = s2.begin() ; it != s2.end() ; it++) {
		// Convert
		sscanf (it->c_str(),"%x",&mask);
		
		// Add to mask vector
		this->masks.push_back(mask);
	}
	
}

void ParamConfig::setOffsetsString(string offsets) {
	// Offset is needed for Access
	parameters |= PARAM_OFFSET;
	
	vector<string> s2;
	vector<string>::iterator it;
	int off;
	
	// Split in several string and convert them into integer offsets
	
	s2 = split(offsets,',');
		
	for(it = s2.begin() ; it != s2.end() ; it++) {
		// Convert
		sscanf (it->c_str(),"%x",&off);
			
		// Add to offset vector
		this->offsets.push_back(off);
	}
	
}

void ParamConfig::setIPParam() {
	// IP netaddress is needed for Access
	parameters |= PARAM_NETADDRESS;
}

void ParamConfig::setValueParam() {
	// Value is needed for Access
	parameters |= PARAM_VALUE;
}

void ParamConfig::setPortParam() {
	// Port is needed for Access
	parameters |= PARAM_PORT;
}

char ParamConfig::getParametersMask() const {
	return parameters;
}

vector<int> ParamConfig::getOffsetsParam() {
	return offsets;
}

vector<int> ParamConfig::getMasksParam() {
	return masks;
}

ostream & operator<<(ostream & os, ParamConfig & pc) {
	vector<int>::iterator it;
	int par = pc.parameters;
	
	os <<endl<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
	os << "Parameter mask: "<< hex << "0x"<<par<<endl;
	
	// If offset vector is set, print it
	if(pc.parameters & PARAM_OFFSET) {
		os<<"Offsets: {";
		
		for(it = pc.offsets.begin() ; it != pc.offsets.end() ; it++)
			os<<hex<<"0x"<<*it<<" ";
		os<<"}";
	}
		
	// If mask vector is set, print it
	if(pc.parameters & PARAM_MASK) {
		os<<"Masks: {";
		
		for(it = pc.masks.begin() ; it != pc.masks.end() ; it++)
			os<<hex<<"0x"<<*it<<" ";
		os<<"}";
	}
	
	os <<endl<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
		
	return os;
}

istream & operator>>(istream & is, ParamConfig & pc) {
	string s;
	vector<string> s2;
	vector<string>::iterator it;
	cout << "Parameter mask: ";
	int aux;
	
	// Set metadata
	
	is >> aux;
	
	pc.parameters = aux;
	
	int poff = pc.parameters & PARAM_OFFSET;
	int pmask = pc.parameters & PARAM_MASK;
	
	// If offset is required, set it
	if(pc.parameters & PARAM_OFFSET) {
		cout << "Offsets: ";
		
		int off;
		
		do {
			getline(is,s);
		}while(s.size() == 0);
		
		s2 = split(s,',');
		
		for(it = s2.begin() ; it != s2.end() ; it++) {
			sscanf (it->c_str(),"%x",&off);
			
			pc.offsets.push_back(off);
		}
	}
	
	// If mask is required, set it
	if(pc.parameters & PARAM_MASK) {
		cout << "Masks: ";
		
		int msk;
		
		do {
			getline(is,s);
		}while(s.size() == 0);
		
		s2 = split(s,',');
		
		for(it = s2.begin() ; it != s2.end() ; it++) {
			sscanf (it->c_str(),"%x",&msk);
			
			pc.masks.push_back(msk);
		}
		
	}
	
	return is;
}

ParamConfig::~ParamConfig() {}

}
