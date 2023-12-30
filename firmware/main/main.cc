#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_heap_caps.h"

#include "WiFi.h"
#include "WebSocketServer.h"
#include "ServoMotorController.h"
#include "CameraController.h"
#include "CommandHandler.h"

static const char *TAG = "APPLICATION";

#ifdef __cplusplus
extern "C" {
#endif

CameraController camera_controller;
ServoMotorController pan_motor_controller(GPIO_NUM_13, LEDC_CHANNEL_0, 0.5, 2.5, 50, LEDC_TIMER_10_BIT, 0, 180);
ServoMotorController tilt_motor_controller(GPIO_NUM_15, LEDC_CHANNEL_1, 0.5, 2.5, 50, LEDC_TIMER_10_BIT, 0, 180);

void app_main(void) {
  // Initialize components
  CommandHandler::Instance().SetCameraController(&camera_controller);
  CommandHandler::Instance().SetServoMotorControllers(&pan_motor_controller, &tilt_motor_controller);
  ESP_LOGI(TAG, "Components initialized.");

  // TODO(MBM) Move NVS into a component.
  // Initialize NVS (Non Volatile Storage)
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
  ESP_LOGI(TAG, "NVS initialized.");

  // Connect to WiFi
  WiFi::Instance().Connect();
  ESP_LOGI(TAG, "WiFi connected.");

  // Start WebSocket Server
  WebSocketServer::Instance().StartServer();
  ESP_LOGI(TAG, "WebSocket server started.");
}
    
#ifdef __cplusplus
}
#endif