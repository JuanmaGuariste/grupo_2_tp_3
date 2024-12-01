/*
 * Copyright (c) 2023 Sebastian Bedin <sebabedin@gmail.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @author : Sebastian Bedin <sebabedin@gmail.com>
 */

/********************** inclusions *******************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "board.h"
#include "logger.h"
#include "dwt.h"
#include "task_button.h"
#include "task_led.h"

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/
static button_event_t evt;
/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

/********************** internal functions definition ************************/

/********************** external functions definition ************************/

void init_ui_active_object(active_object_t *ui_obj,
                            void (*callback)(event_data_t),
                            uint8_t priority) {
    ui_obj->evt = &evt;
    ui_obj->event_size = (uint8_t)sizeof(button_event_t);
    active_object_init(ui_obj, callback, 5,priority,"Task_ui",FREE_RTOS_QUEUE);
}

void ui_process_event(event_data_t event) {
    button_event_t *button_event = (button_event_t *) event;
    led_color_t data;
    int priority;

    switch (button_event->type) {
      case BUTTON_TYPE_PULSE:
        LOGGER_INFO("Se presionó PULSE");
        priority = 3;
        data = LED_COLOR_RED;
        break;
      case BUTTON_TYPE_SHORT:
      LOGGER_INFO("Se presionó SHORT");
        priority = 2;
        data = LED_COLOR_GREEN;
        break;
      case BUTTON_TYPE_LONG:
        LOGGER_INFO("Se presionó LONG");
        priority = 1;
        data = LED_COLOR_BLUE;
        break;
      default:
        break;
    }
    if (button_event->type != BUTTON_TYPE_NONE)
    	active_object_send_priority_event(button_event->led_obj, data, priority);
}
/********************** end of file ******************************************/
