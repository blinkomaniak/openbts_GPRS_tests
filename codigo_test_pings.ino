#include <SoftwareSerial.h>
#include <TimeLib.h>

SoftwareSerial mySerial(10, 11); // RX, TX
#define arraySize 128
int counter = 0;
int success_counter = 0;
int error_counter = 0;
int i;
int temp;

void setup(){ // Open serial communications and wait for port to open:
	Serial.begin(9600);
	mySerial.begin(9600);
	initGPRS();  // inicia el modulo GPRS	
}

void initGPRS(){
	connectGPRS("AT+CIPSHUT","SHUT OK");
	delay(2000);
	connectGPRS("AT+CREG=0","OK");
	delay(2000);
	connectGPRS("AT+CREG=1","OK");
	delay(2000);
	connectGPRS("AT+CGATT=1","OK");
	delay(2000);
	connectGPRS("AT+CIPSHUT","SHUT OK");
	delay(2000);
	connectGPRS("AT+CSTT=\"CMNET\"","OK");
	delay(2000);
	connectGPRS_CIICR("AT+CIICR","OK");
	delay(5000);
	connectGPRS_CIFSR("AT+CIFSR");
	delay(1000);
	connectGPRS("AT+CENG=4","OK");
	delay(1000);
}

void connectGPRS(String cmd, char *res){
	while(1){
		Serial.println(cmd);
		mySerial.println(cmd);
		delay(500);
		while(mySerial.available()>0){
			if(mySerial.find(res)){
			Serial.println(res);
			delay(2000);
			return;
		}
	}
	delay(2000);
	}
}

void connectGPRS_CIICR(String cmd, char *res){
	while(1){
		Serial.println(cmd);
		mySerial.println(cmd);
		delay(10000);
		while(mySerial.available()>0){
			if(mySerial.find(res)){
				Serial.println(res);
				delay(1000);
				return;
			}
		}
	}
}

void connectGPRS_CIFSR(String cmd){
	static char buffer[256];
	static size_t pos;
	mySerial.println(cmd);
	delay(500);
	while(mySerial.available() && pos < sizeof buffer - 1){
		char c = mySerial.read();
		buffer[pos++] = c;
		if(c == '\n'){
			buffer[pos] = '\0';
			Serial.print(buffer);
			pos = 0;
		}
	}
}

bool connectGPRS_PING(){
	bool ping_ok = true;
	char inData[arraySize];
	mySerial.println("AT+CIPPING=\"192.168.216.119\",1,1024\r");
	delay(10000);
	int c = 0;
	while(mySerial.available()){
		inData[c] = mySerial.read();
		Serial.print(inData[c]);
		c++;
	}
	Serial.println();
	if(strstr(inData, "600")){
		ping_ok = false;
	}
	else{
		ping_ok = true;
	}
	return ping_ok;
}

void connectCENG(){
	char inData2[arraySize];
	mySerial.println("AT+CENG?\r");
	delay(1000);
	int c = 0;
	while(mySerial.available()){
		inData2[c] = mySerial.read();
		Serial.print(inData2[c]);
		c++;
	}
	Serial.println();
}

void loop(){// run over and over
	Serial.println( String(hour()) + ":" + String(minute())+":"+String(second()));
	connectCENG();
	bool value = connectGPRS_PING();
	if (value == true){
		success_counter++;
	}
	else{
		error_counter++;
	}
	counter = success_counter + error_counter;
	Serial.println((String)"Total paquetes enviados: " + counter);
	Serial.println((String)"Ping errados: " + error_counter);
	Serial.println((String)"Ping correctos: " + success_counter);
	temp = random(10,50)*100;
	delay(temp);
}