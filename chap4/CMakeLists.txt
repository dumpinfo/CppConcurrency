add_executable(Async Async.cpp)
target_link_libraries(Async pthread)

add_executable(QuickSort QuickSort.cpp)
target_link_libraries(QuickSort pthread)