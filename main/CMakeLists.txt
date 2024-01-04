idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES "lvgl" "LovyanGFX"
                    )
# add "ui" among REQUIRES if Squareline exported "ui" folder was copied to components folder
