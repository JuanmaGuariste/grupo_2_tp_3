/**
 * queue.c
 * 
 * 
 */
#include "queue_p.h"

#define QUEUE_P_LENGTH 10

static node_t _list [QUEUE_P_LENGTH] = { 0 };
static queue_p_t _queue;

static node_t *queue_get_free_node() {
    static uint8_t index = 0;
    while (index < QUEUE_P_LENGTH)
    {
        if( _list[index].used == false){
            _list[index].used = true;
            return &_list[index];
        }
       index = (index + 1) % QUEUE_P_LENGTH;
    }
    return NULL;
}

static void free_node(node_t* node) {
    node->used = false;
    node->data = 0x00;
    node->priority = 0x00;
    node->next = NULL;
}

// Function to create a new node
static node_t* new_node(int d, int p)
{
    node_t* temp = (node_t*)queue_get_free_node(sizeof(node_t));
    temp->data = d;
    temp->priority = p;
    temp->next = NULL;

    return temp;
}

// Return the value at head
int queue_peek(queue_p_t* queue)
{
    node_t *node = (node_t*)queue->head;
    return node->data; 
}

void queue_create(queue_p_t **queue)
{
    if(queue && !_queue.initialized) {
        *queue = &_queue;
        (*queue)->head = NULL;
        (*queue)->tail = NULL;
        (*queue)->current_length = 0;
        (*queue)->queue_mutex = xSemaphoreCreateMutex();
        (*queue)->initialized = true;
        // Register to logging
        vQueueAddToRegistry((*queue)->queue_mutex, "Mutex Handle");
    }
}

void queue_destroy(queue_p_t **queue)
{
    if(queue && *queue) {
        // Protects shared resource (mutex)
        xSemaphoreTake((*queue)->queue_mutex,portMAX_DELAY);
        {
            node_t *next = (*queue)->head->next;
            node_t *current = (*queue)->head;

            while (!next)
            {
                free_node(current);
                current = next;
                next = current->next;
            }

            (*queue)->head = NULL;
            (*queue)->tail = NULL;
            (*queue)->current_length = 0;
            (*queue)->initialized = false;
        }
        xSemaphoreGive((*queue)->queue_mutex);
        // mutex destroy
        vSemaphoreDelete((*queue)->queue_mutex);
        *queue = NULL;
    }
}

// Removes the element with the
// highest priority form the list
bool_t queue_pop(queue_p_t* queue, int* data)
{
    if(!queue || !data) return false;
    bool_t ret = false;

    xSemaphoreTake(queue->queue_mutex,portMAX_DELAY);
    {
        if(!queue_is_empty(queue)) {
            *data = queue->head->data;
            node_t* temp = queue->head;
            (queue->head) = (queue->head)->next;
            queue->current_length--;
            free_node(temp);
            ret = true;
        }
    }
    xSemaphoreGive(queue->queue_mutex);

    return ret;
}

// Function to push according to priority
bool_t queue_push(queue_p_t* queue, int d, int p)
{
    bool_t ret = false;
    xSemaphoreTake(queue->queue_mutex,portMAX_DELAY);
    {
        if (queue->current_length < QUEUE_P_LENGTH)
        {
            node_t* start = (queue->head);

            // Create new node_t
            node_t* temp = new_node(d, p);

            if (temp == NULL) return false;

            queue->current_length++;
            if (queue_is_empty(queue)) {
                queue->head = temp;
            }
            // Special Case: The head of list has
            // lesser priority than new node
            else if ((queue->head)->priority < p) {

                // Insert New node_t before head
                temp->next = queue->head;
                (queue->head) = temp;
            }
            else {

                // Traverse the list and find a
                // position to insert new node
                while (start->next != NULL
                    && start->next->priority > p) {
                    start = start->next;
                }

                // Either at the ends of the list
                // or at required position
                temp->next = start->next;
                start->next = temp;
            }
            ret = true;
        }
        
    }
    xSemaphoreGive(queue->queue_mutex);

    return ret;
}

// Function to check is list is empty
int queue_is_empty(queue_p_t* queue) { return (queue->head) == NULL; }
