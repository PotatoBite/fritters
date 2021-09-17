cmake_minimum_required(VERSION 3.5)

project(potatobite.Fritters LANGUAGES CXX VERSION 0.0.15)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

set(FRITTERS_SOURCES
    #src/cipher.h
    src/ciphers.cpp
)

add_library(fritters_static STATIC
    ${FRITTERS_SOURCES}
)

add_library(fritters SHARED
    ${FRITTERS_SOURCES}
)

include(GNUInstallDirs)

set_target_properties(fritters_static fritters
                      PROPERTIES 
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      PDB_OUTPUT_DIRECTORY     "${CMAKE_BINARY_DIR}/bin"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

### Add some usable files to project build folder
set (RequiredExtras
    ""
)

#target_link_libraries( Fritters )

#preparing install step
set_target_properties(fritters PROPERTIES RESOURCE "${RequiredExtras}")

install(TARGETS fritters_static fritters
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    OBJECTS DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

#installing headers
install(DIRECTORY "./src/" # source directory
        DESTINATION "include" # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
		#PATTERN "*.c" # select header files
		#PATTERN "*.cpp" # select header files
)

#add tests
include("tests.cmake")