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
#include "task_led.h"
#include "ao.h"
#include "task_button.h"

/********************** macros and definitions *******************************/

#define TASK_PERIOD_MS_           (5000)

/********************** internal data declaration ****************************/
static led_color_t evt;
/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data definition *****************************/

/********************** internal functions definition ************************/
void init_led_active_object(active_object_t *led_obj,
                            void (*callback)(event_data_t),
                            uint8_t priority) {
    led_obj->evt = &evt;
    led_obj->event_size = sizeof(int);
    active_object_init(led_obj, callback, 0, priority ,"Task LED", PRIORITIZED_QUEUE);
}


void led_set_colors(bool r, bool g, bool b)
{
  HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, r ? GPIO_PIN_SET: GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, g ? GPIO_PIN_SET: GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, b ? GPIO_PIN_SET: GPIO_PIN_RESET);
}

/********************** external functions definition ************************/

void task_led(event_data_t event)
{
  led_color_t color = *(led_color_t*)event;
  switch (color)
  {
    case LED_COLOR_NONE:
      led_set_colors(false, false, false);
      break;
    case LED_COLOR_RED:
      LOGGER_INFO("led red");
      led_set_colors(true, false, false);
      break;
    case LED_COLOR_GREEN:
      LOGGER_INFO("led green");
      led_set_colors(false, true, false);
      break;
    case LED_COLOR_BLUE:
      LOGGER_INFO("led blue");
      led_set_colors(false, false, true);
      break;
    case LED_COLOR_WHITE:
      LOGGER_INFO("led white");
      led_set_colors(true, true, true);
      break;
    default:
      break;
  }
  vTaskDelay((TickType_t)(TASK_PERIOD_MS_ / portTICK_PERIOD_MS));
  led_set_colors(false, false, false);
}

/********************** end of file ******************************************/
