#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"
#include "driver/gpio.h"

static const char *TAG = "APP";

void setupLed() {
  gpio_config_t config = {
      .pin_bit_mask = (1 << GPIO_NUM_25),
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_ENABLE,
      .intr_type = GPIO_INTR_DISABLE};

  gpio_config(&config);
}

void toggleLed() {
  ESP_LOGI(TAG, "Toggle LED");
  gpio_set_level(GPIO_NUM_25, 1);
  vTaskDelay(500 / portTICK_PERIOD_MS);
  gpio_set_level(GPIO_NUM_25, 0);
  vTaskDelay(500 / portTICK_PERIOD_MS);
}

void app_main() {
  ESP_LOGI(TAG, "Setting up LED\n");
  setupLed();

  while (1) {
    toggleLed();
  }
}