/**
 ******************************************************************************* 
 * @file Operation.cpp
 *  @brief Operation class source file
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
 
#include "Operation.h"

namespace caloe {

Operation::Operation() {
	
}

Operation::Operation(string name, string doc) {
	this->name = name;
	this->doc = doc;
}

Operation::Operation(const Operation & op) {
	name = op.name;
	doc = op.doc;
	list_access = op.list_access;
	list_param = op.list_param;
}

Operation Operation::operator=(const Operation & op) {
	name = op.name;
	doc = op.doc;
	list_access = op.list_access;
	list_param = op.list_param;
	
	return *this;	
}

string Operation::getName() const {
	return name;
}

string Operation::getDoc() const {
	return doc;
}

void Operation::setName(string name) {
	this->name = name;
}

void Operation::setDoc(string doc) {
	this->doc = doc;
}

void Operation::addAccess(const Access & access, const ParamConfig & param) {
	// Add new access to vector end
	list_access.push_back(access);
	// Add needed parameters to vector end
	list_param.push_back(param);
}

void Operation::reset() {
	vector< Access >::iterator it_access;
	
	// For each access in the operation, reset it
	for(it_access = list_access.begin() ; it_access != list_access.end() ; it_access++)
		it_access->reset();
}

vector<eb_data_t> Operation::execute(ParamOperation & params) {
	vector< Access >::iterator it_access;
	vector< ParamConfig>::iterator it_param;
	vector<ParamAccess>::iterator it_user;
	vector<eb_data_t> res;

	// Extracts user parameters of ParamOperation
	vector<ParamAccess> user_params = params.getParamAccess();
	
	// For each access in operation...
	for(it_user = user_params.begin(), it_access = list_access.begin(), it_param = list_param.begin() ; it_user != user_params.end() && it_access != list_access.end() && it_param != list_param.end() ; it_access++, it_param++, it_user++) {
		ParamAccess param = *it_user;
		// Get its needed parameters
		char needed_parameters = it_param->getParametersMask();
		// Get its user parameters
		char user_parameters = param.getParametersMask();
		
		//cout <<endl<<"--------------------------------------------------------------------------------"<<endl;
		//cout <<endl<<"need: "<<hex<<(int)needed_parameters<<" given: "<<hex<<(int)user_parameters<<endl<<endl;

		// If user gave all parameters...
		if(needed_parameters == user_parameters) {
			
			// Update access information with user parameters
			
			Netcon nc = it_access->getNetcon();
			
			if (needed_parameters & PARAM_NETADDRESS) {
				nc.setIP(param.getIP());
			}

			if (needed_parameters & PARAM_PORT) {
				nc.setPort(param.getPort());
			}

			if (needed_parameters & PARAM_MASK) {
				it_access->setMask((it_param->getMasksParam()).at(param.getMask()));
			}

			if (needed_parameters & PARAM_OFFSET) {
				it_access->setOffset((it_param->getOffsetsParam()).at(param.getOffset()));
			}

			if (needed_parameters & PARAM_VALUE) {
				it_access->setValue(param.getValue());
			}

			it_access->setNetCon(nc);

			// Execute access
			int ok;
			int retry = 0;
			
			do {
				ok = it_access->execute();
				retry++;
				
				if(MAX_RETRY > 0 && retry > MAX_RETRY)
					exit(-1);
			} while(ok != ALL_OK);
			
			// If access type is READ, get read value to return it
			
			if(it_access->getMode() == READ)
				res.push_back(it_access->getValue());
		}
		//cout <<endl<<"--------------------------------------------------------------------------------"<<endl;
	}
	
	return res;
}

void Operation::loadOperationCfgFile(ifstream & file) {
	string BOPER("BOPERATION");
	string EOPER("EOPERATION");
	string BACT("BACTION");
	string NAME("NAME");
	string DOC("DOC");
	
	string line;
	int found;
	bool end = false;
	
	char fc;
	
	while (file.good() && !file.eof() && !end) {
		line = "";

		file >> line;
		
		fc = *(line.begin());
		
		if(!line.empty() && fc != '#') {

			if((found = line.find(NAME)) != -1) {
				file >> line;
				
				this->name = line;
				//cout << "Operation name "<<line<<endl;
			}
		
			if((found = line.find(DOC)) != -1) {
				getline(file,line);
				
				this->doc = line;
				//cout << "Operation doc "<<line<<endl;
			}
		
			if((found = line.find(BACT)) != -1) {
					Access a;
					ParamConfig p = a.loadAccessCfgFile(file);
					addAccess(a,p);
			}
			
			if((found = line.find(EOPER)) != -1) {
				end = true;
			}
		}
		else {
			if (fc == '#')
				getline(file,line);
		}
	}
	
}

ostream & operator<<(ostream & os, Operation & op) {
	
	vector< Access >::iterator it;
	vector < ParamConfig >::iterator it2;
	
	os <<endl<<"######################################################################"<<endl;
	
	os << "Operation Name: "<<op.name<<endl;
	os << "Docstring: "<<op.doc<<endl;
	
	for(it2 = op.list_param.begin(), it = op.list_access.begin() ; it != op.list_access.end() && it2 != op.list_param.end() ; it++ , it2++) {
		os <<endl<<endl;
		os << *it;
		os << *it2;
		os <<endl<<endl;
	}
	
	os <<endl<<"######################################################################"<<endl;

	return os;	
}

istream & operator>>(istream & is, Operation & op) {
	
	cout << "Operation Name: ";
	
	do {
		getline(is,op.name);
	} while(op.name.size() == 0);
	
	cout << "Docstring: ";
	
	do {
		getline(is,op.doc);
	} while(op.doc.size() == 0);
	
	cout << "List of Access: "<<endl<<endl;
	
	char cont;
	
	do {
		Access a;
		ParamConfig c;
		
		is >> a;
		
		is >> c;
		
		op.addAccess(a,c);
		
		cout <<"add another access? (y/n): ";
		is >> cont;
		
	} while (cont == 'y');
	
	return is;	
}

Operation::~Operation() {}

}
