#############################################################
# cmake module for finding STDHEP
#
# returns:
#   STDHEP_FOUND        : set to TRUE or FALSE
#   STDHEP_INCLUDE_DIRS : paths to STDHEP includes
#   STDHEP_LIBRARY_DIRS : paths to STDHEP libraries
#   STDHEP_LIBRARIES    : list of STDHEP libraries
#
# @author S. Lukic, Vinca Belgrade, Serbia
#############################################################



# ---------- includes ---------------------------------------------------------
SET( STDHEP_INCLUDE_DIRS STDHEP_INCLUDE_DIRS-NOTFOUND )
MARK_AS_ADVANCED( STDHEP_INCLUDE_DIRS )

FIND_PATH( STDHEP_INCLUDE_DIRS
    NAMES stdhep.h
    PATHS ${STDHEP_DIR}/src/inc
    NO_DEFAULT_PATH
)
IF( NOT STDHEP_DIR )
    FIND_PATH( STDHEP_INCLUDE_DIRS NAMES stdhep.h )
ENDIF()

IF( STDHEP_INCLUDE_DIRS )
    GET_FILENAME_COMPONENT( STDHEP_ROOT ${STDHEP_INCLUDE_DIRS} PATH )
ENDIF()


# ---------- libraries --------------------------------------------------------
INCLUDE( MacroCheckPackageLibs )

# only standard libraries should be passed as arguments to CHECK_PACKAGE_LIBS
# additional components are set by cmake in variable PKG_FIND_COMPONENTS
# first argument should be the package name
CHECK_PACKAGE_LIBS( STDHEP stdhep stdhepC Fmcfio )


# ---------- final checking ---------------------------------------------------
INCLUDE( FindPackageHandleStandardArgs )
# set STDHEP_FOUND to TRUE if all listed variables are TRUE and not empty
FIND_PACKAGE_HANDLE_STANDARD_ARGS( STDHEP DEFAULT_MSG STDHEP_INCLUDE_DIRS STDHEP_LIBRARIES )

