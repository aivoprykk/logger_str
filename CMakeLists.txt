
SET(SRCS numstr.c strbf.c)
SET(INC include)
FILE(GLOB HDRS ${INC}/*.h)

if(ESP_PLATFORM)

#name is set to logger_str for ESP32

idf_component_register(
    SRCS ${SRCS}
    INCLUDE_DIRS ${INC}
    PRIV_REQUIRES logger_common
)
ELSE()

## name is strutil for standalone
cmake_minimum_required(VERSION 3.12.4)

set(PROJECT_NAME strutil)
set(name ${PROJECT_NAME})
string(TOUPPER ${name} PACKAGE_NAME_U)

IF(NOT DEFINED ${PACKAGE_NAME_U}_BUILD_STATIC)
option(${PACKAGE_NAME_U}_BUILD_STATIC  "Build ${PACKAGE_NAME_U} static library" ON)
ENDIF()
IF(NOT DEFINED ${PACKAGE_NAME_U}_BUILD_SHARED)
option(${PACKAGE_NAME_U}_BUILD_SHARED  "Build ${PACKAGE_NAME_U} shared library" ON)
ENDIF()

MESSAGE("-----------------")
MESSAGE("SRCS:         ${SRCS}")
MESSAGE("INC:          ${INC}")
MESSAGE("HDRS:         ${HDRS}")
MESSAGE("PROJECT_NAME: ${PROJECT_NAME}")
MESSAGE("BUILD STATIC: ${${PACKAGE_NAME_U}_BUILD_STATIC}")
MESSAGE("BUILD SHARED: ${${PACKAGE_NAME_U}_BUILD_SHARED}")
MESSAGE("-----------------")

project(${PROJECT_NAME} HOMEPAGE_URL https://github.com/aivoprykk/logger_str.git)

FILE(COPY ${HDRS} DESTINATION ${CMAKE_BINARY_DIR}/include)

include_directories(
  ${CMAKE_BINARY_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(${PACKAGE_NAME_U}_BUILD_SHARED OR ${PACKAGE_NAME_U}_BUILD_STATIC)
if (${PACKAGE_NAME_U}_BUILD_STATIC)
add_library(${name} STATIC ${SRCS})
else()
add_library(${name} SHARED ${SRCS})
endif()
else()
add_library(${name} ${SRCS})
endif()

target_include_directories(${name} PUBLIC ${INC})

install(TARGETS ${name}
  LIBRARY DESTINATION ${INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${INSTALL_LIBDIR})
install(FILES ${HDRS} DESTINATION "${INSTALL_INCLUDEDIR}")

ENDIF()
