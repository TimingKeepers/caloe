/**
 ******************************************************************************* 
 * @file Dio.cpp
 *  @brief Dio high-level operation source file
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
 
#include "Dio.h"

Dio::Dio() {
	// It loads Dio operations from configuration file
	dio.loadCfgFile("dio.cfg","dio");
}

Dio::Dio(const Dio & dio) {
	this->dio = dio.dio;
}

Dio::Dio(string config_path) {
	dio.loadCfgFile(config_path,"dio");
}

Dio & Dio::operator=(const Dio & dio) {
	this->dio = dio.dio;

	return *this;
}

void Dio::print() {
	cout << dio;
}

void Dio::scan(string ip) {
	ParamOperation params;
	ParamAccess param;
  
	// Set IP as parameter
    param.setIP(ip);
    params.addParameter(param);
    
    // Execute scan_root operation to show device memory map
    dio.execute("scan_root",params);
}

void Dio::configChOutput(string ip, int ch) {
	ParamOperation params;
	ParamAccess param;
	
	// Set IP and mask as parameters. Mask is used to choose channel.
	param.setIP(ip);
	param.setMask(ch);
	
	params.addParameter(param);
	params.addParameter(param);
	params.addParameter(param);
	
	// Execute config_channel_O to configure channel as Output
	dio.execute("config_channel_O",params);
}

void Dio::configChInput(string ip, int ch) {
	ParamOperation params;
	ParamAccess param;
	
	// Set IP and mask as parameters. Mask is used to choose channel.
	param.setIP(ip);
	param.setMask(ch);
	
	params.addParameter(param);
	params.addParameter(param);
	params.addParameter(param);
	
	// Execute config_channel_I to configure channel as Input
	dio.execute("config_channel_I",params);
}

void Dio::configChResistor(string ip, int ch) {
	ParamOperation params;
	ParamAccess param;
	
	// Set IP and mask as parameters. Mask is used to choose channel.
	param.setIP(ip);
	param.setMask(ch);
	
	params.addParameter(param);
	
	// Execute config_channel_R to configure channel with resistor termination
	dio.execute("config_channel_R",params);
}

void Dio::configChnResistor(string ip, int ch) {
	ParamOperation params;
	ParamAccess param;
	
	// Set IP and mask as parameters. Mask is used to choose channel.
	param.setIP(ip);
	param.setMask(ch);
	
	params.addParameter(param);
	
	// Execute config_channel_without_R to configure channel without resistor termination
	dio.execute("config_channel_without_R",params);
}

void Dio::configCh(string ip,int ch, char mode) {
	// i: input without R, I: input with R, d: output without R, D: output with R
	switch(mode) {
		case 'i':
			configChInput(ip,ch);
			configChnResistor(ip,ch);
			break;
		case 'I':
			configChInput(ip,ch);
			configChResistor(ip,ch);
			break;
		case 'd':			
			configChOutput(ip,ch);
			configChnResistor(ip,ch);
			break;
		case 'D':
			configChOutput(ip,ch);			
			configChResistor(ip,ch);
			break;
		default:
			break;
	};
}

void Dio::showConfigChs(string ip) {
	ParamOperation params;
	ParamAccess param;
	vector<eb_data_t> res;

	// Aux variables to get all information
	
	int config;

	int config0;
	int config1;
	int config2;
	int config3;
	int config4;

	int ch0_i;
	int ch1_i;
	int ch2_i;
	int ch3_i;
	int ch4_i;

	int ch0_r;
	int ch1_r;
	int ch2_r;
	int ch3_r;
	int ch4_r;

	int con_ch0;
	int con_ch1;
	int con_ch2;
	int con_ch3;
	int con_ch4;
	
	// Set IP as parameter
	param.setIP(ip);
	params.addParameter(param);

	// Execute show_config_channels to check channel configuration
	res = dio.execute("show_config_channels",params);
	
	// Get channel configurations and print them
	 
	config = res.at(0);
	
	// Initialize aux variables
	
	config0 = config & 0x0000000f;
	config1 = (config & 0x000000f0) >> 4;
	config2 = (config & 0x00000f00) >> 8;
	config3 = (config & 0x0000f000) >> 12;
	config4 = (config & 0x000f0000) >> 16;

	ch0_i = config0 & 0x4;
	ch1_i = config1 & 0x4;
	ch2_i = config2 & 0x4;
	ch3_i = config3 & 0x4;
	ch4_i = config4 & 0x4;

	ch0_r = config0 & 0x8;
	ch1_r = config1 & 0x8;
	ch2_r = config2 & 0x8;
	ch3_r = config3 & 0x8;
	ch4_r = config4 & 0x8;

	con_ch0 = config0 & 0x3;
	con_ch1 = config1 & 0x3;
	con_ch2 = config2 & 0x3;
	con_ch3 = config3 & 0x3;
	con_ch4 = config4 & 0x3;

	// Print channel 0 information
	
	cout <<endl<<endl<<"Channel 0"<<endl<<endl;
	
	switch(con_ch0) {
		case 0x0:
			cout <<"Connected to GPIO"<<endl;
			break;
		case 0x1:
			cout <<"Connected to DIO core"<<endl;
			break;
		default:
			cout <<"Undefined"<<endl;
			break;
	};

	if(ch0_i)
		cout <<"Configured as Input"<<endl;
	else
		cout <<"Configured as Output"<<endl;

	if(ch0_r)
		cout <<"Configured with R"<<endl;
	else
		cout <<"Configured without R"<<endl;
		
	// Print channel 1 information
	
	cout <<endl<<endl<<"Channel 1"<<endl<<endl;
	
	switch(con_ch1) {
		case 0x0:
			cout <<"Connected to GPIO"<<endl;
			break;
		case 0x1:
			cout <<"Connected to DIO core"<<endl;
			break;
		default:
			cout <<"Undefined"<<endl;
			break;
	};

	if(ch1_i)
		cout <<"Configured as Input"<<endl;
	else
		cout <<"Configured as Output"<<endl;

	if(ch1_r)
		cout <<"Configured with R"<<endl;
	else
		cout <<"Configured without R"<<endl;
		
	// Print channel 2 information
	
	cout <<endl<<endl<<"Channel 2"<<endl<<endl;
	
	switch(con_ch2) {
		case 0x0:
			cout <<"Connected to GPIO"<<endl;
			break;
		case 0x1:
			cout <<"Connected to DIO core"<<endl;
			break;
		default:
			cout <<"Undefined"<<endl;
			break;
	};

	if(ch2_i)
		cout <<"Configured as Input"<<endl;
	else
		cout <<"Configured as Output"<<endl;

	if(ch2_r)
		cout <<"Configured with R"<<endl;
	else
		cout <<"Configured without R"<<endl;

	// Print channel 3 information
	
	cout <<endl<<endl<<"Channel 3"<<endl<<endl;
	
	switch(con_ch3) {
		case 0x0:
			cout <<"Connected to GPIO"<<endl;
			break;
		case 0x1:
			cout <<"Connected to DIO core"<<endl;
			break;
		default:
			cout <<"Undefined"<<endl;
			break;
	};

	if(ch3_i)
		cout <<"Configured as Input"<<endl;
	else
		cout <<"Configured as Output"<<endl;

	if(ch3_r)
		cout <<"Configured with R"<<endl;
	else
		cout <<"Configured without R"<<endl;
	
	// Print channel 4 information
	
	cout <<endl<<endl<<"Channel 4"<<endl<<endl;
	
	switch(con_ch4) {
		case 0x0:
			cout <<"Connected to GPIO"<<endl;
			break;
		case 0x1:
			cout <<"Connected to DIO core"<<endl;
			break;
		default:
			cout <<"Undefined"<<endl;
			break;
	};

	if(ch4_i)
		cout <<"Configured as Input"<<endl;
	else
		cout <<"Configured as Output"<<endl;

	if(ch4_r)
		cout <<"Configured with R"<<endl;
	else
		cout <<"Configured without R"<<endl;
}

void Dio::pulseImm(string ip, int ch, int len_pulse) {
	ParamOperation params;
	ParamAccess param;
	
	// Configure channel as output without resistor termination
	
	configCh(ip,ch,'d');
	
	// Set user parameters
	
	// User parameter: IP, value (length) and offset/mask (ch) to choose channel
	
	param.setOffset(ch);
	param.setValue(len_pulse);
	param.setIP(ip);
	
	params.addParameter(param);
	
	param.reset();
	
	param.setIP(ip);
	param.setMask(ch);
	
	params.addParameter(param);
	
	// Execute dio_pulse_imm to generate inmediate pulse
	
	dio.execute("dio_pulse_imm",params);
}

bool Dio::isDioReady(string ip, int ch) {
	ParamOperation params;
	ParamAccess param;
	vector<eb_data_t> res;
	bool ready;
	
	// Set IP and mask (channel) as parameters
	
	param.setMask(ch);
	param.setIP(ip);
	
	params.addParameter(param);
	
	// Execute dio_trig_ready to check if Dio is ready
	
	res = dio.execute("dio_trig_ready",params);
	
	if(res.at(0) != 0) {
		ready = true;
	}
	else {
		ready = false;
	}
	
	return ready;
}

void Dio::pulseProg(string ip, int ch, int len_pulse, timespec t_trig) {
	ParamOperation params;
	ParamAccess param;
	
	// Check if Dio is ready
	bool ready = isDioReady(ip,ch);
	
	// If Dio is ready...
	if(ready) {
		
		// Configure channel as Output without resistor termination
		configCh(ip,ch,'d');
		
		// Initialize trigger timestamp in aux variables
		
		unsigned long long sech, secl, ns;

		sech = ((unsigned long long) t_trig.tv_sec) >> 32;
		secl = ((unsigned long long) t_trig.tv_sec);
		ns = (t_trig.tv_nsec/8);

		// Set user parameters
		
		param.setOffset(ch);
		param.setValue(secl);
		param.setIP(ip);
		
		params.addParameter(param);
		
		param.reset();
		
		param.setOffset(ch);
		param.setValue(sech);
		param.setIP(ip);
		
		params.addParameter(param);
		
		param.reset();
		
		param.setOffset(ch);
		param.setValue(ns);
		param.setIP(ip);
		
		params.addParameter(param);
		
		param.reset();
		
		param.setOffset(ch);
		param.setValue(len_pulse);
		param.setIP(ip);
		
		params.addParameter(param);
		
		param.reset();
		
		param.setIP(ip);
		param.setMask(ch);
		
		params.addParameter(param);
		
		// Execute dio_pulse_prog to generate programmable pulse
		dio.execute("dio_pulse_prog",params);
	}
	else { // If Dio is not ready, print an message...
		cout <<"Dio is not ready yet!"<<endl;
	}
}

bool Dio::isFifoFull(string ip, int ch) {
	ParamOperation params;
	ParamAccess param;
	vector<eb_data_t> res;
	bool full;

	// Set IP and Offset (channel) as parameters
	param.setIP(ip);
	param.setOffset(ch);
	
	params.addParameter(param);
	
	// Execute fifo_isfull to check if Fifo is full
	res = dio.execute("fifo_isfull",params);
	
	if(res.at(0) != 0)
		full = true;
	else
		full = false;
	
	return full;
}

bool Dio::isFifoEmpty(string ip, int ch) {
	ParamOperation params;
	ParamAccess param;
	vector<eb_data_t> res;
	bool empty;
	
	// Set IP and Offset (channel) as parameters
	param.setIP(ip);
	param.setOffset(ch);
	
	params.addParameter(param);
	
	// Execute fifo_isempty to check if Fifo is empty
	res = dio.execute("fifo_isempty",params);
	
	if(res.at(0) != 0) {
		empty = true;
	}
	else {
		empty = false;
	}
	
	return empty;
}

int Dio::fifoSize(string ip, int ch) {
	ParamOperation params;
	ParamAccess param;
	vector<eb_data_t> res;
	int nused;

	// Set IP and Offset (channel) as parameters
	param.setIP(ip);
	param.setOffset(ch);
	
	params.addParameter(param);
	
	// Execute fifo_nfilled to get number of elements in Fifo
	res = dio.execute("fifo_nfilled",params);
	
	nused = (int) res.at(0);
	
	return nused;
}

timespec Dio::fifoTop(string ip, int ch, bool & empty) {
	ParamOperation params;
	ParamAccess param;
	vector<eb_data_t> res;
	timespec t;
	
	// Check if fifo is empty
	empty = isFifoEmpty(ip,ch);

	// If fifo is not empty...
	if(!empty) {
		
		// Set IP and Offset (channel) as parameters
		param.setIP(ip);
		param.setOffset(ch);
		
		params.addParameter(param);
		params.addParameter(param);
		params.addParameter(param);
		
		// Execute fifo_value to get top element of Fifo
		res = dio.execute("fifo_value",params);
		
		// Parse result to timespec struct
		
		unsigned long long secl = res.at(0);
		unsigned long long sech = res.at(1);
		int cycs = res.at(2);
	
		t.tv_sec = (sech << 32) + secl;
		t.tv_nsec = cycs; 
		
	}

	return t;
}

vector<timespec> Dio::fifoValues(string ip, int ch) {
	bool empty;
	timespec t;
	vector<timespec> res;
	int istamp = 0;

	// Get Top fifo element
	t = fifoTop(ip,ch,empty);

	cout <<endl<<endl<<"FIFO "<<ch<<endl;
	cout <<"---------------------------------------------------"<<endl<<endl;
	// While Fifo is not empty
	while(!empty) {
		// Add timestamp to vector
		res.push_back(t);
		// print timestamp
		cout <<"Time stamp #"<<istamp<<": "<<t.tv_sec<<" secs "<<t.tv_nsec<<" nsecs "<<endl;
		
		// Get next top element
		t = fifoTop(ip,ch,empty);
		
		// Update counter
		istamp++;		
	}
	
	cout <<"---------------------------------------------------"<<endl<<endl;

	return res;
}

vector< vector<timespec> > Dio::AllFifoValues(string ip) {
	vector< vector<timespec> > res;
	vector<timespec> aux;
	
	// Get all values from Fifo 0
	aux = fifoValues(ip,0);
	res.push_back(aux);
	
	// Get all values from Fifo 1
	aux = fifoValues(ip,1);
	res.push_back(aux);
	
	// Get all values from Fifo 2
	aux = fifoValues(ip,2);
	res.push_back(aux);
	
	// Get all values from Fifo 3
	aux = fifoValues(ip,3);
	res.push_back(aux);
	
	// Get all values from Fifo 4
	aux = fifoValues(ip,4);
	res.push_back(aux);
	
	return res;
}

Dio::~Dio() {}
