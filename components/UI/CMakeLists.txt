file(GLOB_RECURSE SOURCES_C ./include/ANIMATION/*.c
                          ./include/CALL_BACK/*.c
                          ./include/COMPONENTS/*.c
                          ./include/PAGE_MANAGER/*.c
                          ./include/QUICK_CONTROL/*.c
                          ./include/SCREEN/*.c
                          ./include/SRC/fonts/*.c
                          ./include/SRC/images/*.c
                          ./include/THEME_MANAGER/*.c
                          )

idf_component_register(SRCS "UI.c" ${SOURCES_C}
                    INCLUDE_DIRS "." 
                         "./include/ANIMATION/include"
                         "./include/CALL_BACK/include"
                         "./include/COMPONENTS/include"
                         "./include/PAGE_MANAGER/include"
                         "./include/QUICK_CONTROL/include"
                         "./include/SCREEN/include"
                         "./include/SRC"
                         "./include/THEME_MANAGER/include"
                    REQUIRES lvgl Hard_Start)
