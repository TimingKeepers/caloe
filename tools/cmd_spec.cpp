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

/* According to POSIX.1-2001 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>

using namespace std;

/**
 * @brief This function checks if an input character is avalaible to send to vuart. 
 * 
 * @return 0 if there is not data, other value otherwise
 */

int is_there_data() {

	fd_set fds;
	struct timeval tv = {0, 10000};


	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);

	int ret = select(1, &fds, NULL, NULL, &tv);

	return ret;
	
}

/**
 *
 * @brief Vuart terminal fuction
 * 
 */

void vuart_cmd(string ip) {

	Vuart vuart("../devices/vuart/vuart.cfg");
	struct termios oldkey, newkey;
	bool need_exit = false;	
	int keep_term = 0;
	const int MAX_POLL_WAIT = 20;
	int ipoll = 0;
	
	string net = ip;
	int ret;
	bool valid = false;
	
	fprintf(stderr, "[press CTRL+a to exit]\n");

	if(!keep_term) {
		tcgetattr(STDIN_FILENO,&oldkey);
		newkey.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
		newkey.c_iflag = IGNPAR;
		newkey.c_oflag = 0;
		newkey.c_lflag = 0;
		newkey.c_cc[VMIN]=1;
		newkey.c_cc[VTIME]=0;
		tcflush(STDIN_FILENO, TCIFLUSH);
		tcsetattr(STDIN_FILENO,TCSANOW,&newkey);
	}
	
	while(!need_exit) {

		if(is_there_data()) {

			char c = getchar();
			
			if(c == '\x01') {
				need_exit=true;
			}
			else {
				if(vuart.isReady(net)) {
					vuart.write(net,c);
				}
			}
		}
		
		if(ipoll == 0) {
			char data = vuart.read(net,valid);
		
			while(valid) {
				fprintf(stderr,"%c",data);
				data = vuart.read(net,valid);
			}
		}
		
		ipoll = (ipoll+1)%MAX_POLL_WAIT;

	}
	
	if(!keep_term)
		tcsetattr(STDIN_FILENO,TCSANOW,&oldkey);
}

