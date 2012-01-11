# This file is pure madness to deal with CMake's inexplicable
# non-support for anything even vaguely resembling convenience libraries.

MACRO(add_srcgroup name)
	SET(${name}_srcs "")

	foreach(it ${ARGN})
	   get_source_file_property(TMP_PROP ${it} LOCATION)
	   SET(${name}_srcs ${${name}_srcs} ${TMP_PROP}) 
	   get_source_file_property(TMP_PROP ${it} GENERATED)
	   SET(${name}_srcs ${${name}_srcs} ${TMP_PROP}) 
	endforeach(it)

	SET(${name}_srcs ${${name}_srcs} CACHE INTERNAL "Sources and generated flags for ${name}")

	get_directory_property(${name}_include_dirs INCLUDE_DIRECTORIES)
	SET(${name}_include_dirs ${${name}_include_dirs} CACHE INTERNAL "Include directories for ${name}")

	add_custom_target(${name} ALL DEPENDS ${ARGN} COMMENT "Pseudo-target for auto-generation of targets for " ${name})
ENDMACRO(add_srcgroup)

MACRO(attach_srcgroups output_srcs)
	SET(TMP_PROP "")

	foreach(it ${ARGN})
	    include_directories(${${it}_include_dirs})

	    foreach(srcfile ${${it}_srcs})
		if (NOT TMP_PROP)
			SET(TMP_PROP ${srcfile})
		else (NOT TMP_PROP)
			LIST(APPEND ${it}_parsed_srcs ${TMP_PROP})
			set_source_files_properties(${TMP_PROP} PROPERTIES GENERATED ${srcfile})
			SET(TMP_PROP "")
		endif (NOT TMP_PROP)
	   endforeach(srcfile)

	   source_group(${it}_parsed_srcs FILES ${${it}_parsed_srcs})
	   LIST(APPEND ${output_srcs} ${${it}_parsed_srcs})
	endforeach(it)
ENDMACRO(attach_srcgroups)
