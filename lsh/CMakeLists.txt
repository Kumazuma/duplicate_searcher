cmake_minimum_required(VERSION 3.1.0)
project(LSH VERSION 1.0.2)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(TARGET_ARCH unknown CACHE STRING "Target Architecture for simd")
set(SIMD CACHE STRING "Simd")
set_property(CACHE TARGET_ARCH PROPERTY STRINGS unknown x86 x64 arm)
set_property(CACHE SIMD PROPERTY STRINGS sse2 ssse3 avx2)
if(${TARGET_ARCH} MATCHES "unknown")
	if(${CMAKE_MODULE_LINKER_FLAGS} MATCHES "x86")
		set(TARGET_ "x86")
	elseif(${CMAKE_MODULE_LINKER_FLAGS} MATCHES "x64")
		set(TARGET_ "x64")
	elseif(${CMAKE_MODULE_LINKER_FLAGS} MATCHES "arm")
		set(TARGET_ "arm")
	endif()
endif()

if(${TARGET_} MATCHES "x64")
	set(SRCS 
		src/x86_64/cpu_info_ia32.c
		src/x86_64/hmac.c
		src/x86_64/lsh.c
		src/x86_64/no_arch/lsh256.c
		src/x86_64/no_arch/lsh512.c
		src/x86_64/avx2/lsh256_avx2.c
		src/x86_64/avx2/lsh512_avx2.c
		src/x86_64/ssse3/lsh256_ssse3.c
		src/x86_64/ssse3/lsh512_ssse3.c
		src/x86_64/sse2/lsh256_sse2.c
		src/x86_64/sse2/lsh512_sse2.c
		src/x86_64/xop/lsh256_xop.c
		src/x86_64/xop/lsh512_xop.c
	)
elseif(${TARGET_} MATCHES "x86")
	set(SRCS 
		src/x86_64/cpu_info_ia32.c
		src/x86_64/hmac.c
		src/x86_64/lsh.c
		src/x86_64/no_arch/lsh256.c
		src/x86_64/no_arch/lsh512.c
		src/x86_64/avx2/lsh256_avx2.c
		src/x86_64/avx2/lsh512_avx2.c
		src/x86_64/ssse3/lsh256_ssse3.c
		src/x86_64/ssse3/lsh512_ssse3.c
		src/x86_64/sse2/lsh256_sse2.c
		src/x86_64/sse2/lsh512_sse2.c
		src/x86_64/xop/lsh256_xop.c
		src/x86_64/xop/lsh512_xop.c
	)
elseif(${TARGET_} MATCHES "arm")
set(SRCS
	src/general/hmac.c
	src/general/lsh.c
	src/general/lsh256.c
	src/general/lsh512.c
)
else()
set(SRCS
	src/general/hmac.c
	src/general/lsh.c
	src/general/lsh256.c
	src/general/lsh512.c
)
endif()
message(STATUS "Platform is " ${TARGET_})
add_library(lsh ${SRCS})