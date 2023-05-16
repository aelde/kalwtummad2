#include <WiFiClientSecure.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <WiFi.h>

int led = 18;
char msg[150];

// MQTT netpie device 3 **********************
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "";
const char* mqtt_username = "";
const char* mqtt_password = "";

WiFiClient espClient;
PubSubClient client(espClient);

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting NETPIE2020 connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("NETPIE2020 connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

// mpu 6050
float RateRoll, RatePitch, RateYaw;
float AccX, AccY, AccZ;
float AngleRoll, AnglePitch;
float LoopTimer;

double x = 0, y = 0, z = 0;

int pot = 34;
int a = 0;

// mp6050 setup
void gyro_signals(void) {
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6);
  int16_t AccXLSB = Wire.read() << 8 | Wire.read();
  int16_t AccYLSB = Wire.read() << 8 | Wire.read();
  int16_t AccZLSB = Wire.read() << 8 | Wire.read();
  Wire.beginTransmission(0x68);
  Wire.write(0x1B);
  Wire.write(0x8);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 6);
  int16_t GyroX = Wire.read() << 8 | Wire.read();
  int16_t GyroY = Wire.read() << 8 | Wire.read();
  int16_t GyroZ = Wire.read() << 8 | Wire.read();
  RateRoll = (float)GyroX / 65.5;
  RatePitch = (float)GyroY / 65.5;
  RateYaw = (float)GyroZ / 65.5;
  AccX = (float)AccXLSB / 4096;
  AccY = (float)AccYLSB / 4096;
  AccZ = (float)AccZLSB / 4096;
  AngleRoll = atan(AccY / sqrt(AccX * AccX + AccZ * AccZ)) * 1 / (3.142 / 180);
  AnglePitch = -atan(AccX / sqrt(AccY * AccY + AccZ * AccZ)) * 1 / (3.142 / 180);
}
// *************************************************************/

// Your WiFi credentials.
// char ssid[] = "SIAM_WIFI";
// char pass[] = "";

void setup() {
  Serial.begin(115200);
  pinMode(led,OUTPUT);
  // mpu6050 setup
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  Serial.begin(115200);
  Serial.println("Starting...");
  if (WiFi.begin(ssid, password)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // MQTT netpie **********************
  client.setServer(mqtt_server, mqtt_port);
  digitalWrite(led,1);
  // ***********************
}
void loop()
{   
    digitalWrite(led,1);
    gyro_signals();

    double xx = map(AccX,-1,1,-255,255);
    double yy = map(AccY,-1,1,-255,255);
    double zz = map(AccZ,-1,1,-255,255);

    x = AccX*255;
    y = AccY*255;
    z = AccZ*255;

    if (!client.connected()) {
      digitalWrite(led,0);
    reconnect();
    }
    client.loop();

    // String data = "{\"data\": {\"xValue\":" + String(x) + ", \"yValue\":" + String(x) + "}}";
    // String data = "{\"x\": ["String(x)", "String(y)"]}";
    // String data = "{\"x\":" + String(x) + ", \"y\":" + String(y) + "}";

    // Serial.println(data);
    // data.toCharArray(msg, (data.length() + 1));
    // client.publish("@shadow/data/update", msg);
    // client.publish("@msg/test3", msg);

    
    client.publish("@msg/test",String(x).c_str());
    // client.publish("@msg/test","x222");
    delay(10);
    client.publish("@msg/test2",String(y).c_str());
    // client.publish("@msg/test2","y333");

    // Serial.println("Hello Netpie2020");
    delay(250); 

    Serial.print("Acceleration X [g]= ");
    // Serial.print(AccX);
    // Serial.print(xx);
    Serial.print(x);
    Serial.print(" Acceleration Y [g]= ");
    // Serial.print(AccY);
    // Serial.print(yy);
    Serial.println(y);
    // Serial.print(" Acceleration Z [g]= ");
    // Serial.print(AccZ);
    // Serial.print(zz);
    // Serial.print(z);
    // Serial.print(" AngleRoll= ");
    // Serial.print(AngleRoll);
    // Serial.print(" AnglePitch= ");
    // Serial.print(AnglePitch);

    // a = analogRead(pot);
    // Serial.print("   a = ");
    // Serial.println(a);

    

    // Serial.println("dis: "+String(distance));
    delay(50);

}