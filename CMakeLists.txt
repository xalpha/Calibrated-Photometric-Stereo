cmake_minimum_required(VERSION 2.6)

set(PROJ_NAME CPS)
project(${PROJ_NAME})

set (CPS_VERSION_MAJOR 0)
set (CPS_VERSION_MINOR 1)
set (CPS_VERSION_PATCH 0)

set(ROOT_DIR ${CMAKE_SOURCE_DIR} CACHE STRING "root directory where CMakeLists.txt is located")

##############################################
## External libraries

######################################
## OpenMP
find_package(OpenMP REQUIRED)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
add_definitions(-DWITH_OPENMP)

######################################
## Boost
find_package(Boost
    COMPONENTS
      program_options system filesystem 
    REQUIRED
)
list( APPEND
    EXT_INCLUDE_DIRS
    ${Boost_INCLUDE_DIRS}
)
list( APPEND
    #PROJ_LIBRARY_DIRS
    EXT_LIBS_DIR
    ${Boost_LIBRARY_DIRS}
)
list( APPEND
	EXT_LIBS
    ${Boost_LIBRARIES}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_REGEX_LIBRARY}
)

##############################################
set(EXT_DIR ${ROOT_DIR}/extern CACHE STRING "directory storing all external libraries")
list( APPEND CMAKE_MODULE_PATH
  ${EXT_DIR}
)

##############################################
## find Eigen3
find_package( Eigen3 REQUIRED )
list( APPEND
    EXT_INCLUDE_DIRS
    ${EIGEN3_INCLUDE_DIR}
)

######################################
## find ceres
#find_package( Ceres REQUIRED )
#list( APPEND
#	EXT_INCLUDE_DIRS
#	${CERES_INCLUDE_DIRS}
#)
#list( APPEND
#  EXT_LIBS
#  ${CERES_LIBRARIES}
#)

##############################################
## find CImg
find_package( CImg REQUIRED )
list( APPEND
    EXT_INCLUDE_DIRS
    ${CImg_INCLUDE_DIRS}
)
list( APPEND
    EXT_LIBS_DIR
    ${CImg_SYSTEM_LIBS_DIR}
)
list( APPEND
    EXT_LIBS
    ${CImg_SYSTEM_LIBS}
)

# Add CIMG Flags to Compilation Flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CImg_CFLAGS}")

##############################################
## find Xerces-C
FIND_PACKAGE(XercesC REQUIRED)
list( APPEND
    EXT_INCLUDE_DIRS
    ${XERCESC_INCLUDE_DIR}
)
list( APPEND
    EXT_LIBS
    ${XERCESC_LIBRARIES}
)

##############################################
## find Code Synthesis XSD
FIND_PACKAGE(Xsd REQUIRED)
list( APPEND
    EXT_INCLUDE_DIRS
    ${XSD_INCLUDE_DIR}
)

#--------------------------------------------------------------
# main code
#--------------------------------------------------------------

set(CMAKE_CXX_FLAGS_DEBUG "-g -pg")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -pg -O3")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3")
set(CMAKE_CXX_FLAGS_MINSIZEREL "")

set(PROJ_INCLUDE_DIR
    ${ROOT_DIR}/module
)
set(PROJ_SRC_DIR 
    ${ROOT_DIR}/module
)

#--------------------------------------------------------------
# CalibratedPhotometricStereo.xsd -> CalibratedPhotometricStereo.lib

# apply code synthesis XSD
FILE( GLOB FILES ${CMAKE_SOURCE_DIR}/data/config/*.xsd )
message(STATUS "xsd: " ${FILES})
IF( NOT FILES )
  MESSAGE( FATAL "No schema files to process" )
ENDIF( NOT FILES )
FOREACH( FILE ${FILES} )
	GET_FILENAME_COMPONENT( STEM "${FILE}" NAME_WE )
	add_library(${STEM}
		${PROJ_INCLUDE_DIR}/${STEM}.hxx
		${PROJ_INCLUDE_DIR}/${STEM}.cxx
	)
	list(APPEND
		PROJ_TARGET
		${STEM}
	)
ENDFOREACH( FILE )

#--------------------------------------------------------------

FILE(GLOB PROJ_INCLUDE RELATIVE ${ROOT_DIR} ${PROJ_INCLUDE_DIR}/*.h ${PROJ_INCLUDE_DIR}/*.hpp ${PROJ_INCLUDE_DIR}/*.hxx)
FILE(GLOB PROJ_SRC RELATIVE ${ROOT_DIR} ${PROJ_SRC_DIR}/*.cpp ${PROJ_SRC_DIR}/*.cxx)
message(STATUS "PROJ_INCLUDE: " ${PROJ_INCLUDE})
message(STATUS "PROJ_SRC: " ${PROJ_SRC})

include_directories(${EXT_INCLUDE_DIRS})
link_directories(${EXT_LIBS_DIR})

add_executable(${PROJ_NAME}
	${PROJ_INCLUDE}
	${PROJ_SRC}
	${EXT_INCLUDE}
	${EXT_SRC}
)
target_link_libraries(${PROJ_NAME}
    ${EXT_LIBS}
	${PROJ_TARGET}
)
