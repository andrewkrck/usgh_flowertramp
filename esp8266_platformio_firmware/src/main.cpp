#include <I2Cdev.h>
#include <MPU9250_9Axis_MotionApps41.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

MPU9250 mpu;

#define SDA 4
#define SCL 5

// #define LED_PIN 4

//#define head
#define pelvis
//#define spine
//#define thigh_l
//#define calf_l
//#define foot_l
//#define thigh_r
//#define calf_r
//#define foot_r
//#define upperarm_l
//#define lowerarm_l
//#define hand_l
//#define upperarm_r
//#define lowerarm_r
//#define hand_r

// config static IP
/*
head		192.168.50.24
pelvis		192.168.50.21
spine		192.168.50.22
thigh_l		192.168.50.30	x, -z, y
calf_l		192.168.50.32
foot_l		192.168.50.34
thigh_r		192.168.50.31
calf_r		192.168.50.33
foot_r		192.168.50.35
upperarm_l	192.168.50.40
lowerarm_l	192.168.50.42
hand_l		192.168.50.44	-x, -z, -y
upperarm_r	192.168.50.41
lowerarm_r	192.168.50.43
hand_r		192.168.50.45	x, -z, y
*/

#ifdef head
IPAddress ip(10, 10, 10, 24);
const char* topic = "adam/head";
String ESPname = "head";
float blend_w = 0.7217;
float blend_x = 0.6922;
float blend_y = -0.0000;
float blend_z = 0.0000;
//0.7217	0.6922	-0.0000	0.0000
#endif

#ifdef pelvis
IPAddress ip(10, 10, 10, 21);
const char* topic = "adam/pelvis";
String ESPname = "pelvis";
float blend_w = 0.8194;
float blend_x = 0.5733;
float blend_y = -0.0000;
float blend_z = 0.0000;
//0.8194	0.5733	-0.0000	0.0000
#endif

#ifdef spine
IPAddress ip(10, 10, 10, 22);
const char* topic = "adam/spine";
String ESPname = "spine";
float blend_w = 0.7607;
float blend_x = 0.6491;
float blend_y = -0.0000;
float blend_z = 0.0000;
//0.7607	0.6491	-0.0000	0.0000
#endif

#ifdef thigh_l
IPAddress ip(10, 10, 10, 30);
const char* topic = "adam/thigh_l";
String ESPname = "thigh_l";
float blend_w = 0.6585;
float blend_x = -0.7207;
float blend_y = -0.1584;
float blend_z = 0.1480;
//0.6585	-0.7207	-0.1584	0.1480
#endif

#ifdef calf_l
IPAddress ip(10, 10, 10, 32);
const char* topic = "adam/calf_l";
String ESPname = "calf_l";
float blend_w = 0.7165;
float blend_x = -0.6973;
float blend_y = -0.0064;
float blend_z = -0.0197;
//0.7165	-0.6973	-0.0064	-0.0197
#endif

#ifdef foot_l
IPAddress ip(10, 10, 10, 34);
const char* topic = "adam/foot_l";
String ESPname = "foot_l";
float blend_w = 0.2316;
float blend_x = -0.9714;
float blend_y = 0.0032;
float blend_z = -0.0528;
//0.2316	-0.9714	0.0032	-0.0528
#endif

#ifdef thigh_r
IPAddress ip(10, 10, 10, 31);
const char* topic = "adam/thigh_r";
String ESPname = "thigh_r";
float blend_w = 0.6585;
float blend_x = -0.7207;
float blend_y = 0.1584;
float blend_z = -0.1480;
//0.6585	-0.7207	0.1584	-0.1480
#endif

#ifdef calf_r
IPAddress ip(10, 10, 10, 33);
const char* topic = "adam/calf_r";
String ESPname = "calf_r";
float blend_w = 0.7165;
float blend_x = -0.6973;
float blend_y = 0.0064;
float blend_z = 0.0197;
//0.7165	-0.6973	0.0064	0.0197
#endif

#ifdef foot_r
IPAddress ip(10, 10, 10, 35);
const char* topic = "adam/foot_r";
String ESPname = "foot_r";
float blend_w = 0.2316;
float blend_x = -0.9714;
float blend_y = -0.0032;
float blend_z = 0.0528;
//0.2316	-0.9714	-0.0032	0.0528
#endif

#ifdef upperarm_l
IPAddress ip(10, 10, 10, 40);
const char* topic = "adam/upperarm_l";
String ESPname = "upperarm_l";
float blend_w = 0.5151;
float blend_x = 0.4667;
float blend_y = 0.5069;
float blend_z = -0.5099;
//0.5151	0.4667	0.5069	-0.5099
#endif

#ifdef lowerarm_l
IPAddress ip(10, 10, 10, 42);
const char* topic = "adam/lowerarm_l";
String ESPname = "lowerarm_l";
float blend_w = 0.5357;
float blend_x = 0.4798;
float blend_y = 0.4171;
float blend_z = -0.5557;
//0.5357	0.4798	0.4171	-0.5557
#endif

