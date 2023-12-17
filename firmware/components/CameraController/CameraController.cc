/*
 * CameraController.cc
 *
 *  Created on: Dec 8, 2023
 *      Author: Mehmet Bahadır Maktav
 */

#include "CameraController.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_log.h>

// Pin configurations - ESP32Cam (AiThinker)
#define CAM_PIN_PWDN 32
#define CAM_PIN_RESET -1 //software reset will be performed
#define CAM_PIN_XCLK 0
#define CAM_PIN_SIOD 26
#define CAM_PIN_SIOC 27

#define CAM_PIN_D7 35
#define CAM_PIN_D6 34
#define CAM_PIN_D5 39
#define CAM_PIN_D4 36
#define CAM_PIN_D3 21
#define CAM_PIN_D2 19
#define CAM_PIN_D1 18
#define CAM_PIN_D0 5
#define CAM_PIN_VSYNC 25
#define CAM_PIN_HREF 23
#define CAM_PIN_PCLK 22

static const char *TAG = "CAMERA";

CameraController::CameraController() {
  camera_config_.pin_pwdn = CAM_PIN_PWDN;
  camera_config_.pin_reset = CAM_PIN_RESET;
  camera_config_.pin_xclk = CAM_PIN_XCLK;
  camera_config_.pin_sccb_sda = CAM_PIN_SIOD;
  camera_config_.pin_sccb_scl = CAM_PIN_SIOC;

  camera_config_.pin_d7 = CAM_PIN_D7;
  camera_config_.pin_d6 = CAM_PIN_D6;
  camera_config_.pin_d5 = CAM_PIN_D5;
  camera_config_.pin_d4 = CAM_PIN_D4;
  camera_config_.pin_d3 = CAM_PIN_D3;
  camera_config_.pin_d2 = CAM_PIN_D2;
  camera_config_.pin_d1 = CAM_PIN_D1;
  camera_config_.pin_d0 = CAM_PIN_D0;
  camera_config_.pin_vsync = CAM_PIN_VSYNC;
  camera_config_.pin_href = CAM_PIN_HREF;
  camera_config_.pin_pclk = CAM_PIN_PCLK;

  //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
  camera_config_.xclk_freq_hz = 20000000;
  camera_config_.ledc_timer = LEDC_TIMER_0;
  camera_config_.ledc_channel = LEDC_CHANNEL_0;

  camera_config_.pixel_format = PIXFORMAT_JPEG; //YUV422,GRAYSCALE,RGB565,JPEG
  camera_config_.frame_size = FRAMESIZE_QVGA;    //QQVGA-UXGA, For ESP32, do not use sizes above QVGA when not JPEG. The performance of the ESP32-S series has improved a lot, but JPEG mode always gives better frame rates.

  camera_config_.jpeg_quality = 12; //0-63, for OV series camera sensors, lower number means higher quality
  camera_config_.fb_count = 1;       //When jpeg mode is used, if fb_count more than one, the driver will work in continuous mode.
  camera_config_.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  camera_config_.fb_location = CAMERA_FB_IN_PSRAM;
}

void CameraController::StartCamera() {
  // Initialize the camera.
  // Library starts the camera in its initialize call (calls cam_start).
  if (esp_camera_init(&camera_config_) != ESP_OK) {
    ESP_LOGE(TAG, "Camera starting failed.");
    return;
  }
  ESP_LOGI(TAG, "Camera successfully started.");
  // Start streaming camera image
  xTaskCreate(&CameraController::StreamImage, "stream_image", 2048, NULL, 5, &stream_image_task_handle_);
  return;
}

void CameraController::StopCamera() {
  // Deinitialize the camera.
  // Library stops the camera in its deinitialize call (calls cam_stop).
  if (esp_camera_deinit() != ESP_OK) {
    ESP_LOGE(TAG, "Camera stopping failed.");
    return;
  }
  ESP_LOGI(TAG, "Camera successfully stoped.");
  return;
}

void CameraController::StreamImage(void *pvParameter) {
  while (1) {
    camera_fb_t *frame_buffer_ = esp_camera_fb_get();
    ESP_LOGI(TAG, "Picture taken! Its size was: %zu bytes", frame_buffer_->len);
    WebSocketServer::Instance().AsyncSendFrame(frame_buffer_->buf, frame_buffer_->len);
    esp_camera_fb_return(frame_buffer_);
    vTaskDelay(pdMS_TO_TICKS(100));
  } 
}