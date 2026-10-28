#pragma once
#include <concurrentqueue/moodycamel/concurrentqueue.h>
//#include <concurrentqueue/moodycamel/blockingconcurrentqueue.h>

template<typename T>
using ConcurrentQueue = moodycamel::ConcurrentQueue<T>;

//template<typename T>
//using BlockingConcurrentQueue = moodycamel::BlockingConcurrentQueue<T>;