typedef struct {
	button_event_t button_event;
	led_event_t led_event;
} event_data_t;

typedef struct {
	event_data_t data_t;
	void (*callback_process_event)(ao_event_t*);
	QueueHandle_t event_queue_h;
} ao_t;