#ifdef hand_l
IPAddress ip(10, 10, 10, 44);
const char* topic = "adam/hand_l";
String ESPname = "hand_l";
float blend_w = 0.2866;
float blend_x = 0.6757;
float blend_y = 0.5392;
float blend_z = -0.4130;
//0.2866	0.6757	0.5392	-0.4130
#endif

#ifdef upperarm_r
IPAddress ip(10, 10, 10, 41);
const char* topic = "adam/upperarm_r";
String ESPname = "upperarm_r";
float blend_w = 0.5151;
float blend_x = 0.4667;
float blend_y = -0.5069;
float blend_z = 0.5099;
//0.5151	0.4667	-0.5069	0.5099
#endif

#ifdef lowerarm_r
IPAddress ip(10, 10, 10, 43);
const char* topic = "adam/lowerarm_r";
String ESPname = "lowerarm_r";
float blend_w = 0.5357;
float blend_x = 0.4798;
float blend_y = -0.4171;
float blend_z = 0.5557;
//0.5357	0.4798	-0.4171	0.5557
#endif

#ifdef hand_r
IPAddress ip(10, 10, 10, 45);
const char* topic = "adam/hand_r";
String ESPname = "hand_r";
float blend_w = 0.2866;
float blend_x = 0.6757;
float blend_y = -0.5392;
float blend_z = 0.4130;
//0.2866	0.6757	-0.5392	0.4130
#endif


IPAddress gateway(10, 10, 10, 1);
IPAddress subnet(255, 255, 255, 0);

const char* ssid = "kharlashkin";
const char* password = "1123581321345589";
const char* mqtt_server = "10.10.10.1";
String messageMQTT;

WiFiClient espClient;
PubSubClient clientIn(espClient);

uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[48]; // FIFO storage buffer
int16_t mag[3];
float yaw[3];         // данные магнитометра после преобразования согласно даташиту
float d_mag[4];
bool blinkState = false;
bool sendQuat = false;
bool sendVec = false;
bool calib = false;
bool ready = false;
int count = 0;
unsigned long start_time = 0;
float diff_q = 0.0f;
float diff_v = 0.0f;
float G = 9.807f;
VectorFloat accel;
VectorFloat v_in;
VectorFloat v_mid;
VectorFloat v_out;
VectorFloat v_accel;
VectorInt16 raw;
VectorFloat gravity;
VectorFloat v_gravity;
VectorFloat corr;
VectorFloat v_corr;


Quaternion q;           // [w, x, y, z]         quaternion container
Quaternion q_in;
Quaternion q_out;
Quaternion q_out_conj;
Quaternion q_corr;
Quaternion q_mid;
Quaternion q_mid_conj;
Quaternion q_corr_conj;
Quaternion q_diff;
Quaternion q_blend;
Quaternion q_blend_conj;
Quaternion q_ident;
Quaternion q_conj;
Quaternion q_accel;

void setup_wifi() {
  delay(10);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  for (int i = 0; i < length; i++) {
    messageMQTT += (char)payload[i];
  }
    if (messageMQTT == "start"){
	sendQuat = true;
	messageMQTT = "";
  }
  else if (messageMQTT == "stop") {
	sendQuat = false;
	messageMQTT = "";
}
else if (messageMQTT == "calib") {
	calib = true;
	messageMQTT = "";
}
  else if (messageMQTT == "sleep") {
  	Serial.println(messageMQTT);
  	messageMQTT = "";
	digitalWrite(LED_BUILTIN, LOW);
	mpu.setSleepEnabled(true);
	ESP.deepSleep(0);
 }
}

void reconnectIn() {
  while (!clientIn.connected()) {
    String clientId = ESPname;
    if (clientIn.connect(clientId.c_str())) {
      clientIn.subscribe("main");
    }
  }
}

void setup() {
  q_blend.w = blend_w;
  q_blend.x = blend_x;
  q_blend.y = blend_y;
  q_blend.z = blend_z;
  q_blend_conj = q_blend.getConjugate();

  Wire.begin(SDA, SCL);
  Wire.setClock(400000);
  Serial.begin(115200);
  setup_wifi();
  clientIn.setServer(mqtt_server, 1883);
  clientIn.setCallback(callback);
  // client.subscribe("main");

  mpu.initialize();
  mpu.dmpInitialize();
  mpu.setDMPEnabled(true);
  packetSize = mpu.dmpGetFIFOPacketSize();
  fifoCount = mpu.getFIFOCount();
  ArduinoOTA.onStart([]() {
    //Serial.println("Start");  //  "Начало OTA-апдейта"
  });
  ArduinoOTA.onEnd([]() {
    //Serial.println("\nEnd");  //  "Завершение OTA-апдейта"
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    /*Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    //  "Ошибка при аутентификации"
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    //  "Ошибка при начале OTA-апдейта"
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    //  "Ошибка при подключении"
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    //  "Ошибка при получении данных"
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    //  "Ошибка при завершении OTA-апдейта"*/
  });
  ArduinoOTA.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  start_time = millis();
  reconnectIn();
}

