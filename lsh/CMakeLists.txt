cmake_minimum_required(VERSION 3.1.0)
project(LSH VERSION 1.0.2)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

if(${CMAKE_ANDROID_ARCH_ABI} MATCHES "x64")
	set(X64_COMMON_SRCS 
		src/x64/cpu_info_ia32.c
		src/x64/hmac.c
		src/x64/lsh.c
	)
	if(${SIMD} MATCHES "avx2")
		set(SRCS
			${X64_COMMON_SRCS}
			src/x64/sse2/lsh256_avx2.c
			src/x64/sse2/lsh512_avx2.c
		)
	elseif(${SIMD} MATCHES "ssse3")
		set(SRCS
			${X64_COMMON_SRCS}
			src/x64/ssse3/lsh256_ssse3.c
			src/x64/ssse3/lsh512_ssse3.c
		)
	else()
		set(SRCS
			${X64_COMMON_SRCS}
			src/x64/sse2/lsh256_sse2.c
			src/x64/sse2/lsh512_sse2.c
		)
	endif()
elseif(${CMAKE_ANDROID_ARCH_ABI} MATCHES "x86")
	set(X64_COMMON_SRCS 
		src/x64/cpu_info_ia32.c
		src/x64/hmac.c
		src/x64/lsh.c
	)
	if(${SIMD} MATCHES "avx2")
		set(SRCS
			${X64_COMMON_SRCS}
			src/x64/sse2/lsh256_avx2.c
			src/x64/sse2/lsh512_avx2.c
		)
	elseif(${SIMD} MATCHES "ssse3")
		set(SRCS
			${X64_COMMON_SRCS}
			src/x64/ssse3/lsh256_ssse3.c
			src/x64/ssse3/lsh512_ssse3.c
		)
	elseif(${SIMD} MATCHES "sse2")
		set(SRCS
			${X64_COMMON_SRCS}
			src/x64/sse2/lsh256_sse2.c
			src/x64/sse2/lsh512_sse2.c
		)
	else()
		set(SRCS
			${X64_COMMON_SRCS}
			src/x64/no_arch/lsh256.c
			src/x64/no_arch/lsh512.c
		)
	endif()
elseif(${CMAKE_ANDROID_ARCH_ABI} MATCHES "arm")

else()
set(SRCS
	src/general/hmac.c
	src/general/lsh.c
	src/general/lsh256.c
	src/general/lsh512.c
)
endif()
message(STATUS "Platform is " ${CMAKE_ANDROID_ARCH_ABI})
add_library(lsh ${SRCS})