include(CheckTypeSize)

check_type_size(int64_t INT64T_SIZE)
check_type_size(long LONG_SIZE)
check_type_size("long long int" LONGLONG_SIZE)

if (${LONGLONG_SIZE} EQUAL 8)
	set(WCSLIB_INT64 "long long int")
elseif (${INT64T_SIZE} EQUAL 8)
	set(WCSLIB_INT64 "int64_t")
elseif (${LONG_SIZE} EQUAL 8)
	set(WCSLIB_INT64 "long")
endif (${LONGLONG_SIZE} EQUAL 8)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/wcsconfig.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/wcsconfig.h)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

find_package(Flex)

if (FLEX_VERSION GREATER 2.5.9)
	lex(LEX_SRCS
		wcsbth.l
		wcspih.l
		wcsulex.l
		wcsutrn.l
	)
else (FLEX_VERSION GREATER 2.5.9)
	set(LEX_SRCS
		flexed/wcsbth.c
		flexed/wcspih.c
		flexed/wcsulex.c
		flexed/wcsutrn.c
	)
endif (FLEX_VERSION GREATER 2.5.9)

add_library(wcs STATIC
	cel.c
	lin.c
	log.c
	prj.c
	spc.c
	sph.c
	spx.c
	tab.c
	wcs.c
	wcsfix.c
	wcshdr.c
	wcstrig.c
	wcsunits.c
	wcsutil.c

	${LEX_SRCS}
)

