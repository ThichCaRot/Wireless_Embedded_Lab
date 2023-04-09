#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN 15

void app_main(void)
{
  esp_rom_gpio_pad_select_gpio(LED_PIN);
  gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
  int ON = 0;
  
  while(true)
  {
    ON = !ON;
    gpio_set_level(LED_PIN, ON);
    vTaskDelay(2000/ portTICK_PERIOD_MS);
  }
}
