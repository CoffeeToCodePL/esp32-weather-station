#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <esp_log.h>

#include <BME280.h>

extern "C"
{
  [[noreturn]] void app_main();
}

static constexpr const char* TAG = "APP";

static BME280::Sensor<BME280::I2C::Serial, BME280::I2C::Config> bme280;

void setupLed()
{
  gpio_config_t config = {
    .pin_bit_mask = (1 << GPIO_NUM_25),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_ENABLE,
    .intr_type = GPIO_INTR_DISABLE,
  };

  gpio_config(&config);
}

bool setupEnvironmentalSensor()
{
  BME280::I2C::Config config = {
    I2C_NUM_0,
    GPIO_NUM_21,
    GPIO_NUM_22
  };

  if (!bme280.init(config)) {
    ESP_LOGE(TAG, "Could not initialize BME280 sensor");
    return false;
  }

  return true;
}

void toggleLed()
{
  gpio_set_level(GPIO_NUM_25, 1);
  vTaskDelay(pdMS_TO_TICKS(500));
  gpio_set_level(GPIO_NUM_25, 0);
  vTaskDelay(pdMS_TO_TICKS(500));
}

void pollEnvironmentalData()
{
  ESP_LOGI(TAG, "Polling data from BME280");
  const auto maybeData = bme280.poll();

  if (maybeData) {
    ESP_LOGI(TAG, "Temperature: %f", maybeData.value().temperature);
    ESP_LOGI(TAG, "Humidity: %f", maybeData.value().humidity);
    ESP_LOGI(TAG, "Pressure: %f", maybeData.value().pressure);
  }
}

[[noreturn]] void app_main()
{
  ESP_LOGI(TAG, "Setting up LED");
  setupLed();

  ESP_LOGI(TAG, "Setting up BME280 communication via I2C bus");
  const bool envSensorIsFunctional = setupEnvironmentalSensor();

  while (true) {
    toggleLed();
    if (envSensorIsFunctional) {
      pollEnvironmentalData();
    }
  }
}