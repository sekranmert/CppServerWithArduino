 #include <Ethernet.h>
#include <LiquidCrystal.h>

//Mac adress for ethernet shield this one is default for Wiznet W5100
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
int port = 9999; 
IPAddress server(13,59,27,29);  // Publıc IP of the server,

//ip is static IP address for shield, mydns is the default IP of dns  
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);

//create Ethernet client object to use client library
EthernetClient client;

//create lcd object
LiquidCrystal lcd = LiquidCrystal(2, 3, 4, 5, 6, 7);

int Li          = 16;
int Lii         = 0;

void setup() {//this setup part is from webclient aplication example of ethernet library https://github.com/arduino-libraries/Ethernet
  lcd.begin(16, 2);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12,OUTPUT);

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

  client.write("arduino");//arduino name for connectimg to server
  Serial.println("name sent");
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  int len = client.available();
  if (len > 0) {
    if(len > 100) len = 100;
    
    Serial.println("recieving...");
    char buffer[100];
    client.read(buffer, len);
    Serial.write(buffer,len);
    Serial.write("\n"); 

    if (len>1){/* if recieved message longer than 1 char it is a message to
      print on lcd*/
      lcd.setCursor(0,0);
      lcd.print("Incoming Message");
      lcd.setCursor(0,1);
      String result;
      String dummy = buffer;
      String StrProcess = "                "+ dummy+"                ";
      while(Li<StrProcess.length()){// a loop for left scrolling text
        result = StrProcess.substring(Li,Lii);
        Li++;
        Lii++;
        lcd.setCursor(0,1);
        lcd.print(result);
        delay(300);  
      }
      lcd.setCursor(0,0);
      lcd.print("   No Message   ");
      lcd.setCursor(0,1);
      lcd.print("                ");
      Li=16;
      Lii=0;
    }
    // if messsage has 1 char "a" then set led 1 on
    else if (buffer[0]==97){
      digitalWrite(8, HIGH);      
    }
    // if messsage has 1 char "b" then set led 1 off
    else if(buffer[0]==98){
      digitalWrite(8, LOW); 
    }
    // if messsage has 1 char "b" then set led 2 on
    else if (buffer[0]==99){
      digitalWrite(9, HIGH); 
    }
    // if messsage has 1 char "d" then set led 2 off
    else if(buffer[0]==100){
      digitalWrite(9, LOW); 
    }
    // if messsage has 1 char "e" then send led status
    else if (buffer[0]==101){
      String stat="";
      if (digitalRead(10)== HIGH){
        stat+="led 1 set high\n";
      }
      else if(digitalRead(10)== LOW){
        stat+="led 1 set low\n";
      }
      if (digitalRead(11)== HIGH){
        stat+="led 2 set high\n";
      }
      else if(digitalRead(11)== LOW){
        stat+="led 2 set low\n";
      }
      client.println(stat);
    }
    // if messsage has 1 char "f" then set buzzer on
    else if (buffer[0]==102){
      for(int i = 5;i>0;i--);
        digitalWrite(12, HIGH);
        delay(100);
        digitalWrite(12, LOW);
        delay(100);
      }
    else{
      Serial.println("command not found");
    }
  }
  //the server disconnected
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    while(1){
    // do nothing
    }
  }
}
