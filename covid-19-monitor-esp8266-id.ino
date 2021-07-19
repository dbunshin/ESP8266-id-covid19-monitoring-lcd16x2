#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const char* host = "services5.arcgis.com";
String request = "/VS6HdKS0VfIhv8Ct/arcgis/rest/services/COVID19_Indonesia_per_Provinsi/FeatureServer/0/query?where=1%3D1&outFields=Provinsi,Kasus_Posi,Kasus_Semb,Kasus_Meni&returnGeometry=false&outSR=4326&f=json";
const int httpsPort = 443;
const char fingerprint[] PROGMEM = "E2:60:C5:C1:2F:E0:B8:AB:21:05:2C:C6:7A:B6:A8:85:C2:76:0F:85"; //SHA1 finger print


// int dataCovid[11];
int dataCovid[17];
 
unsigned long previousMillis = 0;
//const long intervalGetData = 300000;
const long intervalGetData = 300000; // 5 mins

unsigned long currentMillis, sleep;
String stat;

//PIN backlight dan PIN Interrupt
#define BUTTON D3
#define BACKLIGHT D4

//fungsi untuk menyalakan kembali LCD
ICACHE_RAM_ATTR void wakeup(){
  lcd.display();
  digitalWrite(BACKLIGHT, HIGH);
//  sleep = millis()+120000;
sleep = millis()+600000; // 10 menit
}

