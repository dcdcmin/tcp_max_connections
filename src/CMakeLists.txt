set(shared_src common.c) 

add_executable(http_connection http_connection.c ${shared_src})
add_executable(mqtt_connection mqtt_connection.c ${shared_src})

target_link_libraries(mqtt_connection mqttpacket) 

if(WIN32)
    target_link_libraries(http_connection wsock32 ws2_32) 
    target_link_libraries(mqtt_connection wsock32 ws2_32) 
endif()

