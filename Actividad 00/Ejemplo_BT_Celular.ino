/*Código funcional para lector Bluetooth 
  Es necesario revisar gpio a Raspberry*/

  /* El código permite tener una comunicación por BT entre el ESP32 y el celular,
     para ello se utilizó la aplicación Serial Bluetooth Terminal. Desde esta app
     se puede controlar el encendido y apagado de un led, así como activar y 
     desactivar el Bluetooth por medio de un botón.*/
#include <BluetoothSerial.h>
#define LED1_GPIO 5
#define Button 14

BluetoothSerial SerialBT;
void ConfigByBluetooth(void);

void setup() {
  pinMode(LED1_GPIO, OUTPUT);
  pinMode(Button,INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("Inicio de código");
// TP-Link312A
// 
}

void loop() {
  ConfigByBluetooth();
/*  if(digitalRead(Button) == 0){
    delay(500); // Antirrebotes
    if(digitalRead(Button) == 0){
      while(digitalRead(Button) == 0);
      Serial.println("Botón presionado"); // Aquí se pone el BT begin
    }

  }
  else{
    Serial.println("Botón no presionado");
  }
  delay(1000);
*/
}

void ConfigByBluetooth(void){
  // Inicializamos bluetooth
  String message = "";
  String message2 = "";
  char incomingChar;
  char incomingChar2;
  bool BTHControl = true;
  if(digitalRead(Button) == 0){
    delay(500); // Antirrebotes
    if(digitalRead(Button) == 0){
      while(digitalRead(Button) == 0);
      SerialBT.begin("DSP0211"); // Aquí se pone el BT begin
    }

  }
  else{
    Serial.println("Botón no presionado");
  }
  delay(1000);
  while(BTHControl==true){
    Serial.println("Bluetooth disponible");
   /* if (Button == 1 AND true){
      Blueconf.
      else com_true
    }*/
    if(SerialBT.available()){
      char incomingChar = SerialBT.read(); // lee y almacena
      if (incomingChar != '\n'){
        message += String(incomingChar); // si hay salto de línea se almacena
      }
      else {
        //BTHControl = false;
        message = "";
      }
      Serial.write(incomingChar);
    }
    if (message =="Reset"){
      BTHControl = false;
    }
    
    if(SerialBT.available()){
      char incomingChar2 = SerialBT.read(); // lee y almacena
      if (incomingChar2 != '\n'){
        message2 += String(incomingChar); // si hay salto de línea se almacena
      }
      else {
        //BTHControl = false;
        message2 = "";
      }
      Serial.write(incomingChar2);
    }
  }
  SerialBT.end();
}
// Por bt modificar nombre de red y contraseña
