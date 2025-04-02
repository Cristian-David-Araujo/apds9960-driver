
file(GLOB_RECURSE SRCS "../src/*.c")

idf_component_register(SRCS "main.c" ${SRCS}
                    INCLUDE_DIRS "." "../include")