/*
 * WebSocketServer.cc
 *
 *  Created on: Dec 8, 2023
 *      Author: Mehmet Bahadır Maktav
 */

#include "WebSocketServer.h"

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <functional>

static const char *TAG = "WEBSOCKET_SERVER";

WebSocketServer::WebSocketServer() 
  : server_(NULL)
  , resp_arg_()
{
  ws_.uri = "/ws";
  ws_.method = HTTP_GET;
  ws_.handler = WebSocketServer::RequestHandler;
  ws_.user_ctx = NULL;
  ws_.is_websocket = true;
}

void WebSocketServer::StartServer(void) {
  server_ = NULL;
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  // Start the httpd server
  ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
  if (httpd_start(&server_, &config) == ESP_OK) {
    // Registering the ws handler
    ESP_LOGI(TAG, "Registering URI handlers");
    httpd_register_uri_handler(server_, &ws_);
    return;
  }
  ESP_LOGI(TAG, "Error starting server!");
  return;
}

void WebSocketServer::StopServer(httpd_handle_t server) {
  if (httpd_stop(server_) == ESP_OK) {
    server_ = NULL;
    ESP_LOGI(TAG, "Server stopped successfully");
    return;
  }
  ESP_LOGI(TAG, "Error stoping server!");
  return;
}

uint8_t WebSocketServer::IsServerActive() {
  uint8_t active = 0;
  if (server_ != NULL) {
    active = 1;
  }
  return active;
}

/**
 * @brief Send frame over websocket to the connected client.
 * This function must be called inside the request handler.
 */
void WebSocketServer::SyncSendFrame(uint8_t *data, size_t length) {
  httpd_ws_frame_t ws_pkt;
  memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
  ws_pkt.payload = data;
  ws_pkt.len = length;
  ws_pkt.type = HTTPD_WS_TYPE_BINARY;
  esp_err_t ret = httpd_ws_send_frame(resp_arg_.req, &ws_pkt);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "httpd_ws_send_frame failed with %d", (int)ret);
    return;
  }
  ESP_LOGI(TAG, "Frame is successfully sent");
}

void WebSocketServer::AsyncSendFrame(uint8_t *data, size_t length) {
  httpd_ws_frame_t ws_pkt;
  memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
  ws_pkt.payload = data;
  ws_pkt.len = length;
  ws_pkt.type = HTTPD_WS_TYPE_BINARY;
  esp_err_t ret = httpd_ws_send_frame_async(resp_arg_.hd, resp_arg_.fd, &ws_pkt);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "httpd_ws_send_frame_async failed with %d", (int)ret);
    return;
  }
}

esp_err_t WebSocketServer::RequestHandler(httpd_req_t *req) {
  if (req->method == HTTP_GET) {
    ESP_LOGI(TAG, "Handshake done, the new connection was opened");
    return ESP_OK;
  }
  httpd_ws_frame_t ws_pkt;
  uint8_t *buf = NULL;
  memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
  ws_pkt.type = HTTPD_WS_TYPE_TEXT;
  /* Set max_len = 0 to get the frame len */
  esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "httpd_ws_recv_frame failed to get frame len with %d", ret);
    return ret;
  }
  ESP_LOGI(TAG, "Frame length is %d.", ws_pkt.len);
  if (ws_pkt.len) {
    /* ws_pkt.len + 1 is for NULL termination as we are expecting a string */
    buf = (uint8_t*)calloc(1, ws_pkt.len + 1);
    if (buf == NULL) {
      ESP_LOGE(TAG, "Failed to calloc memory for buf");
      return ESP_ERR_NO_MEM;
    }
    ws_pkt.payload = buf;
    /* Set max_len = ws_pkt.len to get the frame payload */
    ret = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
    if (ret != ESP_OK) {
      ESP_LOGE(TAG, "httpd_ws_recv_frame failed with %d", ret);
      free(buf);
      return ret;
    }
    // TODO(MBM): Print command message in hex format.
    // ESP_LOGI(TAG, "Got packet with message: %s", buf);
  }
  // TODO(MBM): Add setter for resp_arg_
  WebSocketServer::Instance().resp_arg_.req = req;
  WebSocketServer::Instance().resp_arg_.hd = req->handle;
  WebSocketServer::Instance().resp_arg_.fd = httpd_req_to_sockfd(req);
  CommandHandler::Instance().ExecuteCommand(buf, ws_pkt.len);
  free(buf);
  return ret;
}