cmake_minimum_required(VERSION 3.5)
project(stache)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#if you want to have Qt4-support uncomment this
#find_package(Qt4 REQUIRED)
#if(QT_FOUND)
## activate XML component
#set(QT_USE_QTXML TRUE)
#include(${QT_USE_FILE})
#add_definitions(${QT_DEFINITIONS})
#include_directories(${CMAKE_CURRENT_BINARY_DIR} ${QT_INCLUDE_DIR})
#endif(QT_FOUND)

find_package( OpenCV REQUIRED )
#if( OpenCV_FOUND )
#    list( APPEND ThirdParty_LIBS ${OpenCV_LIBS} )
#    include_directories( ${OpenCV_INCLUDE_DIRS} )
#endif( OpenCV_FOUND )

# uncomment this for OpenMP support
#FIND_PACKAGE(OpenMP REQUIRED)
#if(OPENMP_FOUND)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
#endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES ${SOURCE_PATH}/stache.cpp)
#for using with headers
#set(${PROJECT_NAME}_HEADERS test1.h test2.h)

# currently we don't have any forms-uncomment if you have qt-forms
#set(${PROJECT_NAME}_FORMS mainwindow.ui)

# also no moc from headers needed (uncomment if qt used)
#QT4_WRAP_CPP(${PROJECT_NAME}_HDR_MOC ${${PROJECT_NAME}_HDR})
# no forms
#QT4_WRAP_UI(${PROJECT_NAME}_FORMS_HDR ${${PROJECT_NAME}_FORMS})

# use this line if you have used Qt instead the next line
#add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC} ${${PROJECT_NAME}_HDR_MOC} ${${PROJECT_NAME}_FORMS_HDR} )

# use this if you use headers
#add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC} ${${PROJECT_NAME}_HDR})
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})