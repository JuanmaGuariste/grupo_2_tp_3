/*
* ao.h
* author: José Luis Krüger
*/

#ifndef ACTIVE_OBJECT_H
#define ACTIVE_OBJECT_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "queue_p.h"

typedef void* event_data_t;
typedef void (*event_callback_t)(event_data_t event);

typedef enum {
    FREE_RTOS_QUEUE,
    PRIORITIZED_QUEUE
} queue_type_t;
typedef struct {
    void* event_queue;
    event_callback_t process_event;
    uint8_t event_size;
    uint8_t obj_id;
    void *evt;
} active_object_t;

void active_object_init(active_object_t *obj,
                        event_callback_t process_event,
                        size_t queue_size,
                        int task_priority,
                        const char* task_name, queue_type_t opt_queue);

void active_object_send_event(active_object_t *obj, event_data_t event);

bool_t active_object_send_priority_event(active_object_t *obj, int data, int priority);

void active_object_task(void *pv_parameters);

void active_object_task_queue_priorized(void *pv_parameters);

#endif // ACTIVE_OBJECT_H
