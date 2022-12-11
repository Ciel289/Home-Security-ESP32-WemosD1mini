// RX
/* Credit
 https://howtomechatronics.com/tutorials/arduino/how-to-build-an-arduino-wireless-network-with-multiple-nrf24l01-modules/
 
*/


#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>
#include <printf.h>
RF24 radio(4, 5);              // nRF24L01 (CE,CSN) (7, 8)--Arduino || (4, 5)--ESP32
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of our node in Octal format ( 04, 031, etc)


#include <TridentTD_LineNotify.h>
#define SSID        "--"      // Wifi's name
#define PASSWORD    "--"     // Wifi's password
#define LINE_TOKEN  "--"   // รหัส TOKEN ที่ได้มาจากข้างบน


void lineAlert(); // Function about send message


void setup() {
  Serial.begin(115200);

  //  WIFI and Line
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  byte i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    i++;
    if (i == 40) {
      break;  // 20sec for connect to wifi
    }
    delay(500);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());
  Serial.println(LINE.getVersion());
  LINE.setToken(LINE_TOKEN);
  LINE.notify("Hello from ESP32 Main Security");

  //  NRF24L01
  SPI.begin();
  radio.begin();
  network.begin(99, this_node); //(channel 0-125, node address)
  radio.setDataRate(RF24_2MBPS);
  radio.printDetails();

}


//  For keep data from tx
struct data01 {
  byte room;  // Room's Number
  bool conn;  // Connection
  bool alert = false; // Alert if tx detects something
};
struct data01 data01;


void loop() {
  //  Recive data
  dataRX.room = NULL;
  dataRX.alert = false;
  network.update();
  while ( network.available() ) {
    RF24NetworkHeader header01; 
    network.read(header01, &data01, sizeof(data01));
  }  
 if (dataRX.alert) {
    for (byte i = 1; i <= 4; i++) {
      if (dataRX.room == i) {
          Serial.println("Detected at Room" + String(i) + "! \t");
          LINE.notify("Detected at Room" + String(i) + "!");
      }
    }
  }
}



