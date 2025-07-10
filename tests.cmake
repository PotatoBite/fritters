#TESTS
### Add some usable files to test
set (TestRequiredExtras
    tests/test.db.sqlite
    tests/test.db.MH.sqlite
    tests/test.txt
)
set (TESTS_FOLDER ${CMAKE_BINARY_DIR}/tests)

foreach (_file ${TestRequiredExtras})
        configure_file ( ${_file} ${_file} COPYONLY)
endforeach()

#test
add_executable(test_io_strings
               tests/test_io_strings.cpp
)
set_target_properties(test_io_strings PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTS_FOLDER} )
target_include_directories(test_io_strings
    PRIVATE
    src
)
add_test(NAME test_io_strings COMMAND test_io_strings WORKING_DIRECTORY ${TESTS_FOLDER})


#test
add_executable(test_io_files
               tests/test_io_files.cpp
)
set_target_properties(test_io_files PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTS_FOLDER} )

target_include_directories(test_io_files
    PRIVATE
    src
)
add_test(NAME test_io_files COMMAND test_io_files WORKING_DIRECTORY ${TESTS_FOLDER})

#test
add_executable(test_salsa
               tests/test_salsa.cpp
)
set_target_properties(test_salsa PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTS_FOLDER} )

target_include_directories(test_salsa
    PRIVATE
    src
)
add_test(NAME test_salsa COMMAND test_salsa WORKING_DIRECTORY ${TESTS_FOLDER})

#test
add_executable(test_RC4
               tests/test_RC4.cpp
)
set_target_properties(test_RC4 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTS_FOLDER} )

target_include_directories(test_RC4
    PRIVATE
    src
)
add_test(NAME test_RC4 COMMAND test_RC4 WORKING_DIRECTORY ${TESTS_FOLDER})

add_executable(test_rsa
        tests/test_rsa.cpp
)
set_target_properties(test_rsa PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TESTS_FOLDER} )

target_include_directories(test_rsa  PRIVATE src  3rdParty)