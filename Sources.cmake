# Libraries:

link_libraries (gcc)

set (hwlib ${build_environment}/libraries/hwlib)
include_directories (${hwlib}/library)

set (catch ${build_environment}/libraries/Catch2)
include_directories (${catch}/single_include)

set (uart_lib ${build_environment}/libraries/UART_LIB)
include_directories (${uart_lib}/src)

set (sources ${sources}
    ${uart_lib}/src/mock_uart.cpp
    ${uart_lib}/src/uart_connection.cpp
)

# Source Files:

include_directories (lib/)
include_directories (src/)

set (sources ${sources}
    lib/address.cpp
    lib/link_module.cpp
    lib/slave.cpp
)