int main ()
{
  Dio dio("../devices/dio/dio.cfg");
  
  string ip;
  long int len_pulse;
  timespec t_trig;
  int ch;
  char mode;
  char specific_dio;
  bool specific_dio_b = false;
  string cmd;
  string proto("udp");
  
  do {
	cout <<"Connect with specific dio? (y/n): ";
	cin >> specific_dio;
  } while (specific_dio != 'y' && specific_dio != 'n');
  
  if(specific_dio == 'y') {
		specific_dio_b = true;
		cout << "IP: ";
		cin >> ip;
  }
  
  do {
	  
	  if(!specific_dio_b)
		cout << "command>> ";
	  else 
		cout << "dio@"<<ip<<">> ";
		
	  cin >> cmd;
	  
	  if(cmd == "scan") {
		  cout <<endl<<"-------------------------------------------"<<endl;
		  cout <<"SCAN"<<endl<<"-------------------------------------------"<<endl;
		  
		  if(!specific_dio_b) {
			cout <<endl<<"IP: ";
			cin >> ip;
	      }
		  
		  dio.scan(proto+"/"+ip);
		  cout <<endl<<endl<<"-------------------------------------------"<<endl;
	  }
	  else {
		  if (cmd == "pulse_imm") {
			cout <<endl<<"-------------------------------------------"<<endl;
			cout <<"PULSE_IMM"<<endl<<"-------------------------------------------"<<endl;
			
			 if(!specific_dio_b) {
				cout <<endl<<"IP: ";
				cin >> ip;
			}
			
			cout <<endl<<"CHANNEL: ";
			cin >> ch;
			
			cout <<endl<<"PULSE WIDTH: ";
			cin >> len_pulse;
			
			dio.pulseImm(proto+"/"+ip,ch,len_pulse);
			
			cout <<endl<<endl<<"-------------------------------------------"<<endl;
		  }
		  else {
			  if(cmd == "pulse_prog") {
				  
				cout <<endl<<"-------------------------------------------"<<endl;
				cout <<"PULSE_PROG"<<endl<<"-------------------------------------------"<<endl;
			
				 if(!specific_dio_b) {
					cout <<endl<<"IP: ";
					cin >> ip;
				}
			
				cout <<endl<<"CHANNEL: ";
				cin >> ch;
			
				cout <<endl<<"PULSE WIDTH: ";
				cin >> len_pulse;
				
				cout <<endl<<"TRIG TIME (seconds): ";
				cin >> t_trig.tv_sec;
				
				t_trig.tv_nsec = 0;
				
				dio.pulseProg(proto+"/"+ip,ch,len_pulse,t_trig);
			
				cout <<endl<<endl<<"-------------------------------------------"<<endl;
			  }
			  else {
				  if(cmd == "config_ch") {
					  
					cout <<endl<<"-------------------------------------------"<<endl;
					cout <<"CONFIG_CH"<<endl<<"-------------------------------------------"<<endl;
			
					 if(!specific_dio_b) {
						cout <<endl<<"IP: ";
						cin >> ip;
					}
			
					cout <<endl<<"CHANNEL: ";
					cin >> ch;
					
					cout<<endl<<"MODE (i/I: input, d/D: output): ";
					cin >> mode;
					
					dio.configCh(proto+"/"+ip,ch,mode);
			
					cout <<endl<<endl<<"-------------------------------------------"<<endl;
				  }
				  else {
					  if(cmd == "fifo_val") {
						  
						cout <<endl<<"-------------------------------------------"<<endl;
						cout <<"FIFO_VAL"<<endl<<"-------------------------------------------"<<endl;
			
						 if(!specific_dio_b) {
							cout <<endl<<"IP: ";
							cin >> ip;
						}
			
						cout <<endl<<"CHANNEL: ";
						cin >> ch;
						
						dio.fifoValues(proto+"/"+ip,ch);
			
						cout <<endl<<endl<<"-------------------------------------------"<<endl;
						  
					  }
					  else {
						  if(cmd == "all_fifo_val") {
							  
							cout <<endl<<"-------------------------------------------"<<endl;
							cout <<"ALL_FIFO_VAL"<<endl<<"-------------------------------------------"<<endl;
							
							 if(!specific_dio_b) {
								cout <<endl<<"IP: ";
								cin >> ip;
							}
							
							dio.AllFifoValues(proto+"/"+ip);
			
							cout <<endl<<endl<<"-------------------------------------------"<<endl;
						  }
						  else {
							  if(cmd == "fifo_empty") {
								  
								cout <<endl<<"-------------------------------------------"<<endl;
								cout <<"FIFO_EMPTY"<<endl<<"-------------------------------------------"<<endl;
			
								 if(!specific_dio_b) {
									cout <<endl<<"IP: ";
									cin >> ip;
								}
			
								cout <<endl<<"CHANNEL: ";
								cin >> ch;
								
								if(dio.isFifoEmpty(proto+"/"+ip,ch)) {
										cout <<endl<<"FIFO is EMPTY"<<endl;
								}
			
								cout <<endl<<endl<<"-------------------------------------------"<<endl;
							  }
							  else {
								  if(cmd == "fifo_full") {
									  
									cout <<endl<<"-------------------------------------------"<<endl;
									cout <<"FIFO_FULL"<<endl<<"-------------------------------------------"<<endl;
			
									 if(!specific_dio_b) {
										cout <<endl<<"IP: ";
										cin >> ip;
									}
			
									cout <<endl<<"CHANNEL: ";
									cin >> ch;
									
									if(dio.isFifoFull(proto+"/"+ip,ch)) {
										cout <<endl<<"FIFO is FULL"<<endl;
									}
			
									cout <<endl<<endl<<"-------------------------------------------"<<endl;
								  }
								  else {
									  if(cmd == "fifo_size") {
										  
										cout <<endl<<"-------------------------------------------"<<endl;
										cout <<"FIFO_SIZE"<<endl<<"-------------------------------------------"<<endl;
			
										 if(!specific_dio_b) {
											cout <<endl<<"IP: ";
											cin >> ip;
										}
			
										cout <<endl<<"CHANNEL: ";
										cin >> ch;
										
										cout <<endl<<"FIFO contains "<<dio.fifoSize(proto+"/"+ip,ch)<<" values"<<endl;
			
										cout <<endl<<endl<<"-------------------------------------------"<<endl;
									  }
									  else {
										  
											if(cmd == "exit") {}
											
											else {
												if(cmd == "show_config_ch") {
													cout <<endl<<"-------------------------------------------"<<endl;
													cout <<"SHOW_CONFIG_CH"<<endl<<"-------------------------------------------"<<endl;
													
													 if(!specific_dio_b) {
														cout <<endl<<"IP: ";
														cin >> ip;
													}
													
													dio.showConfigChs(proto+"/"+ip);
													cout <<endl<<endl<<"-------------------------------------------"<<endl;
												}
												
												else {
													
													if(cmd == "connect") {
														cout <<endl<<"IP: ";
														cin >> ip;
														
														specific_dio_b = true;
													}
													
													else {
														if(cmd == "disconnect") {
															specific_dio_b = false;
														}
														else {
															if(cmd == "proto") {
																char proto_c;
																
																do {
																	cout <<"PROTO (t: tcp, u: udp): ";
																	cin >> proto_c;
																} while (proto_c != 't' && proto_c != 'u');
																
																if(proto_c == 't')
																	proto = "tcp";
																else
																	proto = "udp";
															}
															else {
																if(cmd == "vuart") {
																	 cout << "WARNING: Vuart is under testing!!"<<endl;
																	 if(!specific_dio_b) {
																		cout <<endl<<"IP: ";
																		cin >> ip;
																	}
																	vuart_cmd(proto+"/"+ip);
																}
																else {
																	if(cmd == "help" || cmd == "?") {
																		cout<<"COMMANDS: "<<endl<<endl;
																		cout <<"scan"<<endl;
																		cout <<"pulse_imm"<<endl;
																		cout <<"pulse_prog"<<endl;
																		cout <<"config_ch"<<endl;
																		cout <<"show_config_ch"<<endl;
																		cout <<"fifo_val"<<endl;
																		cout <<"all_fifo_val"<<endl;
																		cout <<"fifo_empty"<<endl;
																		cout <<"fifo_full"<<endl;
																		cout <<"fifo_size"<<endl;
																		cout <<"exit"<<endl;
																		cout <<"connect"<<endl;
																		cout <<"disconnect"<<endl;
																		cout <<"proto"<<endl;
																		cout <<"vuart"<<endl;
																		cout <<"help/?"<<endl;
																		cout<<endl<<endl;
																	}
																	else {
																		cout <<endl<<endl<<"UNRECOGNIZED COMMAND"<<endl<<endl;
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
