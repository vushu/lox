
add_executable(scanner_test scanner_test.cpp)
target_link_libraries(scanner_test PRIVATE lox)

add_test(
    NAME scanner_test
    COMMAND $<TARGET_FILE:scanner_test>
)
