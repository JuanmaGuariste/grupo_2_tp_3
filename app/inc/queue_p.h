/**
 * queue_p.h
 * 
 */

#ifndef QUEUE_P_H
#define QUEUE_P_H

#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "cmsis_os.h"

typedef bool bool_t;
typedef struct node {
    int data;

    // Lower values indicate
    // higher priority
    int priority;
    bool_t used;
    struct node* next;

} node_t;

typedef struct queue_p
{
    node_t *head;
    node_t *tail;
    uint32_t current_length;
    bool_t initialized;
    SemaphoreHandle_t queue_mutex;
} queue_p_t;

int queue_peek(queue_p_t* queue);

void queue_create(queue_p_t **queue);

void queue_destroy(queue_p_t **queue);

bool_t queue_pop(queue_p_t* queue, int* data);

bool_t queue_push(queue_p_t* queue, int d, int p);

int queue_is_empty(queue_p_t* queue);

#endif // QUEUE_P_H
