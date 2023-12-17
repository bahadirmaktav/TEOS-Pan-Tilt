idf_component_register(SRCS "CommandHandler.cc"
                    INCLUDE_DIRS "include"
                    REQUIRES ServoMotorController CameraController WebSocketServer)
