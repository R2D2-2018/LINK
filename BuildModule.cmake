include (${build_environment}/flags.cmake)

if (SRC_SLAVE)
    set (sources ${sources}
        src-slave/main.cpp
    )
else (SRC_SLAVE)
    set (sources ${sources}
        src/main.cpp
    )
endif (SRC_SLAVE)

set (sources ${sources}
    src/wrap-hwlib.cpp
    src/libc-stub.cpp
    ${uart_lib}/src/hardware_uart.cpp
)

add_definitions (-DBMPTK_TARGET_arduino_due
                 -DBMPTK_TARGET=arduino_due
                 -DBMPTK_BAUDRATE=19200)

set (cxxflags
    # "-Os" ## PRevents issue regarding undefined memset, dirty fix, shoudl be looked into later
    "-fno-tree-loop-distribute-patterns" ## Prevents issue regarding undefined memset, dirty fix, should be looked into later
    "-ffunction-sections"
    "-fdata-sections"
    "-fno-exceptions"
    "-fno-rtti"
    "-fno-threadsafe-statics")

string (REGEX REPLACE ";" " " cxxflags "${cxxflags}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${cxxflags}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostartfiles -nostdlib -Wl,--defsym=STACK_SIZE=92160")

# The target chip that is compiled for:
include (${toolchain}/targets/arm32/cm3/atmel/sam3x/sam3x8e/sam3x8e.cmake)

# The flashscript used to flash the binary (optional):
include (${toolchain}/targets/arm32/cm3/atmel/sam3x/sam3x8e/flashscripts/arduino_due.cmake)
