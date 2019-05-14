#include <SoftwareSerial.h>
int midZ=2000;//time delays
int minZ=500;
int maxZ=5000;
int random_value; // parametro a ser transmitido a TheThingSpeak

SoftwareSerial MySerial(10, 11); // creacion de la coneixon serial

void setup(){
	Serial.begin(9600);
	while (!Serial);
	MySerial.begin(9600);
	delay(midZ);
	MySerial.println("at+csq"); //verificacion de parametros de senal
	delay(minZ);
	feedback();
	MySerial.println("at+cipshut");//desactivacion de cualquier conexion GPRS
	delay(minZ);
	feedback();
	MySerial.println("at+cstt=\"cmnet\",\"\",\"\"");//configuracion de APN
	feedback();
	MySerial.println("at+ciicr");//iniciar conexion con red celular
	feedback();
	MySerial.println("at+cifsr");//solicitar una IP para transmision de datos
	feedback();
}
// funcion que verifica el buffer de entrada y si encuentra  algo lo imprime.
void feedback(){
	delay(midZ);
	while (MySerial.available()) {
		Serial.write(MySerial.read());
	}
}

void loop(){ // funcion loop
	MySerial.println("at+cipstart=\"TCP\",\"api.thingspeak.com\",\"80\"");// conexion TCP
	feedback();
	MySerial.println("at+cipsend=71");//declara el numero de caracteres a enviarse
	feedback();
	MySerial.print("GET /update?");//Escribe el comando GET para enviar data al...
	feedback();
	MySerial.print("api_key=VBI484F2F3BQ4WXX");//...canal API de TheThingSpeak...
	feedback();
	MySerial.print("&field1="); //...especificando el campo (temperatura)...
	feedback();
	random_value = random(70,100);// ... el cual es un valor aleatorio entre 70 y 100...
	MySerial.println(random_value);//se cierra el comando AT+CIPSEND
	feedback();
	MySerial.println("at+cipack");//sobre la misma conexion TCP, este comando solicita al servidor
	//conocer si el ultimo envio fue exitoso o no
	feedback();
	MySerial.println("at+cipclose");//cerrar la conexion TCP
	feedback();
	delay(8000);
}