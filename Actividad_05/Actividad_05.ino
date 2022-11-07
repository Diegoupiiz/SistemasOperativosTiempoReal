//80794141
// Mutexes (Mutual Exclusion)
/* Probar código dentro de la carpeta Actividad 05 y en la parte
   inferior del código agregar una sección de comentarios donde
   deberá incluir el análisis del código y el resultado obtenido
   en la consola. */
// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Globals
static int shared_var = 0;
static SemaphoreHandle_t mutex; // 

// Tasks

// Increment shared variable (the wrong way)
void incTask(void *parameters) {

  int local_var;

  // Loop forever
  while (1) {

    // Take mutex prior to critical section
    if (xSemaphoreTake(mutex, 0) == pdTRUE) {

      local_var = shared_var;
      local_var++;
      vTaskDelay(random(100, 500) / portTICK_PERIOD_MS);
      shared_var = local_var;
      
      Serial.println(shared_var);
  
      //xSemaphoreGive(mutex); //bloquea

    } else {
      // Do something else
    }
  }
}


void setup() {

  // Hack to kinda get randomness
  randomSeed(analogRead(0));

  Serial.begin(115200);

  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS mutexes-");

  mutex = xSemaphoreCreateMutex();

  // Start task 1
  xTaskCreatePinnedToCore(incTask,
                          "Increment Task 1",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  // Start task 2
  xTaskCreatePinnedToCore(incTask,
                          "Increment Task 2",
                          1024,
                          NULL,
                          2,
                          NULL,
                          app_cpu);           

  vTaskDelete(NULL);
}

void loop() {
  
}

/* -----------------------COMENTARIOS---------------------- */
/* En el monitor serie se deberían mostrar las tareas entrelazadas,
   sin embargo, en este caso solo estaba contando. Cuando se
   comentó "//xSemaphoreGive(mutex)", el conteo solo llegaba a 1
   y ya no se observaba nada más.*/