//menampilkan data pada LCD
void tampilkanData(int c = 0){
if(c==0){
  
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("W"); delay(50); lcd.print("E"); delay(100);
  lcd.print("L"); delay(50); lcd.print("C"); delay(100);
  lcd.print("O"); delay(50); lcd.print("M"); delay(100);
  lcd.print("E"); delay(500);
  
  lcd.clear();
  lcd.setCursor(20,0); lcd.print(F("COVID-19"));
  lcd.setCursor(19,1); lcd.print(F("MONITORING"));
  
  int u;
  for(u=0;u<16;u++){
    lcd.scrollDisplayLeft();
    delay(500);
    }
  }
  else{

  //DKI JAKARTA
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("DKI "));
  lcd.print(F(">POSITIF"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[0]);
  delay(3000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("DKI "));
  lcd.print(F(">SEMBUH"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[1]);
  delay(2000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("DKI "));
  lcd.print(F(">WAFAT"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[2]);
  delay(4500);

  //JABAR
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("JABAR "));
  lcd.print(F(">POSITIF"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[3]);
  delay(1500);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("JABAR "));
  lcd.print(F(">SEMBUH"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[4]);
  delay(1500);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("JABAR "));
  lcd.print(F(">WAFAT"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[5]);
  delay(1500);

  //JATENG
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("JATENG "));
  lcd.print(F(">POSITIF"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[6]);
  delay(2500);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("JATENG "));
  lcd.print(F(">SEMBUH"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[7]);
  delay(2500);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("JATENG "));
  lcd.print(F(">WAFAT"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[8]);
  delay(2500);

  //DIY
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("YOGYA "));
  lcd.print(F(">POSITIF"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[9]);
  delay(3500);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("YOGYA "));
  lcd.print(F(">SEMBUH"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[10]);
  delay(3000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("YOGYA "));
  lcd.print(F(">WAFAT"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[11]);
  delay(3500);
/*
 //JATIM
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("JATIM "));
  lcd.print(F(">POSITIF"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[12]);
  delay(3000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("JATIM "));
  lcd.print(F(">SEMBUH"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[13]);
  delay(3000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("JATIM "));
  lcd.print(F(">WAFAT"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[14]);
  delay(3000);

  //BANTEN
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("BANTEN "));
  lcd.print(F(">POSITIF"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[15]);
  delay(3000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("BANTEN "));
  lcd.print(F(">SEMBUH"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[16]);
  delay(3000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("BANTEN "));
  lcd.print(F(">WAFAT"));
  lcd.setCursor(4,1);
  lcd.print(dataCovid[17]);
  delay(3000);
*/
  }
}

  
//fungsi untuk mengambil data dengan RESTful API dari host yang kita akses
void getData(){
  WiFiClientSecure httpsClient;    //Deklarasi object class WiFiClient
    
  Serial.println(host);
  Serial.printf("Using fingerprint '%s'\n", fingerprint);

  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(10000); // 10 Seconds
  delay(500);

  Serial.println("HTTPS Connecting");

  int r = 0; //mengulangi counter
  while ((!httpsClient.connect(host, httpsPort)) && (r < 30)) {
    delay(200);
    Serial.print(".");
    r++;
  }
  
  if (r == 30) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Connected");
  }

  Serial.print("Requesting: ");
  Serial.println(host + request);

  httpsClient.print(String("GET ") + request + " HTTP/1.1\r\n" +
                    "Host: " + host + "\r\n" +
                    "Connection: close\r\n\r\n");

  Serial.println("Request sent");

  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
  }

  Serial.println("Payload received:");

  String payload;
  while (httpsClient.available()) {
    payload = httpsClient.readStringUntil('\n');  //Membaca setiap baris data
    Serial.println(payload); //mencetak respon
  }

  Serial.println("Closing connection");
  
  //MEMBUAT JSON BARU, INI HARUS ADA KARENA JIKA TIDAK KITA AKAN SUSAH MENGAMBIL DATA YANG DIMAU KARENA TERBATASNYA LENGTH UNTUK MEMBUAT ARRAY
  int i;
  String dataCOVID = "{\"title\":\"Data Covid-19\",\"features\":[";
/* orig
/  for(i=1660-28
/  ;i<1908-1;i++){
*/

// start dki to banten
    for(i=1734-28
    ;i<2310-1; i++){ 
        
    dataCOVID += payload[i];
  
  }
  dataCOVID+="]}";
  Serial.println("JSON BARU: "+ dataCOVID);

  /*
  char charBuf[300];
  dataCOVID.toCharArray(charBuf, 300);
  */
  
  char charBuf[650];
  dataCOVID.toCharArray(charBuf, 650);
  
  const size_t capacity = JSON_ARRAY_SIZE(3) + 3*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + 3*JSON_OBJECT_SIZE(4) + 250;
  DynamicJsonDocument doc(capacity);
  
  //const char* json = "{\"title\":\"Data Covid-19\",\"features\":[{\"attributes\":{\"Provinsi\":\"DKI Jakarta\",\"Kasus_Posi\":2815,\"Kasus_Semb\":204,\"Kasus_Meni\":246}},{\"attributes\":{\"Provinsi\":\"Jawa Barat\",\"Kasus_Posi\":632,\"Kasus_Semb\":41,\"Kasus_Meni\":56}},{\"attributes\":{\"Provinsi\":\"Jawa Tengah\",\"Kasus_Posi\":304,\"Kasus_Semb\":36,\"Kasus_Meni\":41}}]}";
  
  deserializeJson(doc, dataCOVID);
  
  const char* title = doc["title"]; // "Data Covid-19"
  
  JsonArray features = doc["features"];
  
  JsonObject features_0_attributes = features[0]["attributes"];
  const char* features_0_attributes_Provinsi = features_0_attributes["Provinsi"]; // "DKI Jakarta"
  dataCovid[0] = features_0_attributes["Kasus_Posi"];
  dataCovid[1] = features_0_attributes["Kasus_Semb"];
  dataCovid[2] = features_0_attributes["Kasus_Meni"];
  
  JsonObject features_1_attributes = features[1]["attributes"];
  const char* features_1_attributes_Provinsi = features_1_attributes["Provinsi"]; // "Jawa Barat"
  dataCovid[3] = features_1_attributes["Kasus_Posi"];
  dataCovid[4] = features_1_attributes["Kasus_Semb"]; 
  dataCovid[5] = features_1_attributes["Kasus_Meni"];
  
  JsonObject features_2_attributes = features[2]["attributes"];
  const char* features_2_attributes_Provinsi = features_2_attributes["Provinsi"]; // "Jawa Tengah"
  dataCovid[6] = features_2_attributes["Kasus_Posi"];
  dataCovid[7] = features_2_attributes["Kasus_Semb"]; 
  dataCovid[8] = features_2_attributes["Kasus_Meni"]; 

  JsonObject features_3_attributes = features[3]["attributes"];
  const char* features_3_attributes_Provinsi = features_3_attributes["Provinsi"]; // "DIY"
  dataCovid[9] = features_3_attributes["Kasus_Posi"]; 
  dataCovid[10] = features_3_attributes["Kasus_Semb"]; 
  dataCovid[11] = features_3_attributes["Kasus_Meni"]; 
/*
  JsonObject features_4_attributes = features[4]["attributes"];
  const char* features_4_attributes_Provinsi = features_4_attributes["Provinsi"]; // "JATIM"
  dataCovid[12] = features_4_attributes["Kasus_Posi"]; 
  dataCovid[13] = features_4_attributes["Kasus_Semb"]; 
  dataCovid[14] = features_4_attributes["Kasus_Meni"]; 

  JsonObject features_5_attributes = features[5]["attributes"];
  const char* features_5_attributes_Provinsi = features_5_attributes["Provinsi"]; // "BANTEN"
  dataCovid[15] = features_5_attributes["Kasus_Posi"]; 
  dataCovid[16] = features_5_attributes["Kasus_Semb"]; 
  dataCovid[17] = features_5_attributes["Kasus_Meni"];  
*/
}

void setup() {
  
  //inisialisasi LCD dan tombol interrupt
  pinMode(BACKLIGHT, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  sleep = 60000;
  digitalWrite(BACKLIGHT, HIGH);
  attachInterrupt(digitalPinToInterrupt(BUTTON), wakeup, CHANGE);
 
  tampilkanData();
  
  Serial.begin(115200);
  Serial.print("Connecting");

  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("....");
  lcd.setCursor(0,1);
  lcd.print(F("NUNGGUIN WIFI"));

  //Koneksi ke WIFI, cari WIFI "COVID-19 MONITORING" lalu atur oleh anda
  WiFiManager wifiManager;  
  wifiManager.autoConnect("Discrete Mathematics");
  
  //IP Address local
  Serial.println("");  //Jika koneksi berhasil ke sebuah SSID, maka akan mencetak IP Address
  Serial.println("Connection Success ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print(F("CONNECTED"));
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print(F("NARIK DATA"));
  lcd.setCursor(2,0);
  lcd.print(F("DATA DITARIK"));
  
  //mengambil data saat pertama kali device dinyalakan
  getData();
  tampilkanData(1);
  
}

void loop() {
  currentMillis = millis();
  
  int selisih = currentMillis - previousMillis;
  
  // mengambil data setiap 5 menit
  if(currentMillis - previousMillis >= intervalGetData){

      previousMillis = currentMillis;

      //debug time
      Serial.println(currentMillis);
      
      getData();

      //menampilkan data
      tampilkanData(1);
  }

  //mematikan LCD jika device sudah meyala selama 30 detik
  // if(currentMillis > sleep) {
    // lcd.noDisplay();
    // digitalWrite(BACKLIGHT, LOW);
    // stat = "lcd Off";
  // }
  
//  Serial.println(selisih);

  //menampilkan data
  tampilkanData(1);

  
} 
