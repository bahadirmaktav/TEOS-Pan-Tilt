idf_component_register(SRCS "main.cc"
                       INCLUDE_DIRS "."
                       REQUIRES nvs_flash WiFi WebSocketServer ServoMotorController CameraController)