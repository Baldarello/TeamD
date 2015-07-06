/*
Web server

Sketch per usare una scheda galileo come server

created 15 May 2015
by Team Destroyer

*/

#include
#include

byte mac[] = { 0x98, 0x4F, 0xEE, 0x00, 0x5F, 0xCC }; //indirizzo Mac della scheda che lavora da server
IPAddress ip(192,168,1,1); //Ip del server
byte leggi=0; //byte per leggere da Serail
const int button = 11; // pushbutton 1 pin
int buttonState = 0;
int conta=0;

EthernetServer server(60001);//porta 60001

void setup() {
pinMode(button, INPUT);
// Open serial communications and wait for port to open:
Serial.begin(9600);

// Avvio della connessione Ethernet e del server:
Ethernet.begin(mac, ip);
delay(2000);
server.begin();
Serial.print("Indirizzo IP Server: ");
Serial.println(Ethernet.localIP());
}

void loop() {
//verifico la presenza di un client connesso
EthernetClient client = server.available();
if (client) {

Serial.println("Nuovo client connesso!");
  boolean currentLineIsBlank = true;
// Aspetto informazioni dal client

while (client.connected()) {
  if (client.available()) {
char c=client.read();

 Serial.write(c);
  if (c == '\n' && currentLineIsBlank) {
      //Qui posso inviare una risposta al client
  break;
    }
    if (c == '\n') {
      // Nuova linea
      currentLineIsBlank = true;
    } 
    else if (c != '\r') {
      //Ho ricevuto un carattere
      currentLineIsBlank = false;
    }
  }

}

while(1){ //Parte per inviare al client ciò che digito

 buttonState= digitalRead(button);//Lettura stato bottone

while(buttonState == LOW && conta>=0 && conta <=200){//Ciclo per identificare la pressione di un '.' o di una '_'
conta++;
buttonState= digitalRead(button);
}

if(conta > 2 && conta <=120){
Serial.print(".");
client.write("."); //Invito al client '.'

conta=0;

}else
if(conta>120)
{
Serial.print("");
client.write(""); //Invito al client '_'

conta=0;

}

client.write("0"); //comunico al client che non sto digitando nulla

   leggi = Serial.read(); //Lettura per eventuale stop del client da parte del server

switch (leggi){

 case'.':
  delay(1);
// Chiudo la connessione al client
client.write("\nIl server ha chiuso la connessione\n");
client.stop();
Serial.println("Client disconnesso manualmente!");

 break;

}
}

}
}
