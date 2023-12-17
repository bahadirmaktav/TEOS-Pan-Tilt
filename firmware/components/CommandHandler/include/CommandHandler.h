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

#define COMMAND_BUFFER_LENGTH 8

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
  void MotorCommandHandler(uint8_t *command);
  void CameraCommandHandler(uint8_t *command);

private:
  TaskHandle_t camera_streaming_task_handler_;
  CameraController *camera_controller_;
  ServoMotorController *pan_motor_controller_;
  ServoMotorController *tilt_motor_controller_;
};

#endif // COMMAND_HANDLER_H_