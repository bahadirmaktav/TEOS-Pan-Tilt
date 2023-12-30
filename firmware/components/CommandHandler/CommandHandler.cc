/*
 * CommandHandler.cc
 *
 *  Created on: Dec 6, 2023
 *      Author: Mehmet Bahadır Maktav
 */

#include "CommandHandler.h"
#include "EnumCommandResponse.h"

static const char *TAG = "COMMAND_HANDLER";

CommandHandler::CommandHandler() 
  : camera_streaming_task_handler_(NULL)
  , camera_controller_(nullptr)
  , pan_motor_controller_(nullptr)
  , tilt_motor_controller_(nullptr)
{}

void CommandHandler::SetCameraController(CameraController *camera_controller) {
  camera_controller_ = camera_controller;
}

void CommandHandler::SetServoMotorControllers(ServoMotorController *pan_motor_controller, ServoMotorController *tilt_motor_controller) {
  pan_motor_controller_ = pan_motor_controller;
  tilt_motor_controller_ = tilt_motor_controller;
}

void CommandHandler::ExecuteCommand(uint8_t *command, size_t length) {
  if (length < COMMAND_BUFFER_MIN_LENGTH || length > COMMAND_BUFFER_MAX_LENGTH) {
    ESP_LOGW(TAG, "Command message length must be between 6 and 32!");
    return;
  }
  if (command[0] != 0xBB || command[1] != 0xFF || command[length - 1] != 0xEE || command[length - 2] != 0xFF) {
    ESP_LOGW(TAG, "Sync bytes are wrong!");
    return;
  }
  CmdRsp command_buff = (CmdRsp)((uint16_t)command[2] * 256 + command[3]);
  switch (command_buff) {
  case CmdRsp::CMD_WEB_CONF_SET_CONFIGURATIONS:
    ESP_LOGI(TAG, "Command handler of set websocket configurations not implemented yet!");
    break;
  case CmdRsp::CMD_WEB_CONF_GET_CONFIGURATIONS:
    ESP_LOGI(TAG, "Command handler of get websocket configurations not implemented yet!");
    break;
  case CmdRsp::CMD_WEB_CONT_RESTART_SERVER:
    ESP_LOGI(TAG, "Command handler of restart websocket server not implemented yet!");
    break;
  case CmdRsp::CMD_WEB_STAT_GET_STATUS:
    GetWebSocketServerStatus();
    break;
  case CmdRsp::CMD_MOT_CONF_SET_CONFIGURATIONS:
    ESP_LOGI(TAG, "Command handler of set motor configurations not implemented yet!");
    break;
  case CmdRsp::CMD_MOT_CONF_GET_CONFIGURATIONS:
    ESP_LOGI(TAG, "Command handler of get motor configurations not implemented yet!");
    break;
  case CmdRsp::CMD_MOT_CONT_RESET_POSITION:
    ResetMotorPositionHandler(command, length);
    break;
  case CmdRsp::CMD_MOT_CONT_ROTATE:
    RotateMotorHandler(command, length);
    break;
  case CmdRsp::CMD_CAM_CONF_SET_CONFIGURATIONS:
    ESP_LOGI(TAG, "Command handler of set camera configurations not implemented yet!");
    break;
  case CmdRsp::CMD_CAM_CONF_GET_CONFIGURATIONS:
    ESP_LOGI(TAG, "Command handler of get camera configurations not implemented yet!");
    break;
  case CmdRsp::CMD_CAM_CONT_START_STREAM:
    StartCameraStreamHandler();
    break;
  case CmdRsp::CMD_CAM_CONT_STOP_STREAM:
    StopCameraStreamHandler();
    break;
  default:
    break;
  }
}

void CommandHandler::GetWebSocketServerStatus() {
  uint8_t message_buffer[7] = {0xBB, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xEE};
  uint8_t is_server_active = WebSocketServer::Instance().IsServerActive();
  uint16_t command_buffer = CmdRsp::RSP_WEB_STAT_STATUS;
  message_buffer[2] = (command_buffer >> 8) & 0xFF;
  message_buffer[3] = command_buffer & 0xFF;
  message_buffer[4] = is_server_active;
  WebSocketServer::Instance().SyncSendFrame(message_buffer, sizeof(message_buffer));
  ESP_LOGI(TAG, "Server status successfully sent.");
}

void CommandHandler::ResetMotorPositionHandler(uint8_t *command, size_t length) {
  int motor_type = command[4];
  if (motor_type == 0) {
    // Pan
    pan_motor_controller_->RotateToAngle(0);
    ESP_LOGI(TAG, "Pan motor position reset.");
  } else if (motor_type == 1) {
    // Tilt
    tilt_motor_controller_->RotateToAngle(0);
    ESP_LOGI(TAG, "Tilt motor position reset.");
  } else {
    ESP_LOGW(TAG, "Motor type to reset position cannot found!");
  }
}
  
void CommandHandler::RotateMotorHandler(uint8_t *command, size_t length) {
  int motor_type = command[4];
  if (motor_type == 0) {
    // Rotate pan
    float angle = ((int)command[5] * 256 + (int)command[6]) / 100.0;
    ESP_LOGI(TAG, "Pan motor rotation to %f degree started.", angle);
    pan_motor_controller_->RotateToAngle(angle);
  } else if (motor_type == 1) {
    // Rotate tilt
    float angle = ((int)command[5] * 256 + (int)command[6]) / 100.0;
    ESP_LOGI(TAG, "Tilt motor rotation to %f degree started.", angle);
    tilt_motor_controller_->RotateToAngle(angle);
  }
}

void CommandHandler::StartCameraStreamHandler() {
  camera_controller_->StartCamera();
}

void CommandHandler::StopCameraStreamHandler() {
  camera_controller_->StopCamera();  
}