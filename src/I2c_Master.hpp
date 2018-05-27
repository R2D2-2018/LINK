
/**
 * @file	  i2c_Master.hpp
 * @brief     i2c Master 
 * @author    Rik Honcoop
 * @license   See LICENSE
 */

#ifndef I2C_MASTER_HPP
#define I2C_MASTER_HPP

#include <stdio.h>
#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>

class NavigationPath {
  public:
  	
	/**
	* @brief scan if device is available for paring.
	*
	* Reads from slave device one byte and if that byte is 0xff then the device 
	* is available for pairing
	*
	* @return If device is available for paring
	*/
	bool deviceAvailableForParing(int device);
		
	/**
	* @brief add a device to the device list
	*
	* the will also retreve its capabliaties
	*
	* @param[in] int device the device id byte
	* @return todo
	*/
	void addToDevices(int device)
	
	/**
	* @brief writes array of bytes to slave device
	*
	* Writes array of bytes to slave divice.
	* At this point we send a fixed leng of bytes to the slave and the slave 
	* needs to know how much byte it can acspect.
	*
	* @param[in]  device 	the device id byte
	* @param[in]  data 		byte array by reference
	* @param[in]  size		size of data array
	* @return none
	*/
	void writeToDevice(int device, char &data, int size);
	
	/**
	* @brief reads byte array from slave device
	*
	* Reads byte array from slave device, this is a fixed sized array for now
	* 
	*
	* @param[in]  device 	the device id byte
	* @param[in]  data 		byte array by reference
	* @param[in]  size		size of data array
	* @return 
	*/
	void readFromDevice(int device, uint8_t[] &data, int size);
	
	
#endif