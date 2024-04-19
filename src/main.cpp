// Version v1.02 Works with the Wemos D1 board R2 http://bit.ly/WEMOS_D1

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <LiquidCrystal.h>

/*
                        +-----+-----+--------  
             +----------| D15 | SCL | GPIO5                                            WH-061 I2c SCL 000 01234567
             |          +-----+-----+--------  
             | +--------| D14 | SDA | GPIO4                                            WH-061 I2c SDA 000 01234567 - *LCD DB4
             | |        +-----+-----+----- ---  
             | |        |     |     |
             | |        +-----+-----+--------  
             | |        | GND |     |                   
------+      | |        +-----+-----+--------  
      |      | | +------| D13 | SCK | GPIO14                                          *LCD DB5
------+      | | |      +-----+-----+--------
5v    |      | | | +----| D12 | MISO| GPIO12                                          *LCD DB6
------+      | | | |    +-----+-----+--------+------
RST   |      | | | | +--| D11 | MOSI| GPIO13 |  RX0*                                  *LCD DB7
------+      | | | | |  +-----+-----+--------+------+---------------
3v3   |      | | | | |  | D10 | SS  | GPIO15 |  TX0*| 10K Pull down                                                     Boot fails if pulled HIGH               
------+      | | | | |  +-----+-----+--------+------+---------------+--------------
5v    |      | | | | |  | D9  | TX1 | GPIO2  |      | 10K Pull up   | Built in led    LCD ENABLE                        HIGH at boot ,connected to on-board LED, boot fails if pulled LOW
------+      | | | | |  +-----+-----+--------+      +---------------+-------------
GND   |      | | | | |  | D8  |     | GPIO0  |      | 10K Pull up                     LCD RS                            connected to FLASH button, boot fails if pulled LOW 
------+      | | | | |  +-----+-----+--------+      +---------------
GND   |      | | | | |  |        
------+      | | | | |  +-----+-----+--------+    
VIN   |      | | | | +--| D7  | MOSI| GPIO13 |                                        LCD DB7  
------+      | | | |    +-----+-----+--------+    
      |      | | | +----| D6  | MISO| GPIO12 |                                        LCD DB6
------+      | | |      +-----+-----+--------+    
A0    | ADC  | | +------| D5  | SCK | GPIO14 |                                        LCD DB5
------+      | |        +-----+-----+--------+      
      |      | +--------| D4  | SDA | GPIO4  |                                        LCD DB4
------+      |          +-----+-----+--------+    
      |      +----------| D3  | SCL | GPz  |                                        
------+                 +-----+-----+--------+    
      |                 | D2  |     | GPIO16 |                                       boton comun                        Hight at boot  used to wake up from deep sleep
------+                 +-----+-----+--------+                                
      |                 | D1  | RX  | GPIO1  |                                                                          Hight at boot  boot fails if pulled LOW
------+                 +-----+-----+--------+          
      |  WEMOS D1 R1    | D0  | TX  | GPIO3  |                                                                          Hight at boot 
------+-----------------+-----+-----+--------+    



*/.

/*
#define D0  3 // GPIO3 maps to Ardiuno D0
#define D1 1 // GPIO1 maps to Ardiuno D1
#define D2 16 // GPIO16 maps to Ardiuno D2
#define D3 5 // GPIO5 maps to Ardiuno D3
#define D4 4 // GPIO4 maps to Ardiuno D4
#define D5  0 // GPIO14 maps to Ardiuno D5
#define D6 2 // GPIO12 maps to Ardiuno D6
#define D7 14 // GPIO13 maps to Ardiuno D7
#define D8 12 // GPIO0 maps to Ardiuno D8
#define D9 13 // GPIO2 maps to Ardiuno D9
#define D10 15 // GPIO15 maps to Ardiuno D10
*/
#define D0  3 // GPIO3 maps to Ardiuno D0
#define D1 1 // GPIO1 maps to Ardiuno D1
#define D2 16 // GPIO16 maps to Ardiuno D2
#define D3 5 // GPIO5 maps to Ardiuno D3
#define D4 4 // GPIO4 maps to Ardiuno D4
#define D5 14 // 0 // GPIO14 maps to Ardiuno D5
#define D6 12// 2 // GPIO12 maps to Ardiuno D6
#define D7 13 // 14 // GPIO13 maps to Ardiuno D7
#define D8 0// 12 // GPIO0 maps to Ardiuno D8
#define D9 2// 13 // GPIO2 maps to Ardiuno D9
#define D10 15 // GPIO15 maps to Ardiuno D10

