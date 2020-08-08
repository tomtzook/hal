
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/bbbio/bbbio)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../interface)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/bbbio/bbbio/include)

add_library(hal_impl_bbb
        STATIC
        interface.c
        dio_interface.c
        aio_interface.c
        ${BBBIO_SOURCES})

