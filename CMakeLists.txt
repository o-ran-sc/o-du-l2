enable_testing()
add_test(
        NAME drive_unit_tests
        COMMAND bash test/unit_test.sh
        WORKING_DIRECTORY test
)