//const char *ssid     = "EficentoComputingEdge";      // SSID of local network
//const char *password = "EdgeComputing013";   // Password on network
//const char *ssid     = "Fibertel Wifi 220 2.4GHZ";      // SSID of local network
//const char *password = "01411398178";   // Password on network
const char *ssid     = "papi";      // SSID of local network
const char *password = "wifipapi";   // Password on network

WiFiClient client;

String result;

int  counter = 60;

LiquidCrystal lcd(D8,D9,D4,D5,D6,D7); 


// put function definitions here:

uint8_t tecla;
uint8_t bufTecla;
unsigned int RetardoTecla =8;
unsigned long ContTecla=0;
int prueba =100;
unsigned long TiempoAhora = 0;


void espera(int periodo) {
    TiempoAhora = millis();
    while(millis() < TiempoAhora + periodo){
        
    }
 
  
}

void displayhello()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" -- Eficento -- ");
  lcd.setCursor(0,1);
  lcd.print(" LevelDispenser ");
}

void IRAM_ATTR onTimer(){
  unsigned int Repetir;
  uint8_t TeclaAux;// a global para probar
	static uint8_t TeclaAnt;
	//static unsigned long ContTecla=0;// a global para probar
	static uint8_t Nueva;
  uint8_t d4a;
  uint8_t d5a;
  uint8_t d6a;
  uint8_t d7a;
  Repetir = RetardoTecla/8;
  //digitalRead(D2);

  d4a = digitalRead(D4);
  d5a = digitalRead(D5);
  d6a = digitalRead(D6);
  d7a = digitalRead(D7);
  
  
  digitalWrite(D4,HIGH);
  digitalWrite(D5,HIGH);
  digitalWrite(D6,HIGH);
  digitalWrite(D7,HIGH);
  
 if(digitalRead(D2) == HIGH){
    pinMode ( D4, INPUT_PULLUP);
    pinMode ( D5, INPUT_PULLUP);
    pinMode ( D6, INPUT_PULLUP);
    pinMode ( D7, INPUT_PULLUP);
    pinMode(D2, OUTPUT); 
    digitalWrite(D2,LOW);
        
    TeclaAux=!digitalRead(D4)*1+!digitalRead(D5)*2+!digitalRead(D6)*4+!digitalRead(D7)*8;
    //tecla=TeclaAux;
}else TeclaAux=0;
if(TeclaAnt==TeclaAux)
{
    if(ContTecla<(RetardoTecla+Repetir)){
      ContTecla++;
    }
}
else{
    ContTecla = RetardoTecla+1;
    Nueva=1;
}
TeclaAnt=TeclaAux;
if(ContTecla==(RetardoTecla+Repetir)){
  ContTecla = RetardoTecla;
  
}
if(ContTecla==RetardoTecla)
{
  if ((TeclaAux=='*')||(TeclaAux=='#')){
    if (Nueva==0) TeclaAux=0;
  }
  if (Nueva==1) {
    ContTecla=0;
    Nueva=0;
  }
  tecla=TeclaAux;
  bufTecla=tecla;
}else  tecla=0;
pinMode(D2, INPUT_PULLDOWN_16); 
pinMode ( D4, OUTPUT);
pinMode ( D5, OUTPUT);
pinMode ( D6, OUTPUT);
pinMode ( D7, OUTPUT);
digitalWrite(D4,d4a);
digitalWrite(D5,d5a);
digitalWrite(D6,d6a);
digitalWrite(D7,d7a);



 }


