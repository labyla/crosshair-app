add_library(glad STATIC
    glad.cpp
)

target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(glad PUBLIC
    khr
)