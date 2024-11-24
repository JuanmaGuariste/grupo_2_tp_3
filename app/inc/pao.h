#define MAX_SIZE 10

typedef enum
{
  LOW,
  MEDIUM,
  HIGH
} priority_t;

typedef struct {
	priority_t priority;
	event_data_t data_t;
} queue_data_t;

typedef struct {
	int size;
	queue_data_t elements[MAX_SIZE];
	SemaphoreHandle_t mtx;
} PrioQueueHandle_t;

typedef struct {
	void (*callback_process_event)(pao_event_t*);
	PrioQueueHandle_t event_queue_h;
} pao_t;
