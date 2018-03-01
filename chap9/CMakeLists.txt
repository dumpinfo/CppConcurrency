add_executable(SimpleThreadPoolTest ../chap8/JoinThreads.h
        SimpleThreadPool.h SimpleThreadPoolTest.cpp)

add_executable(ThreadPoolTest FunctionWrapper.h ThreadPool.h ThreadPoolTest.cpp)

add_executable(WorkStealingTest WorkStealingQueue.h WorkStealingThreadPool.h WorkStealingTest.cpp)