void setup() {
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
//2 timers -> 0 y 1 -> 23 bits
  //timer0 -> 0  (no utilizar lo usa para wifi
  //timer1 -> 1
  //23 bits 2^23 = 8388608 / 80 da maximo que puede contar 
  //freq base = 80 MHZ -> 1us -> 80 ticks -> 104857.6 us maximo -> 0.1048576s maximo
  //Divisor 16 = freq base = 5 MHZ -> 1us -> 5 ticks -> 0.2 entre ticks -> 167721.6us maximo -> 1.6777216 s maximo
  //Divisor 256 = freq base = 31.25 Khz -> 3,2us -> 1 ticks -> 26843545.6 us maximo ->26.8 seg
  timer1_enable(TIM_DIV16, TIM_EDGE , TIM_LOOP);
  timer1_write(5000);//5000000 para un segundo 50000 andaba 10000
  timer1_attachInterrupt(onTimer);
  delay(10);
  WiFi.disconnect();
  delay(10);
  
  //int cursorPosition=0;
  
  lcd.begin(16, 2);
  lcd.print("   Connecting");
 
//Teclado recibe por boton
  //pinMode(D2, INPUT_PULLDOWN_16); 
  pinMode(D2, INPUT_PULLDOWN_16); 
  digitalWrite(D2,HIGH);
  
/*
//escanear redes cuando no funciona
Serial.println("Escaneando...");		//Letrero de escaneando
int n = WiFi.scanNetworks();		//Guardar cantidad de redes en variable n
Serial.println("Escaner terminado");	//Letrero de escáner terminado

//Si n es igual a 0 muestra que no se encontraron redes
if(n == 0){	
Serial.println("No se encontraron redes Wi-Fi"); 
}

//Si encuentra redes va a mostrar la cantidad
else{
    Serial.print(n);
  Serial.println(" Redes encontradas");

//Arreglo que me va a dar los datos de la Red
  for(int i=0;i<n;++i){
    Serial.print(i+1);
    Serial.print(": ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i));
    Serial.print(") ");
    Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
    delay(1000);
  }
}

*/
// fin de escanear redes cuando no funciona

  Serial.println("Connecting");

  Serial.print("ESP Board MAC Address:  ");

  Serial.println(WiFi.macAddress());
  delay(500);
  WiFi.mode(WIFI_STA);  		//Configurar el Node en modo STA
  delay(500);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Not connected");
    WiFi.begin(ssid, password);
  } else {
      Serial.println("Connected");
  }
  //WiFi.begin(ssid, password);
  int retries=0;
  
 
  while (WiFi.status() != WL_CONNECTED) {

    retries++;
    delay(100);
    if (retries == 750) {
      ESP.reset();
    }
    lcd.setCursor(0,1); 
    lcd.print(retries);
    lcd.setCursor(8,1); 
    lcd.print(prueba);
    Serial.println(WiFi.status());
    Serial.println(retries);
    Serial.println("Estado teclado");
    Serial.println(tecla);
    
    if (bufTecla == 4 ) {prueba--; bufTecla =0;}
    if (bufTecla == 8 ) {prueba++; bufTecla =0;}
  
    Serial.println(ContTecla);
    Serial.println(" ---- ");
    Serial.println(prueba);
  
    
  }
  //lcd.clear();
  //lcd.print("   Connected!");
  Serial.println("");
  Serial.print("Conectedo a ");
  Serial.println(ssid);
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  delay(1000);
  displayhello();
  delay(1000);
  
   lcd.setCursor(0,0);
   lcd.print("  Comenzando.   ");
   lcd.setCursor(0,1);
   lcd.print("                ");  
 
}
/*
void Parametros(){
	char SubItem=1;
	do{
		SubItem=Eleccion(Titul,SubItem,10,2);
	    if (Tecla=='*'){
		switch(SubItem)
		{
			case 1:
				VbatTope=IngresarNumero((EMVBulk),3120,4);
				if (Tecla=='*') {
					wixe(Offseteext+EMVBulk,VbatTope);
				}
				Tecla=0;
				break;
			case 2:
				IbatMax=IngresarNumero((EMIbatmax),800,1);
				if (Tecla=='*') {
				wixe(Offseteext+EMIbatmax,IbatMax);
				}
				Tecla=0;
				break;
			case 3:
				Imaxdren=IngresarNumero((EMImaxdren),800,1);
				if (Tecla=='*') {
					wixe(Offseteext+EMImaxdren,Imaxdren);
				}
				Tecla=0;
				break;
			case 4:
				Vflote=IngresarNumero((EMVflote),9999,4);
				if (Tecla=='*') {
					wixe(Offseteext+EMVflote,Vflote);
				}
				Tecla=0;

				break;
			case 5:
				Vecualiz=IngresarNumero((EMVecualiz),3120,4);
				if (Tecla=='*') {
					wixe(Offseteext+EMVecualiz,Vecualiz);
				}
				Tecla=0;

				break;
			case 6:
				Ahcarga=IngresarNumero((EMAhhab),9999,1);
				if (Tecla=='*') {
					wixe(Offseteext+EMAhhab,Ahcarga);
				}
				Tecla=0;

				break;
			case 7:
				k=IngresarNumero((EMQmetro),9999,1);
				if (Tecla=='*') {
					wixe(Offseteext+EMQmetro,k);
				}
				Tecla=0;

				break;

			case 8:
				VconMax=IngresarNumero((EMVmaxDes),3120,4);
				if (Tecla=='*') {
					wixe(Offseteext+EMVmaxDes,VconMax);
				}
				Tecla=0;
				break;
			case 9:
				VconMin=IngresarNumero((EMVminDes),3120,4);
				if (Tecla=='*') {
					wixe(Offseteext+EMVminDes,VconMin);
				}
				Tecla=0;
			break;
			case 10:
				Tempcomp=IngresarNumero((EMTempcomp),570,0);
				if (Tecla=='*') {
					wixe(Offseteext+EMTempcomp,Tempcomp);
				}
				Tecla=0;
			break;
			}
	    }
	}while(Tecla!='#');

}


// ***************************************************************
//Subrutina Eleccion: Muestra datos en menu desplegable formato tipo
//tipo 	2:Menu principal u otro en el cual lee de la memoria serial
//	  externa
//	2.Titulo 	referencia a memoria en vectores de 16 bytes
//	2.ItemtT	numero de item actual
//	2.Items		numero total de items
//	2.accion.1 	linea sup muestra la linea Titulo
//	2.accion.2 	linea inf muestra la linea Titulo+itemT
//	2.accion.3	valor(int) almacenado en esa linea mas 9
//	        	sacando la precision de la pos anterior+2
//	2.accion.4	con cursor maneja ItemT
//			otra tecla/s sale y devuelve itemT sin
//			alterar el valor de tecla apretada
//tipo 	4:Idem 2
//tipo 	4.accion.1      Imprime linea de ayuda (16)
//			F Edita Ent Elige
// ***************************************************************
// Eleccion(Titul,SubItem,10,2);
char Eleccion(char Titulo,char ItemT,char Items,char Tipo){
	int mem;
	Cursor_XY(1,1);
	if (Tipo==4){
		ImprimirLinea(16);
	}else ImprimirLinea(Titulo);
	do {
		Tecla=0;
		Cursor_XY(1,2);
		if (Tipo==4) {
			Cursor_XY(6+((Titulo==20)||(Titulo==23)),2);
	//		Cursor_XY(6+((Titulo==20)||(Titulo==23)),2);
			Disp_Dato(48+ItemT);
		}else ImprimirLinea(Titulo+ItemT);
		Cursor_XY(16,1);
		if (Items==1) Disp_Dato(32);
		if (ItemT==Items) Disp_Dato(179);
		if (ItemT==1) Disp_Dato(180);
		if (Tipo==2) {
			Cursor_XY(11,2);
			Disp_Dato(' ');
			Disp_Dato(' ');
		//	Cursor_XY(8+((ItemT==9)||(ItemT==8))*2,2);
		Cursor_XY(8,2);
			mem=Offseteext+9+(Titulo+ItemT)*16;
			ImprimirNum(rixe(mem),0,rixe(mem+2));
		}
		if (Tipo==4) {
		//	Cursor_XY(1,2); //sacar
		//	ImprimirNum(Titulo,0,0);
		//	Disp_Dato(' ');
			Cursor_XY(8,2);
			if (Titulo==(23)){
				Disp_Dato(' ');
				if (rixe(Offseteext+Titulo*16-1+ItemT*2)<1000) {Disp_Dato(' ');}
			}
	//		ImprimirNum(rixe(Offseteext+Titulo*16-1+ItemT*2),0,1+(Titulo!=(20)));
			ImprimirNum(rixe(Offseteext+Titulo*16-1+ItemT*2),0,1+(Titulo!=(20))-2*(Titulo==(23)));
		}
		do {
	  		Cursor_XY(16,1);
			if ((ItemT>1)&&(ItemT<Items)) Disp_Dato(179+(segh>4));
			Tecla=EscanearTeclado(1000);
			if (Tecla=='2'){
				if (ItemT>1) ItemT=ItemT-1;
			}
			if (Tecla=='8'){
				if (ItemT<Items) ItemT=ItemT+1;
			}
			if  ((Tecla=='*')||(Tecla=='E')){
				return ItemT;
			}
		} while(Tecla==0);
	} while(Tecla!='#');
	return ItemT;
}
*/
void loop() {
  
   if(counter == 60) //Get new data every 10 minutes
    {
      counter = 0;
     
      
  
  
    }else
    {
      counter++;
     
      lcd.setCursor(0,1);
      lcd.print(counter); 
      
      
      
    
    }
    espera(prueba);
    if (bufTecla == 4 ) {prueba--; bufTecla =0;}
    if (bufTecla == 8 ) {prueba++; bufTecla =0;}
    lcd.setCursor(8,1);
      lcd.print(prueba); 
   
 

 
}

