/*
* ao.c
* author: José Luis Krüger
*/

#include "ao.h"
#include "task_button.h"
#include "logger.h"

// constantes definidas para facilitar el debugging del programa
static char *QUEUE_ID_1 = "Queue_id_1";
static char *QUEUE_ID_2 = "Queue_id_2";
static char *QUEUE_ID_3 = "Queue_id_3";
static char *QUEUE_ID_4 = "Queue_id_4";
static char *QUEUE_ID_DEF = "Queue_id_default";

 char *get_queue_name( uint8_t id) {
     char *buffer[4] = { 0 };
     if (id > 4) return QUEUE_ID_DEF;

     buffer[0] = QUEUE_ID_1;
     buffer[1] = QUEUE_ID_2;
     buffer[2] = QUEUE_ID_3;
     buffer[3] = QUEUE_ID_4;

    return buffer[id - 1];
}

void active_object_init(active_object_t *obj,
                        event_callback_t process_event,
                        size_t queue_size,
                        int task_priority,
                        const char* task_name, queue_type_t opt_queue) {

    if (opt_queue == PRIORITIZED_QUEUE) {
        queue_create((queue_p_t**)&obj->event_queue);

    } else if ( opt_queue == FREE_RTOS_QUEUE) {
        obj->event_queue = xQueueCreate(queue_size, obj->event_size);
        vQueueAddToRegistry(obj->event_queue, get_queue_name(obj->obj_id));
    }              
    configASSERT(NULL != obj->event_queue);
    obj->process_event = process_event;

    BaseType_t status;
    status = xTaskCreate(opt_queue == PRIORITIZED_QUEUE ? active_object_task_queue_priorized : active_object_task,
                            task_name,
                            configMINIMAL_STACK_SIZE,
                            obj,
                            task_priority,
                            NULL);

    configASSERT(pdPASS == status);
//    LOGGER_INFO("Se inicializa el objeto activo id: %d, Tamaño del evento: %d",obj->obj_id, obj->event_size);
}

void active_object_send_event(active_object_t *obj, event_data_t event) {
    configASSERT(obj != NULL || event != NULL);
    xQueueSend(obj->event_queue, event, 0);
}

bool_t active_object_send_priority_event(active_object_t *obj, int data, int priority) {
    configASSERT(obj != NULL);
    return queue_push(obj->event_queue, data, priority);
}

void active_object_task(void *pv_parameters) {
    active_object_t *obj = (active_object_t *) pv_parameters;
    event_data_t event = obj->evt;

    for (;;) {
        xQueueReceive(obj->event_queue, event, portMAX_DELAY);

        if (obj->process_event != NULL) obj->process_event(event);
    }
}

void active_object_task_queue_priorized(void *pv_parameters) {
    active_object_t *obj = (active_object_t *) pv_parameters;
    event_data_t event = obj->evt;
    for (;;) {
        if (queue_pop(obj->event_queue, event) && obj->process_event != NULL)
        	obj->process_event(event);
    }
}
