#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Configuraciones 
static const int num_tasks = 8;     // Numero de tareas, modificarlo varias veces para revisar el funcionamiento

// Ejemplo de estructura para enviar cadena 
typedef struct Message {
  char body[20];
  uint8_t len;
} Message;

// Variables globales 
static SemaphoreHandle_t sem_params; // Conteo

//*****************************************************************************
// Tarea

void myTask(void *parameters) {

  Message msg = *(Message *)parameters;

  // Incremento de semaphore 
  xSemaphoreGive(sem_params);

  // Mensaje salida 
  Serial.print("Recibido: ");
  Serial.print(msg.body);
  Serial.print(" | longitud: ");
  Serial.println(msg.len);

  // retardo
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL);
}

//*****************************************************************************
// Main 

void setup() {
  
  char task_name[12];
  Message msg;
  char text[20] = "abcdefgh";
  
  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS ---");

  // Create semaphores (initialize at 0)
  sem_params = xSemaphoreCreateCounting(num_tasks, 0);

  // Create message to use as argument common to all tasks
  strcpy(msg.body, text);
  msg.len = strlen(text);

  // Start tasks
  for (int i = 0; i < num_tasks; i++) {

    // Generate unique name string for task
    sprintf(task_name, "Tarea %i", i);

    // Start task and pass argument (common Message struct)
    xTaskCreatePinnedToCore(myTask,
                            task_name,
                            1024,
                            (void *)&msg,
                            1,
                            NULL,
                            app_cpu);
  }
  for (int i = 0; i < num_tasks; i++) {
    xSemaphoreTake(sem_params, portMAX_DELAY);
  }
  Serial.println("Todas las tareas creadas");
}

void loop() {

  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

// Da la longitud de la cadena introducida, pero está buggeado
//   El semaphore se encarga de organizar las tareas
// Si se modifica el número de tareas, cambia las veces que se imprimen en el monitor serie
