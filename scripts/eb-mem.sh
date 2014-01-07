#! /bin/bash

 # ****************************************************************************** 
 # @file eb-mem.sh
 #  @brief Script to use easy Etherbone tools
 #
 #  Copyright (C) 2013
 #
 #  Memory access with Etherbone tools
 #
 #  @author Miguel Jimenez Lopez <klyone@ugr.es>
 #
 #  @bug none!
 #
 # ******************************************************************************
 #  This library is free software; you can redistribute it and/or
 #  modify it under the terms of the GNU Lesser General Public
 #  License as published by the Free Software Foundation; either
 #  version 3 of the License, or (at your option) any later version.
 #
 #  This library is distributed in the hope that it will be useful,
 #  but WITHOUT ANY WARRANTY; without even the implied warranty of
 #  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 #  Lesser General Public License for more details.
 #  
 #  You should have received a copy of the GNU Lesser General Public
 #  License along with this library. If not, see <http//www.gnu.org/licenses/>.
 # ******************************************************************************

print_help() {
	echo ""
	echo "############################################################################"
	echo ""
	echo "Command: eb-mem <options>"
	echo ""
	echo "--read|-r: Perform a read operation. It needs a IP and memory address."
	echo "--write|-w: Perform a write operation. It needs a IP, memory address and value."
	echo "--scan|-s: Perform a scan operation. It needs a IP address."
	echo "--compact|-c: Use compact format. Perform a read or write operation depending on arguments"
	echo "--tcp|-t: Use TCP packets to communication."
	echo "--udp|-u: Use UDP packets to communication."
	echo "--bytes|-b: Indicates n-bytes alignment of memory."
	echo "--ip|-i: Device IP address."
	echo "--port|-p: Use a determinate port."
	echo "--address|-a: Device address to read/write access."
	echo "--value|-v: Value to write into device."
	echo "--help|-h: Show this help."
	echo ""
	echo "NOTE: This script does not try to implement all options of Etherbone tools."
	echo "If you want to execute these operations with other options, you must use "
	echo "original tools developped by Etherbone team!!"
	echo ""
	echo "Examples:"
	echo ""
	echo "eb-mem --scan --ip 10.10.10.10"
	echo "eb-mem --read --ip 10.10.10.10 --address 0x64233"
	echo "eb-mem --write --ip 10.10.10.10 --address 0x64233 --value 0x123"
	echo "(COMPACT READ) => eb-mem --compact --ip 10.10.10.10 --address 0x64233"
	echo "(COMPACT WRITE) => eb-mem --compact --ip 10.10.10.10 --address 0x64233 --value 0x124"
	echo ""
	echo "############################################################################"
	echo ""
}

nargs=$#
EB_TOOLS=${EB_TOOLS-../etherbone/api/tools}

n=0
mode='none'
t_transport='udp'
bytes=4
port=''
ip='none'
address='none'
value='none'

while (($n < $nargs)) ;
do
	arg=$1
	
	case $arg in
		--read|-r) mode='read'; echo "Mode read enabled!";;
		--write|-w) mode='write' ; echo "Mode write enabled!";;
		--scan|-s) mode='scan' ; echo "Mode scan enabled!";;
		--compact|-c) mode='compact' ; echo "Mode compact enabled!";;
		--tcp|-t) t_transport='tcp' ; echo "Using TCP packets!";;
		--udp|-u) t_transport='udp' ; echo "Using UDP packets!";;
		--bytes|-b) shift; bytes=$1 ; echo "Operation for $bytes bytes!";;
		--ip|-i) shift; ip=$1; echo "IP address is $ip";;
		--port|-p) shift; port="/$1"; echo "Port is $port";;
		--address|-a) shift; address=$1 ; echo "Memory addresss is $address";;
		--value|-v) shift; value=$1 ; echo "Value to write is $value";;
		--help|-h) print_help ; exit 0;;
	esac
	
	n=$n+1
	shift
done

if [ "$mode" = 'none' ];
then
	echo "You must get access mode: --scan|--read|--write"
	exit -1
fi

if [ "$ip" = 'none' ];
then
	echo "You must get IP address to connect with Etherbone core"
	exit -1
fi

case $mode in
	scan) $EB_TOOLS/eb-ls $t_transport/$ip$port;;
	read) if [ "$address" = 'none' ];
		then
			echo "You must get memory address in read operation"
			exit -1
		fi
		$EB_TOOLS/eb-read $t_transport/$ip$port $address/$bytes
		;;
	write) if [ "$address" = 'none' ];
		then
			echo "You must get memory address in write operation"
			exit -1
		fi

		if [ "$value" = 'none' ];
		then
			echo "You must get value in write operation"
			exit -1
		fi
		
		$EB_TOOLS/eb-write $t_transport/$ip$port $address/$bytes $value
		;;
	compact)  if [ "$address" = 'none' ];
		then
			echo "You must get memory address in read or write operation"
			exit -1
		fi
		
		if [ "$value" = 'none' ];
		then
			$EB_TOOLS/eb-read $t_transport/$ip$port $address/$bytes
		else
			$EB_TOOLS/eb-write $t_transport/$ip$port $address/$bytes $value
		fi
		
		
esac
