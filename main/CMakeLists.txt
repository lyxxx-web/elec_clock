#Add sources from ui directory
set(UI_DIR ./ui)
file(GLOB_RECURSE UI_SRC ${UI_DIR}/*.c)

set(APP_DIR ./app)
file(GLOB_RECURSE APP_SRC ${APP_DIR}*.c)

idf_component_register(
    SRCS
        "main.c"
        ${APP_SRC}
        ${UI_SRC}
    INCLUDE_DIRS 
        "." 
        ${UI_DIR}
        ${APP_DIR})
