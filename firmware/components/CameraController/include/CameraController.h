/*
 * CameraController.h
 *
 *  Created on: Dec 8, 2023
 *      Author: Mehmet Bahadır Maktav
 */

#ifndef CAMERA_CONTROLLER_H_
#define CAMERA_CONTROLLER_H_

class CameraController;

#include "esp_camera.h"

#include "WebSocketServer.h"

class CameraController {
public:
  CameraController();
  void StartCamera();
  void StopCamera();

private:
  static void StreamImage(void *pvParameter);

private:
  TaskHandle_t stream_image_task_handle_;
  camera_config_t camera_config_;
};

#endif // CAMERA_CONTROLLER_H_