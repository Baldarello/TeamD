*
Web client

Sketch per connettere una scheda Intel Galileo come client ad un'altra utilizzata come server

created 15 May 2015
by Team Destroyer

*/

#include
#include

byte mac[] = { 0x98, 0x4F, 0xEE, 0x00, 0x5F, 0xB6 }; //Mac della scheda da usare come client
IPAddress server(192,168,100,100); // Server aka altra scheda con mac[] = { 0x98, 0x4F, 0xEE, 0x00, 0x5F, 0xCC };
IPAddress ip(192,168,100,101); //Ip del client
int led=13; //pin dove è collegato il led
int i=0; //Contatore per la lunghezza del carattere morse immesso
int contaspazio=0; //Contatore per il tempo che passa senza digitazione
char dec[]="%%%%"; //Array dove memorizzare i caratteri morse inseriti
boolean vuoto=true; //Controllo per verificare se dec è pieno o vuoto
char lettera; //Carattere di appoggio dove inserire la lettera convertita dal codice morse
int buzzer =9;

EthernetClient client; //client dove invieremo i dati

void setup() {
pinMode(buzzer,OUTPUT);

Serial.begin(9600); //Serial per debug

pinMode(led,OUTPUT); //led per verificare '.' o '_'

Ethernet.begin(mac,ip) ; // Avvio della connessione Ethernet

delay(1000);
Serial.println("Connessione in corso...");

//Comunico alla Serial se ricevo una connessione:
if (client.connect(server, 60001)) {
Serial.println("Connesso!");
//Comunico al server che il client è pronto!
client.println("Client pronto!");
client.println();
}
else {
//Se fallisce la connessione al server:
Serial.println("Connessione fallita!");
}
}

void loop()
{

while (client.available()) {
char c = client.read();

if(c!='.' && c!='' && c!='0'){ Serial.print(c);} //Scrivo su Serial eventuali comunicazioni dal server che non siano '.' '' o '0'(che corrisponde a nessun carattere digitato'

/*Prendo il carattere ricevuto dal server e lo identifico!*/
switch(c){

case '.':
digitalWrite(led,HIGH);
delay(300);
digitalWrite(led,LOW);
tone(buzzer,1000,30);
contaspazio=0;
dec[i]='.';
i++;

break;
case '':
digitalWrite(led,HIGH);
delay(500);
digitalWrite(led,LOW);
tone(buzzer,1000,80);
contaspazio=0;
dec[i]='';
i++;

break;
case '0':contaspazio++;
break;
}

if (contaspazio>=700){

 contaspazio=0;
 lettera=decoder(dec);

 for(int b=0;b<=i;b++){
   if(dec[b]=='%' && vuoto)
   { vuoto=true;}
   else
   {vuoto=false;}
 }

 if(!vuoto){
    if(lettera!='?')
    { Serial.print(lettera);}
    else{tone(buzzer,200,500);}

   for(int n=0;n<=i;n++)
   {dec[n]='%';}
   i=0;
   vuoto=true;
 }

}

}

// Se non sono connesso al server lo segnalo:
if (!client.connected()) {
Serial.println();
Serial.println("Disconnesso.");
client.stop();
// Entro in un loop per non riavviare il metodo Main/Loop
for(;;)
;
}
}

//funzione per decodificare il codice morse
char decoder(char* s){
switch (i) // i = Lunghezza del codice morse che ho digitato
{
case 1:
if (s[0]=='.')
return 'e';
if (s[0]=='')
return 't';
break;

case 2:
if (s[0]=='.' && s[1]=='')
return 'a';
if (s[0]=='.' && s[1]=='.')
return 'i';
if (s[0]=='' && s[1]=='.')
return 'n';
break;
case 3:
if (s[0]=='' && s[1]=='' && s[2]=='.')
return 'g';
if (s[0]=='' && s[1]=='.' && s[2]=='')
return 'k';
if (s[0]=='' && s[1]=='' && s[2]=='')
return 'o';
if (s[0]=='.' && s[1]=='.' && s[2]=='.')
return 's';
if (s[0]=='.' && s[1]=='.' && s[2]=='')
return 'u';

break;
case 4:
if (s[0]=='.' && s[1]=='.' && s[2]=='.' && s[3]=='.')
return 'h';

if(s[0]=='.' && s[1]=='.' && s[2]=='' && s[3]=='.')

return 'f';

if(s[0]=='.' && s[1]=='' && s[2]=='' && s[3]=='')

return 'j';

if(s[0]=='.' && s[1]=='' && s[2]=='.' && s[3]=='.')

return 'l';

if (s[0]=='.' && s[1]=='' && s[2]=='' && s[3]=='.')

return 'p';

if(s[0]=='' && s[1]=='' && s[2]=='.' && s[3]=='')

return 'q';

if(s[0]=='.' && s[1]=='.' && s[2]=='.' && s[3]=='')

return 'v';

if(s[0]=='' && s[1]=='.' && s[2]=='.' && s[3]=='')

return 'x';

if(s[0]=='' && s[1]=='.' && s[2]=='' && s[3]=='')

return 'y';

if(s[0]=='' && s[1]=='' && s[2]=='.' && s[3]=='.')

return 'z';
if(s[0]=='' && s[1]=='' && s[2]=='' && s[3]=='_')

return ' ';
break;
default :
return '?';
}
return '?'; // non ho riconosciuto nulla
}
