/**
 ******************************************************************************* 
 * @file Netcon.cpp
 *  @brief Network connection class source file
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
 
#include "Netcon.h"

namespace caloe {

Netcon::Netcon() {
	port = 60368;
}

Netcon::Netcon(string ip, unsigned int port) {
	this->ip = ip;
	this->port = port;
}

Netcon::Netcon(const Netcon & nc) {
	ip = nc.ip;
	port = nc.port;
}

Netcon Netcon::operator=(const Netcon & nc) {
	ip = nc.ip;
	port = nc.port;
	
	return *this;
}

string Netcon::getIP() const {
	return ip;
}

unsigned int Netcon::getPort() const {
	return port;
}

void Netcon::setIP(string ip) {
	this->ip = ip;
}

void Netcon::setPort(unsigned int port) {
	this->port = port;
}

ostream & operator<<(ostream & os, Netcon & nc) {
	os << "Network address: "<<nc.ip<<endl;
	os << "Port: "<<nc.port<<endl;
	
	return os;
}

istream & operator>>(istream & is, Netcon & nc) {
	cout << "ip: ";
	is >> nc.ip;
	cout << "port: ";
	is >> nc.port;

	return is;
}

Netcon::~Netcon() {}

}
