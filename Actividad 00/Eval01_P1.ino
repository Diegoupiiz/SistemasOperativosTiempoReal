/*  En este código se obtiene una conexión WiFi con el ESP32.
    Los datos obtenidos de la fecha y hora, así como la dis-
    tancia adquirida con el ultrasónico son enviados al fire-
    base. */
/*Aplicación sin FreeRTOS */
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <HTTPClient.h>
#include <time.h>

//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
//#define WIFI_SSID "TP-Link_953A"
//#define WIFI_PASSWORD "52232473"
#define WIFI_SSID "INFINITUM_2.4_EXT"
#define WIFI_PASSWORD "Contraseña"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyCFY5e-awdYnE1QDdq6eT_7RqUUleALH-M"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://esp32-8235e-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "emilianosilva14@gmail.com"
#define USER_PASSWORD "Password"

const char WiFi_SSID[] = "INFINITUM_2.4_EXT"; //Red a la que se conecta
const char WiFi_PASSWORD[] = "Contraseña"; // Contraseña

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;


/*----------------------------------------*/
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;
/*----------------------------------------*/

/*--------------Ultrasónico------------ */
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const int trigPin = 14;
const int echoPin = 2;

long duration;
float distanceCm;
float distanceInch;
/*----------------------------------------*/

/*------------------Fecha---------------- */
String fecha = "0";
/*----------------------------------------*/

/*----------------------------------------*/
void GetDate(void); 
void ReadUltrasonico(void);
void FireBase(void);
/*----------------------------------------*/

// Variable de control
int control = 0;

void setup()
{
  Serial.begin(115200);   // Baudios
  WiFi.begin(WiFi_SSID, WiFi_PASSWORD);   // Conecta a WiFi
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  control = 1;
  Serial.println("");
  Serial.print("Conectando con la siguiente dirección IP: ");
  Serial.println(WiFi.localIP());
  pinMode(trigPin, OUTPUT); // Configuración de 
  pinMode(echoPin, INPUT); // Configuración de echo
  digitalWrite(trigPin, LOW);

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;

    /* Assign the callback function for the long running token generation task */
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

    //Or use legacy authenticate method
    //config.database_url = DATABASE_URL;
    //config.signer.tokens.legacy_token = "<database secret>";

    Firebase.begin(&config, &auth);

    //Comment or pass false value when WiFi reconnection will control by your code or third party library
    Firebase.reconnectWiFi(true);

  }


void loop()
{
        while(1){
          GetDate();
          ReadUltrasonico();
          FireBase();
          delay(5000);
        }
}

void GetDate(void) {
  struct tm timeinfo;
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  char timeYear[5];
  strftime(timeYear,5, "%Y", &timeinfo);
  char timeMonth[15];
  strftime(timeMonth,15, "%B", &timeinfo);
  char timeDay[10];
  strftime(timeDay,10, "%d", &timeinfo);
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  char timeMin[3];
  strftime(timeMin,3, "%M", &timeinfo);
  fecha = String(timeYear)+String(timeMonth)+String(timeDay)+String(timeHour)+String(timeMin);
  Serial.println(fecha);
}

void ReadUltrasonico(void){

  // Clears the trigPin (Limpia el trigger)
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micros (Establece el trigger en ALTO durante 10 ms)
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave trigg (Lee el echo, regresa la onda)
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;

  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
}

void FireBase(void) {
    if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
    {
        sendDataPrevMillis = millis();

        FirebaseJson json;
        json.setDoubleDigits(3);
        json.add("value", count);

        Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, "/test/json", &json) ? "ok" : fbdo.errorReason().c_str());

        Serial.printf("Get json... %s\n", Firebase.RTDB.getJSON(&fbdo, "/test/json") ? fbdo.to<FirebaseJson>().raw() : fbdo.errorReason().c_str());

        FirebaseJson jVal;
        Serial.printf("Get json ref... %s\n", Firebase.RTDB.getJSON(&fbdo, "/test/json", &jVal) ? jVal.raw() : fbdo.errorReason().c_str());

        FirebaseJsonArray arr;
        arr.setFloatDigits(2);
        arr.setDoubleDigits(4);
        arr.add("DistanciaCm:", distanceCm,"DistanciaPulg:", distanceInch,"Fecha Y hora:", fecha); // Aquí se le agregan las variables a mandar a la base de datos

        Serial.printf("Set Array... %s\n", Firebase.RTDB.setArray(&fbdo, "/test/Array", &arr) ? "ok" : fbdo.errorReason().c_str());

        Serial.printf("Get Array... %s\n", Firebase.RTDB.getArray(&fbdo, "/test/Array") ? fbdo.to<FirebaseJsonArray>().raw() : fbdo.errorReason().c_str());

        Serial.printf("Push json... %s\n", Firebase.RTDB.pushJSON(&fbdo, "/test/push", &json) ? "ok" : fbdo.errorReason().c_str());

        json.set("value", count + 0.29745);
        Serial.printf("Update json... %s\n\n", Firebase.RTDB.updateNode(&fbdo, "/test/push/" + fbdo.pushName(), &json) ? "ok" : fbdo.errorReason().c_str());

        count++;
    }
}
