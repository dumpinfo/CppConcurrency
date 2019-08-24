add_executable(SimpleThreadPoolTest ../chap8/JoinThreads.h
        SimpleThreadPool.h SimpleThreadPoolTest.cpp)
target_link_libraries(SimpleThreadPoolTest pthread)

add_executable(ThreadPoolTest FunctionWrapper.h ThreadPool.h ThreadPoolTest.cpp)
target_link_libraries(ThreadPoolTest pthread)

add_executable(WorkStealingTest WorkStealingQueue.h WorkStealingThreadPool.h WorkStealingTest.cpp)
target_link_libraries(WorkStealingTest pthread)