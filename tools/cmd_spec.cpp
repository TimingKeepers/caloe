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

using namespace std;

void printf_vuart(string res,string cmd) {
	vector<string> res_split = split(res,'\n');
	vector<string>::iterator it;
	
	for(it = res_split.begin() ; it != res_split.end()-1 ; it++) {
		cout << *it <<endl;
	}
}

int main ()
{
  Dio dio("../devices/dio/dio.cfg");
  Vuart vuart("../devices/vuart/vuart.cfg");
  
  string ip;
  long int len_pulse;
  timespec t_trig;
  int ch;
  char mode;
  char specific_dio;
  bool specific_dio_b = false;
  string cmd;
  string virtual_cmd;
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
																	string res;
																	char trash;
																	char loop;
																	
																	cout << "WARNING: Vuart is under testing!!"<<endl;
																	if(!specific_dio_b) {
																		cout <<endl<<"IP: ";
																		cin >> ip;
																	}
																	
																	cout << "Loop mode (y/n): ";
																	cin >> loop;
																	
																	if(loop == 'y')
																		cout << "Note: Execute exit command in order to close loop mode of vuart!!"<<endl;
																	
																	trash = getchar();
																	
																	do {
																		cout << "wrc# ";
																		getline(cin,virtual_cmd);
																		
																		if(virtual_cmd == "exit")
																			loop = 'n';
																			
																		else {
																			
																			if(virtual_cmd == "gui" || virtual_cmd == "stat cont") {
																				cout << "WARNING: gui and stat cont must not be used in remote mode (changing to stat cmd)!!"<<endl;
																				virtual_cmd = "stat";
																			}
																		
																			vuart.flush(proto+"/"+ip);
																		
																			res = vuart.execute_cmd(proto+"/"+ip,virtual_cmd);
																	
																			printf_vuart(res,virtual_cmd);
																		}
																		
																	} while(loop == 'y');
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
																		cout <<endl<<endl<<cmd<<" UNRECOGNIZED COMMAND"<<endl<<endl;
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