void loop() {
	clientIn.loop();
	ArduinoOTA.handle();
   	fifoCount = mpu.getFIFOCount();
  	if (fifoCount == 1024) {
    	mpu.resetFIFO();
  		}
  	else if (fifoCount % packetSize != 0) {
      	mpu.resetFIFO();
    	}
    else if (fifoCount >= packetSize) {
       	mpu.getFIFOBytes(fifoBuffer, packetSize);
       	fifoCount -= packetSize;
       	mpu.dmpGetQuaternion(&q, fifoBuffer);
       	mpu.dmpGetAccel(&raw, fifoBuffer);
       	mpu.dmpGetAccelFloat(&accel, &raw);
       	v_accel = v_corr.getRotated(&q_mid_conj);
    	//v_accel = accel.getDifference(v_accel);
    	v_out = v_accel.getDifference(accel);
    	v_out = v_out.getRotated(&q_mid);
    	//v_accel = v_accel.getRotated(&q_out);
       	//q_kryl = q_kryl.getProduct(q_kryl_conj);
       	//q_kryl = q_kryl.getProduct(q_kryl_conj);
       	//q_kryl = q_kryl_conj.getProduct(q_kryl);
       	//q_kryl = q_kryl_conj.getProduct(q_kryl);
       	//q_kryl = q_kryl.getNormalized();
       	//Serial.write((uint8_t*)&q_kryl, sizeof(q_kryl));
       	//accel = accel.getNormalized();
       	/*Serial.print(accel.x, 6);
       	Serial.print(" ");
       	Serial.print(accel.y, 6);
       	Serial.print(" ");
       	Serial.println(accel.z, 6);*/
       	//mpu.dmpGetKrylov(kryl, &q);
       	/*Serial.print(gravity.x, 6);
       	Serial.print(" ");
       	Serial.print(gravity.y, 6);
       	Serial.print(" ");
       	Serial.print(gravity.z, 6);
       	Serial.print(" ");
       	Serial.print(accel.x, 6);
       	Serial.print(" ");
       	Serial.print(accel.y, 6);
       	Serial.print(" ");
       	Serial.println(accel.z, 6);*/
       	q_diff = q.getDifference(q_in);
       	diff_q = q_diff.getMagnitude();
   	    if (diff_q <= 0.01 && calib) {
   	    	q_corr_conj = q.getAxis().getConjugate();
   	    	//q_corr_conj = q_corr_conj.getAxis();;
   	    	//q_corr = q_blend.getProduct(q_corr_conj);
   	    	//q_conj = q;
   	    	q_ident = q.getProduct(q_corr_conj);
   	    	mpu.dmpGetGravity(&gravity, &q_ident);
       		v_gravity = gravity.getRotated(&q);
       		v_corr = accel.getDifference(v_gravity);
       		v_corr = v_gravity.getSumm(v_corr);
       		//v_corr = v_corr.getRotated(&q_corr_conj);
   	    	//q_corr = q_corr_conj.getAxis();
   	    	ready = true;
       		calib = false;
       	}
		else if (diff_q >= 0.01 && sendQuat) {
			q_mid = q.getAxis();
			q_out = q_corr_conj.getProduct(q_mid);
			q_out = q_out.getConjRotated(q_blend);
			//q_out = q_corr.getProduct(q_mid);
    		//q_mid_conj = q_mid.getConjugate();
    			//accel = accel.getRotated(&q_out);
       		//q_out = q_mid.getRotated(q_blend);
       		//q_out = q_out.getAxis();
       		clientIn.publish(topic, (uint8_t*)&q_out, sizeof(q_out));
       		/*v_out.z = 0.0f;
       		diff_v = v_out.getMagnitude();
       		if (diff_v >= 0.0005 && ready) {
       				v_out = v_out.multiplByNumber(G);
       					v_mid = v_out.getSpeed();
       					v_mid = v_mid.getSumm(v_out);
       					v_in = v_mid.getRoundThous();
    					q_accel.w = 0.0f;
    					q_accel.x = v_in.x;
    					q_accel.y = v_in.y;
    					clientIn.publish("adam/root", (uint8_t*)&q_accel, sizeof(q_accel));
    					//sendVec = true;
    				}
    		else if (diff_v < 0.0005 && sendVec){
    					q_accel.w = 0.0f;
    					q_accel.x = 0.0f;
    					q_accel.y = 0.0f;
    					q_accel.z = 0.0f;
    					clientIn.publish("adam/root", (uint8_t*)&q_accel, sizeof(q_accel));
    					//sendVec = false;
    			}*/
   			blinkState = !blinkState;
           	digitalWrite(LED_BUILTIN, blinkState);
			q_in = q;
		}
		/*else if (ready && sendVec) {
    			q_accel.w = 0.0f;
    			q_accel.x = 0.0f;
    			q_accel.y = 0.0f;
    			q_accel.z = 0.0f;
    			clientIn.publish("adam/root", (uint8_t*)&q_accel, sizeof(q_accel));
    			sendVec = false;
    			}*/
		mpu.resetFIFO();
	}
}