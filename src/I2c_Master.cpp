#include "I2c_Master.hpp"

bool I2c_Master::deviceAvailableForParing(int device)
{
	bool isDevice = false;
	int fd = wiringPiI2CSetup(device);
	int data = wiringPiI2CRead(fd);
	//std::cout << data <<"\n";
	if(data == 0xff){
		isDevice = true;
	}
	return isDevice;
}

void I2c_Master::addToDevices(int device)
{
	//todo
}

void I2c_Master::writeToDevice(int device, uint8_t * data, int size)
{
	int fd = wiringPiI2CSetup(device);
	for(int i = 0; i < size - 1 ; i++){
		wiringPiI2CWrite(fd, data[i]);		
	}
}

void I2c_Master::readFromDevice(int device, uint8_t * data, int size)
{
	int fd = wiringPiI2CSetup(device);
	int dataCount = 0;
	for(;;){
		if(dataCount < size){
			data[dataCount] = (uint8_t)wiringPiI2CRead(fd);
			//std::cout << data[dataCount];
			dataCount++;
		}else{
			break;
		}
	}
}

