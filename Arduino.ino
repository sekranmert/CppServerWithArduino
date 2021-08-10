#include <Ethernet.h>

//Mac adress for ethernet shield this one is default for Wiznet W5100
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
int port = 9999; 
IPAddress server(88,241,246,74);  // Ip for server,

// ip is static IP address for shield, mydns is the default IP of dns  
IPAddress ip(192, 168, 0, 3);
IPAddress myDns(192, 168, 0, 1);

// create Ethernet client object to use client library
EthernetClient client;

void setup() {//this setup part is from webclient aplication example of ethernet library https://github.com/arduino-libraries/Ethernet
  Serial.begin(9600);//Serial port is used for debugging and to see recieved data after debug not necessary
  while (!Serial) {
    ; // wait for serial port to connect.
  }

  // start the Ethernet connection serial lines are used for debuging
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to configure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.print("connecting to ");
  Serial.print(server);
  Serial.write("\n");
  
  // if you get a connection, report back via serial:
  if (client.connect(server, port)) {
    Serial.print("connected to ");
    Serial.println(client.remoteIP());    
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  //client.println("a");
  //delay(1000);
  //client.println("arduSekran");
  //delay(1000);
  
}

void loop() {
  int i = 0;
  while(true)
  {
    delay(1000);
    i +=1; 
    client.write("%d seconds",&i);//arduino name for subscribing to server
  }
  
}
