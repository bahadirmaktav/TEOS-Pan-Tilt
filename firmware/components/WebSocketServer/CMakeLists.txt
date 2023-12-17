idf_component_register(SRCS "WebSocketServer.cc"
                       INCLUDE_DIRS "include"
                       REQUIRES esp_http_server CommandHandler)