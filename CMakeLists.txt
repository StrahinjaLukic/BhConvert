########################################################
# cmake file for building drs4daq
# @author Strahinja Lukic
CMAKE_MINIMUM_REQUIRED( VERSION 3.0 FATAL_ERROR )
########################################################

# project name
PROJECT( BhConvert )

# project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 0 )
SET( ${PROJECT_NAME}_VERSION_MINOR 1 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )

### DEPENDENCIES ############################################################

SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmakemodules )

if ( DEFINED LCIO_DIR ) 
  message( STATUS "Including LCIO config" )
  include(${LCIO_DIR}/LCIOConfig.cmake)
else ( DEFINED LCIO_DIR ) 
  message( STATUS "Looking up LCIO" )
  FIND_PACKAGE( LCIO REQUIRED )
endif ( DEFINED LCIO_DIR ) 
FIND_PACKAGE( STDHEP )

SET( CMAKE_CXX_FLAGS  "-w -ansi -pedantic ${CMAKE_CXX_FLAGS}" )
####################################################################################################


### SUBDIRECTORIES #####################################

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

add_subdirectory (bhlumi2slcio)

if ( STDHEP_FOUND )
  add_subdirectory (bhlumi2stdhep)
endif ( STDHEP_FOUND )



