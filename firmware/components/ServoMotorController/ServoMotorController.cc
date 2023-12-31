/*
 * ServoMotorController.cc
 *
 *  Created on: Dec 14, 2023
 *      Author: Mehmet Bahadır Maktav
 */

#include "ServoMotorController.h"

#include <math.h>

#include "esp_log.h"

static const char *TAG = "SERVO_MOTOR_CONTROLLER";

ServoMotorController::ServoMotorController(int pwm_gpio_num, ledc_channel_t ledc_channel, float min_width_ms, float max_width_ms, 
                                           uint32_t timer_frequency, ledc_timer_bit_t duty_resolution,
                                           float min_angle, float max_angle)
  : pwm_gpio_num_(pwm_gpio_num)
  , ledc_channel_(ledc_channel)
{
  uint32_t max_width_duty = (pow(2, (int)duty_resolution) - 1) * max_width_ms / (1000 / timer_frequency);
  min_width_duty_ = (pow(2, (int)duty_resolution) - 1) * min_width_ms / (1000 / timer_frequency);
  duty_per_angle_ = (max_width_duty - min_width_duty_) / (max_angle - min_angle);
  ESP_LOGI(TAG, "Calculated max width duty : %lu | min width duty : %lu | duty per angle : %f", max_width_duty, min_width_duty_, duty_per_angle_);
  Configure(pwm_gpio_num, timer_frequency, duty_resolution);
}

ServoMotorController::~ServoMotorController() {
  ledc_stop(LEDC_LOW_SPEED_MODE, ledc_channel_, 0);
}

void ServoMotorController::RotateToAngle(float angle) {
  uint32_t duty = ((angle * duty_per_angle_) + min_width_duty_);
  ESP_LOGI(TAG, "Rotating to %f angle / %lu duty", angle, duty);
  ledc_set_duty(LEDC_LOW_SPEED_MODE, ledc_channel_, duty);
  ledc_update_duty(LEDC_LOW_SPEED_MODE, ledc_channel_);
}

void ServoMotorController::Configure(int pwm_gpio_num, uint32_t timer_frequency, ledc_timer_bit_t duty_resolution) {
  // Set ledc timer configurations
  ledc_timer_config_t ledc_timer;
  ledc_timer.speed_mode       = LEDC_LOW_SPEED_MODE;
  ledc_timer.timer_num        = LEDC_TIMER_0;
  ledc_timer.duty_resolution  = duty_resolution;
  ledc_timer.freq_hz          = timer_frequency;  
  ledc_timer.clk_cfg          = LEDC_AUTO_CLK;
  ledc_timer_config(&ledc_timer);
  // Set ledc channel configurations
  ledc_channel_config_t ledc_channel;
  ledc_channel.speed_mode     = LEDC_LOW_SPEED_MODE;
  ledc_channel.channel        = ledc_channel_;
  ledc_channel.timer_sel      = LEDC_TIMER_0;
  ledc_channel.intr_type      = LEDC_INTR_DISABLE;
  ledc_channel.gpio_num       = pwm_gpio_num;
  ledc_channel.duty           = 0;
  ledc_channel.hpoint         = 0;
  ledc_channel_config(&ledc_channel);  
}