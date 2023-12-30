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
  CmdRsp command_buff = (CmdRsp)((uint16_t)command[2] * 16 + command[3]);
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

void CommandHandler::ExecuteCommand(uint8_t *command, size_t length) {
  if (length != COMMAND_BUFFER_LENGTH) {
    ESP_LOGI(TAG, "Command message length must be 8!");
    return;
  }
  if (command[0] != 0xFF) {
    ESP_LOGI(TAG, "Sync byte is wrong!");
    return;
  }
  // TODO(MBM): Checksum control logic will be added.
  uint8_t settings_option = command[1];
  if (settings_option == 1) {          // Camera Settings
    CameraCommandHandler(command);
  } else if (settings_option == 2) {   // Motor Settings
    MotorCommandHandler(command);
  } else if (settings_option == 3) {   // System Mode Settings
    // TODO(MBM): Implement later.
  } else {
    ESP_LOGW(TAG, "Settings option cannot be found! Given option: %d", settings_option);
  }
}

void CommandHandler::CameraCommandHandler(uint8_t *command) {
  uint8_t camera_control_mode = command[2];
  if (camera_control_mode == 1) {
    // Start camera
    camera_controller_->StartCamera();
  } else if(camera_control_mode == 2) {
    // Stop camera
    camera_controller_->StopCamera();
  } else {
    ESP_LOGW(TAG, "Given camera control mode cannot be found! Given mode: %d", camera_control_mode);
  }
}

void CommandHandler::MotorCommandHandler(uint8_t *command) {
  int motor_type = command[2];
  if (motor_type == 1) {
    // Rotate pan
    float angle = ((int)command[3] * 256 + (int)command[4]) / 100.0;
    ESP_LOGI(TAG, "Pan motor rotation to %f degree started.", angle);
    pan_motor_controller_->RotateToAngle(angle);
  } else if (motor_type == 2) {
    // Rotate tilt
    float angle = ((int)command[3] * 256 + (int)command[4]) / 100.0;
    ESP_LOGI(TAG, "Tilt motor rotation to %f degree started.", angle);
    tilt_motor_controller_->RotateToAngle(angle);
  }
}