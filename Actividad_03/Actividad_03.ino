// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

String message = "";
static char incomingChar;
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

//*****************************************************************************
// Tareas

// Tarea 1 (lectura)
void startTask1(void *parameter) {

  while (1) {
    if (Serial.available()){
      char incomingChar = Serial.read();
      if (incomingChar != '\n'){
        message += String(incomingChar);
      }
      else{
        message = "";
      }
      Serial.write(incomingChar);
  }
}
}
// Tarea 2
void startTask2(void *parameter) {
  while (1) {
        Serial.println(incomingChar);
         if (message == "Reset"){
            incomingChar='\n';
         }
    }
  }

//*****************************************************************************
// Main 
 
void setup() {

  // Serial lento para poder visualizar 
  Serial.begin(300);

  // Retardo antes de iniciar 
  vTaskDelay(1000 / portTICK_PERIOD_MS);

 // Task 1
  xTaskCreatePinnedToCore(startTask1,
                          "Tarea 1",
                          1024,
                          NULL,
                          1,
                          &task_1,
                          app_cpu);

  // Tarea 2
  xTaskCreatePinnedToCore(startTask2,
                          "Tarea 2",
                          1024,
                          NULL,
                          1,
                          &task_2,
                          app_cpu);
}

void loop() {
  
}

/* Aquí la tarea 2 se ejecuta de acuerdo a la tarea 1. La tarea 1
   se encarga de leer el puerto serial y realiza la detección de salto
   de línea. Ésta le notifica a la tarea 2 sobre un nuevo mensaje e imprime
   dichoo mensaje.*/
