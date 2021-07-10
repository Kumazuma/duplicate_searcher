cmake_minimum_required(VERSION 3.1.0)
project(cmake-wx VERSION 0.1.0)

include(CTest)
enable_testing()
find_package(wxWidgets REQUIRED COMPONENTS net core base aui)
include(${wxWidgets_USE_FILE})
include_directories(AFTER include "include")
include_directories(AFTER include src)
include_directories(AFTER include ${CMAKE_SOURCE_DIR}/lsh/include)
FILE(GLOB generated_src src/generated/*.cpp)
set(CMAKE_CXX_STANDARD 17)
set(SOURCES 
	${generated_src}
	"src/app.cpp"
	"src/searcher.cpp"
	"src/hashkey.cpp"
	"src/mainform.cpp"
	 "src/duplicate_file_view_model.cpp")
add_executable(cmake-wx WIN32 ${SOURCES})
target_link_libraries(cmake-wx ${wxWidgets_LIBRARIES})
target_link_libraries(cmake-wx lsh)
if(UNIX)
target_link_libraries(cmake-wx -lX11)
endif()
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
