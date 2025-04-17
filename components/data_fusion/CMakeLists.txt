idf_component_register(SRCS "data_fusion.c"
                    INCLUDE_DIRS "include"
                    REQUIRES mpu6050 libnmea Hard_Start)
