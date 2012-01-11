# Try to find the standalone CERN Minuit 2 package
# On success, defines:
#
#  MINUIT2_FOUND - MINUIT2 located
#  MINUIT2_INCLUDE_DIR - Directory containing Minuit2/FCNBase.h
#  MINUIT2_LIBRARIES - libcfitsio.so

INCLUDE(CheckLibraryExists)

IF (NOT MINUIT2_INCLUDE_DIR)
	FIND_PATH(MINUIT2_INCLUDE_DIR Minuit2/FCNBase.h)
ENDIF (NOT MINUIT2_INCLUDE_DIR)

IF (NOT MINUIT2_LIBRARIES)
	FIND_LIBRARY(MINUIT2_LIBRARIES NAMES Minuit2)
	IF (NOT MINUIT2_LIBRARIES)
		FIND_LIBRARY(MINUIT2_LIBRARIES NAMES Minuit2Base)
	ENDIF (NOT MINUIT2_LIBRARIES)
ENDIF (NOT MINUIT2_LIBRARIES)

IF (MINUIT2_INCLUDE_DIR AND MINUIT2_LIBRARIES)
	SET(MINUIT2_FOUND TRUE )
	SET(MINUIT2_LIBRARIES )
	SET(MINUIT2_INCLUDE_DIR )

	MESSAGE(STATUS "Found Minuit2: ${MINUIT2_LIBRARIES}")
ELSE (MINUIT2_INCLUDE_DIR AND MINUIT2_LIBRARIES)
	SET(MINUIT2_FOUND FALSE)
ENDIF (MINUIT2_INCLUDE_DIR AND MINUIT2_LIBRARIES)
