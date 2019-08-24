add_executable(Stack Stack.h StackTest.cpp)
target_link_libraries(Stack pthread)

add_executable(QueueTest Queue.h QueueTest.cpp)
target_link_libraries(QueueTest pthread)

#add_executable(QueueTest2 Queue2.h QueueTest2.cpp)
#target_link_libraries(QueueTest2 pthread)

#add_executable(QueueTest3 Queue3.h QueueTest3.cpp)
#target_link_libraries(QueueTest3 pthread)