/**
 * @file      main.cpp
 * @brief     i2c Master 
 * @author    Rik Honcoop
 * @license   See LICENSE
 */

 
#include <stdio.h>
#include <iostream>
#include "I2c_Master.hpp"

int main (int argc, char *argv[])
{
	
	I2c_Master m = new I2c_Master();
	int deviceId = 0x05;
	bool available = m.deviceAvailableForParing(0x05);
	std::cout <<"Is deviceId: "<< deviceId <<"available? " available << "\n";
	
	std::cout << "Response van slave: ";
	if(available){
		uint8_t data[5];
		
		m.readFromDevice(deviceId,data,5);
		
		for(int x = 0; x <5; x++){
			std::cout << (char)data[x];
		}
	}

    return 0;
}


