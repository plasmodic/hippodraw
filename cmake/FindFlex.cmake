# - Try to find Flex
# Once done this will define
#
#  FLEX_FOUND - system has Flex
#  FLEX_EXECUTABLE - path of the flex executable
#  FLEX_VERSION - the version string, like "2.5.31"
#


FIND_PROGRAM(FLEX_EXECUTABLE NAMES flex)

IF(FLEX_EXECUTABLE)
    SET(FLEX_FOUND TRUE)

    EXECUTE_PROCESS(COMMAND ${FLEX_EXECUTABLE} --version
        OUTPUT_VARIABLE _FLEX_VERSION
    )
    string (REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" FLEX_VERSION "${_FLEX_VERSION}")
ENDIF(FLEX_EXECUTABLE)

FIND_PATH(FLEX_INCLUDE_DIR FlexLexer.h )
FIND_LIBRARY(FLEX_LIBRARY fl)

IF(FLEX_FOUND)
  IF(NOT Flex_FIND_QUIETLY)
    MESSAGE(STATUS "Found Flex: ${FLEX_EXECUTABLE} ${FLEX_VERSION}")
  ENDIF(NOT Flex_FIND_QUIETLY)
ELSE(FLEX_FOUND)
  IF(Flex_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find Flex")
  ENDIF(Flex_FIND_REQUIRED)
ENDIF(FLEX_FOUND)

MACRO (LEX outfiles)
  FOREACH(it ${ARGN})
        GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
        GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
        SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/${outfile}.c)

        ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
            COMMAND ${FLEX_EXECUTABLE}
            ARGS -t ${infile} > ${outfile}
            MAIN_DEPENDENCY ${infile})

        LIST(APPEND ${outfiles} ${outfile})
  ENDFOREACH(it)
ENDMACRO (LEX)

