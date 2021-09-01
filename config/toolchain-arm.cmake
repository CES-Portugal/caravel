set(CMAKE_TOOLCHAIN_FILE "toolchain-arm.cmake")
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)
 
set(tools C:/SysGCC/raspberry)
SET(CMAKE_C_COMPILER ${tools}/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER ${tools}/bin/arm-linux-gnueabihf-g++)
 
# Where is the target environment
SET(CMAKE_FIND_ROOT_PATH ${tools}/arm-linux-gnueabihf/sysroot/)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
 
# Search for programs only in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
 
# Search for libraries and headers only in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
 
SET(CMAKE_ARM_COMPILATION 1)