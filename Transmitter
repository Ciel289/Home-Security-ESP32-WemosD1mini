// TX
/*
  https://howtomechatronics.com/tutorials/arduino/how-to-build-an-arduino-wireless-network-with-multiple-nrf24l01-modules/
*/


#define inputPin D0 // choose the input pin (for PIR sensor)
bool pirState = false; // we start, assuming no motion detected
bool val = false; // variable for reading the pin status

#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>
#include <printf.h>
#define cep D2  // CE pin 
#define cnp D8  // CNS pin
RF24 radio(cep, cnp);             // nRF24L01 (CE,CSN) (7,8)-- Aduino || (0,8)--Wemos D1 mini
RF24Network network(radio);       // Include the radio in the network
const uint16_t nodeBase = 00;
const uint16_t this_node = 01;    // Address of this node in Octal format ( 04,031, etc) 


struct dataTX {
  const byte room = 1;  // If you have many room --> chang "room"'s number and "this_node" in same number
  bool conn = true;
  bool alert = false;
};
struct dataTX dataTX;


void setup() {
  pinMode(inputPin, INPUT); // declare sensor as input
  Serial.begin(115200);
  SPI.begin();
  radio.begin();
  network.begin(99, this_node); //(channel, node address) // Set channel same RX
  radio.setDataRate(RF24_2MBPS);
}


void loop() {
  val = digitalRead(inputPin);  // read input value
  if (val == true) {
    if (pirState == false) {
      Serial.println("Motion detected!"); // print on output change
      pirState = true;

      //  Send data
      dataTX.alert = true;
      network.update();
      RF24NetworkHeader header01(nodeBase); // (Address where the data is going)
      bool ok = network.write(header01, &dataTX, sizeof(dataTX));
    }
  }
  else {
    if (pirState == true) {
      Serial.println("Motion ended!\n");  // print on output change
      pirState = false;
      dataTX.alert = false;
    }
  }

}

