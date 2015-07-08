/*
  Web client
 
 Sketch to connect Intel Galileo Client board to Server board
   
 created 15 May 2015
 by Team Destroyer
 
 */

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {  0x98, 0x4F, 0xEE, 0x00, 0x5F, 0xB6 }; //MAC address of the board
IPAddress server(192,168,100,100);
IPAddress ip(192,168,100,101); //Client IP address
int led=13; //pin where the led is connected
int i=0; //counter for lenght of morse character put in
int countSpace=0; //counter for time pass without typing
char dec[]="%%%%"; //Array to stora morse characters put in
boolean empty=true; //Control to check if dec is full or empty
char letter; //support character where put in the converted letter from code morse
int buzzer =9;

EthernetClient client; //client to send data

void setup() {
pinMode(buzzer,OUTPUT);

  Serial.begin(9600); //Serial to debug
  
   pinMode(led,OUTPUT); //led to check '.' or '_'


 Ethernet.begin(mac,ip) ;  // start of Ethernet connection
  
  delay(1000);
  Serial.println("connection in progress...");

  //signal to Serial if a connected is received:
  if (client.connect(server, 60001)) {
    Serial.println("Connected!");
    //signal to Server that the Client is ready! 
    client.println("Client ready!");
    client.println();
  } 
  else {
    //if Server connection fail:
    Serial.println("Connessione failed!");
  }
}

void loop()
{

  while (client.available()) {
    char c = client.read();
    
   if(c!='.' && c!='_' && c!='0'){ Serial.print(c);} //write to Serial possible Server communication which are not '.' '_' o '0'(which corrisponds to no character typed)
    
    
    /*take received character from Server and identify this*/
    switch(c){
      
    case '.':
    digitalWrite(led,HIGH);
    delay(300);
    digitalWrite(led,LOW);
    tone(buzzer,1000,30);
    countSpace=0;
    dec[i]='.';
    i++;
    
  break;
 case '_':
    digitalWrite(led,HIGH);
    delay(500);
    digitalWrite(led,LOW);
    tone(buzzer,1000,80);
    countSpace=0;
    dec[i]='_';
    i++;
    
  break;
  case '0':countSpace++;
   break;
}
  
  if (countSpace>=700){
    
     countSpace=0;
     letter=decoder(dec);
    
     for(int b=0;b<=i;b++){
       if(dec[b]=='%' && empty)
       { empty=true;}
       else
       {empty=false;}
     }
     
     if(!empty){
        if(letter!='?')
        { Serial.print(letter);}
        else{tone(buzzer,200,500);}
        
       for(int n=0;n<=i;n++)
       {dec[n]='%';}
       i=0;
       empty=true;
     }
  }

  }

  // signal if Client is not connected to Server:
  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnected.");
    client.stop();
    // Enter in a loop to non reboot Main/Loop method
    for(;;)
      ;
  }
}


//function to decode morse code
char decoder(char* s){
switch (i) // i = lenght of the typed code morse
{
case 1:
if (s[0]=='.')
return 'e';
if (s[0]=='_')
return 't';
break;         
case 2:
if (s[0]=='.' && s[1]=='_')
return 'a';
if (s[0]=='.' && s[1]=='.')
return 'i';
if (s[0]=='_' && s[1]=='.')
return 'n';
break;
case 3:
if (s[0]=='_' && s[1]=='_' && s[2]=='.')
return 'g';
if (s[0]=='_' && s[1]=='.' && s[2]=='_')
return 'k';
if (s[0]=='_' && s[1]=='_' && s[2]=='_')
return 'o';
if (s[0]=='.' && s[1]=='.' && s[2]=='.')
return 's';
if (s[0]=='.' && s[1]=='.' && s[2]=='_')
return 'u';                  
break;
case 4:
if (s[0]=='.' && s[1]=='.' && s[2]=='.' && s[3]=='.')
return 'h';                       
if(s[0]=='.' && s[1]=='.' && s[2]=='_' && s[3]=='.')                       
return 'f';                     
if(s[0]=='.' && s[1]=='_' && s[2]=='_' && s[3]=='_')                       
return 'j';                      
if(s[0]=='.' && s[1]=='_' && s[2]=='.' && s[3]=='.')                     
return 'l';                      
if (s[0]=='.' && s[1]=='_' && s[2]=='_' && s[3]=='.')     
return 'p';                      
if(s[0]=='_' && s[1]=='_' && s[2]=='.' && s[3]=='_')                    
return 'q';                      
if(s[0]=='.' && s[1]=='.' && s[2]=='.' && s[3]=='_')                    
return 'v';                      
if(s[0]=='_' && s[1]=='.' && s[2]=='.' && s[3]=='_')                    
return 'x';                
if(s[0]=='_' && s[1]=='.' && s[2]=='_' && s[3]=='_')                    
return 'y';                      
if(s[0]=='_' && s[1]=='_' && s[2]=='.' && s[3]=='.')                    
return 'z';
if(s[0]=='_' && s[1]=='_' && s[2]=='_' && s[3]=='_')                    
return ' ';
break;
default :
return '?';
}
return '?';    // nothing recognized
}

