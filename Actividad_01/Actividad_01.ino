#include <Arduino.h> // Si utiliza el entorno de Arduino, esta linea debera ser comentada
/**
 * Tarea que realiza el parpadeo de un LED
 */

// Configuracion de nucleos a utilizar 
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Numero de GPIO a utilizar 
static const int led_pin = 4;

// Tarea 1
void toggleLED(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
// Tarea 2
  void toggleLED2(void *parameter) {
  while(1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(800 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(800 / portTICK_PERIOD_MS);
  }
}

void setup() {

  // Configuracion de GPIO como salida
  pinMode(led_pin, OUTPUT);

  // Task to run forever
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
              toggleLED,    // Function to be called
              "Toggle LED", // Name of task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              1,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL,         // Task handle
              app_cpu);     // Run on one core for demo purposes (ESP32 only)

  xTaskCreatePinnedToCore(  
              toggleLED2,  
              "Toggle 2",   
              1024,         
              NULL,         
              1,            
              NULL,         
              app_cpu);
}
void loop() {
}
/* -----------Comentarios---------------
   Se generan dos tareas, al cambiar la prioridad de éstas, se observa
   que la de mayor prioridad se logra ver más. En el caso de que tengan
   la misma prioridad, hay un comportamiento estable entre ellas.*/


// Después de la sección del loop principal, deberá agregregar un comentario donde indique lo que realiza el código.
//Genere un código que contemple lo siguiente:

// Dos tareas que permitan controlar el toggle de un GPIO. (Solo un GPIO por tarea)
// El toggle de una tarea deberá ser del doble de la frecuencia de la otra tarea.
/* Modifique la prioridad de las tareas, primero establezca la prioridad igual, después modifique la prioridad e invierta
  esta prioridad con la finalidad de observar el comportamiento a través de un osciloscopio. */
/* Después del loop principal deberá agregar una sección de comentarios donde realizará un análisis del comportamiento.
   Si desea agregar imagenes deberá integrarlas en el repositorio. */
