add_executable(LaunchThread LaunchThread.cpp)
target_link_libraries(LaunchThread pthread)

add_executable(NaiveParAccmulate NaiveParAccmulate.cpp)
target_link_libraries(NaiveParAccmulate pthread)
