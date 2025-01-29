#sonar qube testing
cmake_minimum_required( VERSION 2.8.12.2)
project( odu-l2 )
#No unit test present in this framework TBD in further releases.
enable_testing()
add_test(NAME drive_unit_tests COMMAND bash  ../test/unit_test.sh)
