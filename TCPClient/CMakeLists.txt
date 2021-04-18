set(target TCPClient)

aux_source_directory(${CMAKE_HOME_DIRECTORY}/TCPClient ${target}_SRC)

add_executable(${target} ${${target}_SRC})

target_include_directories(${target} PRIVATE
        ${CMAKE_HOME_DIRECTORY}/NetMessage
        ${CMAKE_HOME_DIRECTORY}/TCPCommon)

if (WIN32)
    target_link_libraries(${target} PRIVATE ws2_32.lib)
else ()
    target_link_libraries(${target} PRIVATE pthread)
endif ()

target_link_libraries(${target} PRIVATE TcpCommon)

target_compile_definitions(${target} PRIVATE -D_CRT_SECURE_NO_WARNINGS)