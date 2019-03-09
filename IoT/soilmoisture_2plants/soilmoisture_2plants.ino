
//Library
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>;

#define DHTPIN 12     // what pin we're connected to D6
#define DHTTYPE DHT11   // DHT 11  (AM2302)
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino


int WET = 16; // Wet Indicator at Digital PIN D0
int DRY =  2; // Dry Indicator at Digital PIN D4

int sensor_Pin = 0;

int enable1 = 5;      // enable reading sensor A D1
int enable2 = 4;      // enable reading sensor B D2

int sensorValue1 = 0;  // variable to store the value coming from sensor A
int sensorValue2 = 0;  // variable to store the value coming from sensor B

// dht variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

SimpleTimer timer;
WiFiClient client;

const char* auth = " ffe182244e4a4a198d9998446c620d20";
const char* apiKey = "GIURZDOG89UZA595"; //write
const char* ssid = "moto";
const char* pass = "12345678";
const char* server = "api.thingspeak.com";

void dht_sensor()
{
  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  //Print temp and humidity values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(hum);
  Blynk.virtualWrite(V6 , hum);
  Serial.print(" %, Temp: ");
  Serial.print(temp);
  Blynk.virtualWrite(V7 , temp);
  Serial.println(" Celsius of Sensor 1.");
  delay(1000); //Delay 1 sec

  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {

    String postStr = apiKey;
    postStr += "&field3=";
    postStr += String(hum);
    postStr += "&field4=";
    postStr += String(temp);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " );
    client.print( apiKey );
    client.print( "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    delay(200);
  }

  client.stop();

  // Serial.println("Waiting...");

  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(900000);

}

void MoistureLevel()
{
  //1 SENSOR START
  Serial.print("MOISTURE LEVEL plant 1: ");
  digitalWrite(enable1, HIGH);
  sensorValue1 = analogRead(sensor_Pin);
  Serial.println(sensorValue1);
  digitalWrite(enable1, LOW);

  // Serial.println(value);
  Blynk.virtualWrite(V5 , sensorValue1);
  if (sensorValue1 < 650)              // indicates low resistance tending to zero , means water is present
  {
    digitalWrite(WET, HIGH);

  }
  else
  {
    digitalWrite(DRY, HIGH);   // indicates high resistance t, means water is required
    Blynk.notify("PLEASE ADD WATER in plant1");
  }
  digitalWrite(WET, LOW);
  digitalWrite(DRY, LOW);
  delay(3000);
  //1 SENSOR END

  //2 SENSOR START
  Serial.print("MOISTURE LEVEL plant 2: ");
  digitalWrite(enable2, HIGH);
  sensorValue2 = analogRead(sensor_Pin);
  Serial.println(sensorValue2);
  digitalWrite(enable2, LOW);

  Blynk.virtualWrite(V4 , sensorValue2);
  if (sensorValue1 < 650)              // indicates low resistance tending to zero , means water is present
  {
    digitalWrite(WET, HIGH);

  }
  else
  {
    digitalWrite(DRY, HIGH);   // indicates high resistance t, means water is required
    Blynk.notify("PLEASE ADD WATER in plant 2");
  }

  digitalWrite(WET, LOW);
  digitalWrite(DRY, LOW);

  delay(1000);
  // 2 SENSOR END


  Serial.println("Blynk complete");
  Serial.println("THINK SPEAK BEGIN");
  //   thinkspeak begins

  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
  {

    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(sensorValue1);
    postStr += "&field2=";
    postStr += String(sensorValue2);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " );
    client.print( apiKey );
    client.print( "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    delay(200);
  }

  client.stop();

  // Serial.println("Waiting...");

  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(900000);


}



void setup()
{
  Serial.begin(115200);
  pinMode(WET, OUTPUT);
  pinMode(DRY, OUTPUT);

  pinMode(enable1, OUTPUT);
  pinMode(enable2, OUTPUT);
  Blynk.begin(auth, ssid, pass);

  Serial.println("Connecting to ");
  Serial.println(ssid);


  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  //  Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //  Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  timer.setInterval(2000, MoistureLevel);
  timer.setInterval(2000, dht_sensor);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
}
