add_executable(crosshair-app
    src/main.cpp
)

target_include_directories(crosshair-app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(crosshair-app PRIVATE
    glad
    glfw3
    imgui
    khr
)

set(APP_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)

add_custom_command(TARGET crosshair-app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${APP_RESOURCES_DIR}/fonts
        $<TARGET_FILE_DIR:crosshair-app>/fonts
    COMMENT "Copying application resources"
)
