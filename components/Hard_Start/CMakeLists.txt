file(GLOB_RECURSE SOURCES ./JB_T6K71_lcd/*.c
                          ./lvgl_port/*.c
                          ./Encoder/*.c
                          ./SD/*.c
                          ./lvgl_register/*.c
                          )
                          
idf_component_register(SRCS  ${SOURCES} "Hard_Start.c"                             
                    INCLUDE_DIRS "include" 
                                 "./JB_T6K71_lcd/include" 
                                 "./lvgl_port/include" 
                                 "./Encoder/include" 
                                 "./SD/include"
                                 "./lvgl_register/include"
                                 "."
                    REQUIRES lvgl esp_lcd driver sdmmc fatfs esp_new_jpeg
                )
