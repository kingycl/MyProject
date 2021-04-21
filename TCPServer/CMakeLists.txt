set(target TCPServer)

aux_source_directory(${CMAKE_HOME_DIRECTORY}/TCPServer ${target}_SRC)
aux_source_directory(${CMAKE_HOME_DIRECTORY}/TCPCommon ${target}_SRC)
add_executable(${target} ${${target}_SRC})

target_include_directories(${target} PRIVATE
        ${CMAKE_HOME_DIRECTORY}/NetMessage
        ${CMAKE_HOME_DIRECTORY}/TCPCommon
        ${CMAKE_HOME_DIRECTORY}/MemoryPool)

if (WIN32)
    target_link_libraries(${target} PRIVATE ws2_32.lib)
else ()
    target_link_libraries(${target} PRIVATE pthread)
endif ()

target_compile_definitions(${target} PRIVATE -D_CRT_SECURE_NO_WARNINGS)