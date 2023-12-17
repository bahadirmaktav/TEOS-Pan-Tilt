/*
 * WiFi.h
 *
 *  Created on: Dec 8, 2023
 *      Author: Mehmet Bahadır Maktav
 */

#ifndef WIFI_H_
#define WIFI_H_

#include "esp_event.h"

#define SSID_LENGTH 				        32
#define PASSWORD_LENGTH				      64
#define EXAMPLE_ESP_MAXIMUM_RETRY  100
#define WIFI_CONNECTED_BIT        BIT0
#define WIFI_FAIL_BIT             BIT1

class WiFi {
public:
  // Singleton setup  
  static WiFi &Instance() {
    static WiFi wifi;
    return wifi;
  }
  WiFi(const WiFi &) = delete;
  WiFi & operator = (const WiFi &) = delete;
  
  void Connect();
  void Disconnect();

private:
  // C'tor and D'tor defined private for singleton
  WiFi() {}
  ~WiFi() {}

  static void EventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
};

#endif // WIFI_H_