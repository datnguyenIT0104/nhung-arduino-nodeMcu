// Gọi thư viện DHT11
#include "DHT.h"

const int DHTPIN = 2;       //Đọc dữ liệu từ DHT11 ở chân 2 trên mạch Arduino
const int DHTTYPE = DHT11;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
int quangTro = A0;
int giatri = 0;
unsigned long timerDelay = 5000;
unsigned long lastTime = 0;

String server = "http://192.168.100.3:8080/addinfo";

DHT dht(DHTPIN, DHTTYPE);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();  
  delay(1000);
  
}

void loop() {
  if ((millis() - lastTime) > timerDelay ) {
    // put your main code here, to run repeatedly:
    float h = dht.readHumidity();    //Đọc độ ẩm
    float t = dht.readTemperature(); //Đọc nhiệt độ
    
    giatri = analogRead(quangTro);
    String sendDataToServer = server + "*" + "temp="+ t + "&humi=" + h + "&ldr=" + giatri;
    if( giatri > 800){
      Serial.println(sendDataToServer);
    }
    
    lastTime = millis();
  }

}
