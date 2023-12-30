/*
 * WebSocketServer.h
 *
 *  Created on: Dec 8, 2023
 *      Author: Mehmet Bahadır Maktav
 */

#ifndef WEB_SOCKET_SERVER_H_
#define WEB_SOCKET_SERVER_H_

class WebSocketServer;
struct RespArg;

#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include <esp_http_server.h>

#include "CommandHandler.h"

struct RespArg {
  // Variable sync operation
  httpd_req_t *req;
  // Variables for async operation
  httpd_handle_t hd;
  int fd;
  RespArg() 
    : req(nullptr)
    , hd(NULL)
    , fd(-1)
  {}  
  RespArg(httpd_req_t *vreq, httpd_handle_t vhd, int vfd) 
    : req(vreq)
    , hd(vhd)
    , fd(vfd)
  {}
};

class WebSocketServer {
public:
  // Singleton setup  
  static WebSocketServer &Instance() {
    static WebSocketServer websocket_server;
    return websocket_server;
  }
  WebSocketServer(const WebSocketServer &) = delete;
  WebSocketServer & operator = (const WebSocketServer &) = delete;

  void StartServer(void);
  void StopServer(httpd_handle_t server);
  uint8_t IsServerActive();
  void SyncSendFrame(uint8_t *data, size_t length);
  void AsyncSendFrame(uint8_t *data, size_t length);

private:
  // C'tor and D'tor defined private for singleton
  WebSocketServer();
  ~WebSocketServer() {}

  // Websocket handler function
  static esp_err_t RequestHandler(httpd_req_t *req);

public:
  httpd_uri_t ws_;            // Server configurations
  httpd_handle_t server_;     // Server handler
  RespArg resp_arg_;
};

#endif // WEB_SOCKET_SERVER_H_