#include <Wire.h>

//Slave Address for the Communication
#define SLAVE_ADDRESS 0x50

char number[50];
int state = 0;
int cl = 1000000;
char hallo[] = "hallo";
int teller = 0;
bool rdy = false;

//Code Initialization
void setup() {
  // initialize i2c as slave
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDRESS);
  
 // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Wire.setClock(cl);
 
}

void loop() {
  delay(1);

} // end loop

// callback for received data
void receiveData(int byteCount){
  while(Wire.available()) { 
    Serial.print( Wire.read());
  }
}  // end while

// callback for sending data
void sendData(){
  
  if(rdy){
  Wire.write(hallo[teller]);
  teller++;
  if(teller == 5){
    teller = 0;
    rdy = false;
    Serial.print( "done!");
  }
  }else{
    Wire.write(0xff);
    rdy = true;
  }
}
