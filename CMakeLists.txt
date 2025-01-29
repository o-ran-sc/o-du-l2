#sonar qube testing
project( odu-l2 )
cmake_minimum_required( VERSION 2.8.12.2)
#No unit test present in this framework TBD in further releases.
enable_testing()
add_test(NAME drive_unit_tests COMMAND bash  ../test/unit_test.sh)
