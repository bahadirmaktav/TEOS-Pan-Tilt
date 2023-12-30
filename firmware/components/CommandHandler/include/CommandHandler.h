/*
 * CommandHandler.h
 *
 *  Created on: Dec 6, 2023
 *      Author: Mehmet Bahadır Maktav
 */

#ifndef COMMAND_HANDLER_H_
#define COMMAND_HANDLER_H_

class CommandHandler;

#include <stdint.h>

#include <esp_log.h>

#include "WebSocketServer.h"
#include "CameraController.h"
#include "ServoMotorController.h"

#define COMMAND_BUFFER_MIN_LENGTH 6
#define COMMAND_BUFFER_MAX_LENGTH 32

class CommandHandler {
public:
  // Singleton setup  
  static CommandHandler &Instance() {
    static CommandHandler command_handler;
    return command_handler;
  }
  CommandHandler(const CommandHandler &) = delete;
  CommandHandler & operator = (const CommandHandler &) = delete;
  
  // Setters
  void SetCameraController(CameraController *);
  void SetServoMotorControllers(ServoMotorController *pan_motor_controller, ServoMotorController *tilt_motor_controller);

  // Command executer
  void ExecuteCommand(uint8_t *command, size_t length);

private:
  // C'tor and D'tor defined private for singleton
  CommandHandler();
  ~CommandHandler() {}
  
  // Command handlers
  void GetWebSocketServerStatus();
  void ResetMotorPositionHandler(uint8_t *command, size_t length);
  void RotateMotorHandler(uint8_t *command, size_t length);
  void StartCameraStreamHandler();
  void StopCameraStreamHandler();

private:
  TaskHandle_t camera_streaming_task_handler_;
  CameraController *camera_controller_;
  ServoMotorController *pan_motor_controller_;
  ServoMotorController *tilt_motor_controller_;
};

#endif // COMMAND_HANDLER_H_