/**
 ******************************************************************************* 
 * @file cmd_spec.cpp
 *  @brief SPEC command terminal
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
 
#include "../devices/dio/Dio.h"
#include "../devices/vuart/Vuart.h"

#include <arpa/inet.h>

using namespace std;

// 1-> It avoid vuart executes gui/stat cont
#define HIDE_GUI_STAT_CONT 0

// Number of channels of DIO device
#define NUMBER_CHS 5

// Max pulse length (28 b in DIO)
#define MAX_PULSE_LEN 268435455

// Code available at http://stackoverflow.com/questions/791982/determine-if-a-string-is-a-valid-ip-address-in-c
bool isValidIpAddress(const char *ipAddress)
{
	char buff[30];

	strcpy(buff,ipAddress);

	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, buff, &(sa.sin_addr));
	return result != 0;
}

string get_ip() {
	string ip;
	do {
		cout << "IP: ";
		cin >> ip;

		if(cin.fail()) {
			cout <<endl<<"Error: CIN istream error!"<<endl;
			exit(-1);
		}
	} while(!isValidIpAddress(ip.c_str()));

	return ip;
}

char get_specific_spec_mode() {
	char specific_spec;
 	
	do {
		cout <<"Do you want to connect with specific spec? (y/n): ";
		cin >> specific_spec;

		if(cin.fail()) {
			cout <<endl<<"Error: CIN istream error!"<<endl;
			exit(-1);
		}
  	} while (specific_spec != 'y' && specific_spec != 'n');

	return specific_spec;
}

int get_ch() {
	int ch;

	do {
		cout <<endl<<"Channel [0-4]: ";
		cin >> ch;

		if(cin.fail()) {
			cout <<endl<<"Error: CIN istream error!"<<endl;
			exit(-1);
		}
	} while (ch < 0 || ch >= NUMBER_CHS);

	return ch;
}

long int get_pulse_len() {

	long int len_pulse;
	
	do {
		cout <<endl<<"Pulse width (cycles) [>= 0 && <= 268435455] : ";
		cin >> len_pulse;

		if(cin.fail()) {
			cout <<endl<<"Error: CIN istream error!"<<endl;
			exit(-1);
		}
	} while(len_pulse < 0 || len_pulse > MAX_PULSE_LEN);

	return len_pulse;
}

long get_trig() {
	long tv_sec;

	cout <<endl<<"Trig time (seconds): ";
	cin >> tv_sec;

	if(cin.fail()) {
			cout <<endl<<"Error: CIN istream error!"<<endl;
			exit(-1);
	}

	return tv_sec;
}

char get_mode() {
	char mode;

	do {
		cout<<endl<<"Mode (i/I: input/input + R, d/D: output/output + R): ";
		cin >> mode;

		if(cin.fail()) {
			cout <<endl<<"Error: CIN istream error!"<<endl;
			exit(-1);
		}
	} while(mode != 'i' && mode != 'I' && mode != 'd' && mode != 'D');

	return mode;
}

char get_proto() {
	char proto_c;

	do {
		cout <<"Proto (t: tcp, u: udp): ";
		cin >> proto_c;
		
		if(cin.fail()) {
			cout <<endl<<"Error: CIN istream error!"<<endl;
			exit(-1);
		}

	} while (proto_c != 't' && proto_c != 'u');

	return proto_c;
}

char get_vuart_loop_mode() {
	char loop;

	do {
		cout << "Loop mode (y/n)?: ";
		cin >> loop;

		if(cin.fail()) {
			cout <<endl<<"Error: CIN istream error!"<<endl;
			exit(-1);
		}
	} while(loop != 'y' && loop != 'n');

	return loop;
}

// Remove additional characters
string delete_format_chars(string s) {
	string s2;
	int size = s.length();
	int i;
	
	for(i = 4 ; i < size ; i = i+5) {
		s2.push_back(s.at(i));
	}
	
	return s2;
}

// Print vuart command response
void printf_vuart(string res,string cmd) {
	vector<string> res_split = split(res,'\n');
	vector<string>::iterator it;
	vector<string>::iterator itb = res_split.begin();
	vector<string>::iterator ite = res_split.end()-1;
	string fl = delete_format_chars(*itb);
	
	bool cmd_first_line = (fl.find(cmd) != string::npos);
	
	if(cmd_first_line)
		itb++;
	
	for(it = itb ; it != ite ; it++) {
		cout << *it <<endl;
	}
}

int main ()
{
  // Load devices (Dio and Vuart)
  Dio dio("../devices/dio/dio.cfg");
  Vuart vuart("../devices/vuart/vuart.cfg");
  
  string ip;
  long int len_pulse;
  timespec t_trig;
  int ch;
  char mode;
  char specific_spec;
  bool specific_spec_b = false;
  string cmd;
  string virtual_cmd;
  string proto("udp");

  specific_spec = get_specific_spec_mode();

  if(specific_spec == 'y') {
		specific_spec_b = true;
		ip = get_ip();
  }
  
  do {
	  // Spec prompt
	  if(!specific_spec_b)
		cout << "command>> ";
	  else 
		cout << "spec@"<<ip<<">> ";
		
	  // Read command from user
	  cin >> cmd;
	  
	  if(cmd == "scan") {
		  cout <<endl<<"-------------------------------------------"<<endl;
		  //cout <<"SCAN"<<endl<<"-------------------------------------------"<<endl;
		  
		  if(!specific_spec_b) {
			ip = get_ip();
	      }
		  
		  dio.scan(proto+"/"+ip);
		  cout <<endl<<endl<<"-------------------------------------------"<<endl;
	  }
	  else {
		  if (cmd == "pulse_imm") {
			cout <<endl<<"-------------------------------------------"<<endl;
			//cout <<"PULSE_IMM"<<endl<<"-------------------------------------------"<<endl;
			
			 if(!specific_spec_b) {
				ip = get_ip();
			}
			
			ch = get_ch();

			len_pulse = get_pulse_len();
			
			dio.pulseImm(proto+"/"+ip,ch,len_pulse);
			
			cout <<endl<<endl<<"-------------------------------------------"<<endl;
		  }
		  else {
			  if(cmd == "pulse_prog") {
				  
				cout <<endl<<"-------------------------------------------"<<endl;
				//cout <<"PULSE_PROG"<<endl<<"-------------------------------------------"<<endl;
			
				 if(!specific_spec_b) {
					ip = get_ip();
				}
			
				ch = get_ch();
			
				len_pulse = get_pulse_len();
				
				t_trig.tv_sec = get_trig();
				
				t_trig.tv_nsec = 0;
				
				dio.pulseProg(proto+"/"+ip,ch,len_pulse,t_trig);
			
				cout <<endl<<endl<<"-------------------------------------------"<<endl;
			  }
			  else {
				  if(cmd == "config_ch") {
					  
					cout <<endl<<"-------------------------------------------"<<endl;
					//cout <<"CONFIG_CH"<<endl<<"-------------------------------------------"<<endl;
			
					 if(!specific_spec_b) {
						ip = get_ip();
					}
			
					ch = get_ch();
					
					mode = get_mode();
					
					dio.configCh(proto+"/"+ip,ch,mode);
			
					cout <<endl<<endl<<"-------------------------------------------"<<endl;
				  }
				  else {
					  if(cmd == "fifo_val") {
						  
						cout <<endl<<"-------------------------------------------"<<endl;
						//cout <<"FIFO_VAL"<<endl<<"-------------------------------------------"<<endl;
			
						 if(!specific_spec_b) {
							ip = get_ip();
						}
			
						ch = get_ch();
						
						dio.fifoValues(proto+"/"+ip,ch);
			
						cout <<endl<<endl<<"-------------------------------------------"<<endl;
						  
					  }
					  else {
						  if(cmd == "all_fifo_val") {
							  
							cout <<endl<<"-------------------------------------------"<<endl;
							//cout <<"ALL_FIFO_VAL"<<endl<<"-------------------------------------------"<<endl;
							
							 if(!specific_spec_b) {
								ip = get_ip();
							}
							
							dio.AllFifoValues(proto+"/"+ip);
			
							cout <<endl<<endl<<"-------------------------------------------"<<endl;
						  }
						  else {
							  if(cmd == "fifo_empty") {
								  
								cout <<endl<<"-------------------------------------------"<<endl;
								//cout <<"FIFO_EMPTY"<<endl<<"-------------------------------------------"<<endl;
			
								 if(!specific_spec_b) {
									ip = get_ip();
								}
			
								ch = get_ch();
								
								if(dio.isFifoEmpty(proto+"/"+ip,ch)) {
										cout <<endl<<"Fifo is empty"<<endl;
								}
								else {
									cout <<endl<<"Fifo is not empty"<<endl;
								}
			
								cout <<endl<<endl<<"-------------------------------------------"<<endl;
							  }
							  else {
								  if(cmd == "fifo_full") {
									  
									cout <<endl<<"-------------------------------------------"<<endl;
									//cout <<"FIFO_FULL"<<endl<<"-------------------------------------------"<<endl;
			
									 if(!specific_spec_b) {
										ip = get_ip();
									}
			
									ch = get_ch();
									
									if(dio.isFifoFull(proto+"/"+ip,ch)) {
										cout <<endl<<"Fifo is full"<<endl;
									}
									else {
										cout <<endl<<"Fifo is not full"<<endl;
									}
			
									cout <<endl<<endl<<"-------------------------------------------"<<endl;
								  }
								  else {
									  if(cmd == "fifo_size") {
										  
										cout <<endl<<"-------------------------------------------"<<endl;
										//cout <<"FIFO_SIZE"<<endl<<"-------------------------------------------"<<endl;
			
										 if(!specific_spec_b) {
											ip = get_ip();
										}
			
										ch = get_ch();
										
										cout <<endl<<"Fifo contains "<<dio.fifoSize(proto+"/"+ip,ch)<<" values"<<endl;
			
										cout <<endl<<endl<<"-------------------------------------------"<<endl;
									  }
									  else {
										  
											if(cmd == "exit") {}
											
											else {
												if(cmd == "show_config_ch") {
													cout <<endl<<"-------------------------------------------"<<endl;
													//cout <<"SHOW_CONFIG_CH"<<endl<<"-------------------------------------------"<<endl;
													
													 if(!specific_spec_b) {
														ip = get_ip();
													}
													
													dio.showConfigChs(proto+"/"+ip);
													cout <<endl<<endl<<"-------------------------------------------"<<endl;
												}
												
												else {
													
													if(cmd == "connect") {
														ip = get_ip();
														
														specific_spec_b = true;
													}
													
													else {
														if(cmd == "disconnect") {
															specific_spec_b = false;
														}
														else {
															if(cmd == "proto") {
																char proto_c;
																
																proto_c = get_proto();
																
																if(proto_c == 't')
																	proto = "tcp";
																else
																	proto = "udp";
															}
															else {
																if(cmd == "vuart") {
																	string res;
																	char trash;
																	char loop;
																	
																	cout << "Warning: Vuart is under testing!!"<<endl;
																	if(!specific_spec_b) {
																		ip = get_ip();
																	}
																	
																	loop = get_vuart_loop_mode();
																	
																	if(loop == 'y')
																		cout << "Note: Execute exit command in order to close loop mode of vuart!!"<<endl;
																	
																	trash = getchar();
																	
																	do {
																		cout << "wrc# ";
																		getline(cin,virtual_cmd);
																		
																		if(virtual_cmd == "exit")
																			loop = 'n';
																			
																		else {
																			
																			if(HIDE_GUI_STAT_CONT == 1) {
																				if(virtual_cmd == "gui" || virtual_cmd == "stat cont") {
																					cout << "Warning: gui and stat cont must not be used in remote mode (changing to stat cmd)!!"<<endl;
																					virtual_cmd = "stat";
																				}
																			}
																		
																			vuart.flush(proto+"/"+ip);
																		
																			res = vuart.execute_cmd(proto+"/"+ip,virtual_cmd);
																	
																			printf_vuart(res,virtual_cmd);
																		}
																		
																	} while(loop == 'y');
																}
																else {
																	if(cmd == "help" || cmd == "?") {
																		cout <<endl<<endl<<"-------------------------------------------"<<endl;
																		cout<<"Available commands: "<<endl<<endl;
																		cout <<"scan: it gets SDB information about device"<<endl<<endl;
																		cout <<"pulse_imm: it generates one immediate pulse throughtout one DIO channel"<<endl<<endl;
																		cout <<"pulse_prog: it generates one programmable pulse (with trigger timestamp) throughtout one DIO channel"<<endl<<endl;
																		cout <<"config_ch: it configures one DIO channel as Output/Input with/without resistor termination"<<endl<<endl;
																		cout <<"show_config_ch: it shows all DIO channel configuration"<<endl<<endl;
																		cout <<"fifo_val: it shows all timestamps in one DIO channel fifo"<<endl<<endl;
																		cout <<"all_fifo_val: it shows all timestamps in all DIO channel fifos"<<endl<<endl;
																		cout <<"fifo_empty: it checks if one DIO channel fifo is empty"<<endl<<endl;
																		cout <<"fifo_full: it checks if one DIO channel fifo is full"<<endl<<endl;
																		cout <<"fifo_size: it gets how many elements there are in one DIO channel fifo"<<endl<<endl;
																		cout <<"exit: it closes cmd_spec tool"<<endl;
																		cout <<"connect: it connects with one device (it avoids another commands asks for your IP address)"<<endl<<endl;
																		cout <<"disconnect: it disconnects of one device (another tools will ask for IP address)"<<endl<<endl;
																		cout <<"proto: it changes transport protocol (udp/tcp)"<<endl;
																		cout <<"vuart: it allows to send a vuart command to device"<<endl<<endl;
																		cout <<"help/?: it shows this message"<<endl<<endl;
																		cout <<"-------------------------------------------"<<endl;
																	}
																	else {
																		cout <<endl<<endl<<cmd<<": Unrecognized command"<<endl<<endl;
																	}
																}
																
															}
														}
													}
													
													}
												}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	  
  } while(cmd != "exit");
  
  return 0;
}
