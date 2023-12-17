/*
 * ServoMotorController.h
 *
 *  Created on: Dec 14, 2023
 *      Author: Mehmet Bahadır Maktav
 */

#ifndef SERVO_MOTOR_CONTROLLER_H_
#define SERVO_MOTOR_CONTROLLER_H_

#include <driver/gpio.h>
#include <driver/ledc.h>

class ServoMotorController {
public:
  ServoMotorController(int pwm_gpio_num, float min_width_ms, float max_width_ms, 
                       uint32_t timer_frequency, ledc_timer_bit_t duty_resolution,
                       float min_angle, float max_angle);
  ~ServoMotorController();
  void RotateToAngle(float angle);

private:
  void Configure(int pwm_gpio_num, uint32_t timer_frequency, ledc_timer_bit_t duty_resolution);

private:
  int pwm_gpio_num_;
  uint32_t min_width_duty_;
  float duty_per_angle_;
};

#endif // SERVO_MOTOR_CONTROLLER_H_