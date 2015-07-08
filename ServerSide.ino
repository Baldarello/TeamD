/*
  Web server
 
 Sketch to use Intel Galileo board as a Server.
   
 created 15 May 2015
 by Team Destroyer
 
 */

#include <SPI.h>
#include <Ethernet.h>


byte mac[] = {   0x98, 0x4F, 0xEE, 0x00, 0x5F, 0xCC }; //MAC address of the board
IPAddress ip(192,168,1,1); //Server IP address
byte reading=0; //byte to read from Serial
const int button = 11;  // pushbutton 11-pin
int buttonState = 0;
int count=0;



EthernetServer server(60001);//port 60001

void setup() {
   pinMode(button, INPUT); 
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  

  // Start the Server and Ethernet connections:
  Ethernet.begin(mac, ip);
  delay(2000);
  server.begin();
  Serial.print("Server IP address: ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  //check of the connected Client
  EthernetClient client = server.available();
  if (client) {
    
    Serial.println("New Client connected!");
      boolean currentLineIsBlank = true;
    // wait for Client informations
   
    while (client.connected()) {
      if (client.available()) {
    char c=client.read();
    
     Serial.write(c);
      if (c == '\n' && currentLineIsBlank) {
          //here you can send answer to Client
      break;
        }
        if (c == '\n') {
          // new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          //character received
          currentLineIsBlank = false;
        }
      }
   }
   
   
   while(1){ //section to send to Client what you type
   
     buttonState= digitalRead(button);//reading button state
    

  while(buttonState == LOW && count>=0 && count <=200){//Cycle to identify the press of a '.' or a '_'
    count++;
    buttonState= digitalRead(button);
  }
  
 
  
  if(count > 2 && count <=120){
    Serial.print(".");
    client.write("."); //Send '.' to Client
 
  count=0;

  }else
  if(count>120)
  {
    Serial.print("_");
    client.write("_"); //Send '_' to Client

  count=0;

 
  }
  
  client.write("0"); //signal to Client that you're not typing anything
  
  
      
       reading = Serial.read(); //reading for possible stop of the Client
   
   switch (reading){
   
     case'.':
      delay(1);
    // Close Client connection
    client.write("\nServer has closed the connection\n");
    client.stop();
    Serial.println("Client manually disconnected!");
     
     break;
   }
     } 
   
    
   
}
}
