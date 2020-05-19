#include "tools/threadpool.h"

using namespace boost;
using namespace boost::phoenix::arg_names;

thread_pool::thread_pool(int threads) : shutdown(false) {
    for (unsigned i = 0; i < threads; ++i)
    pool.create_thread(bind(worker_thread, ref(*this)));
}

void thread_pool::enqueue(job_t job)
{
    lock_guard<mutex> lk(mx);
    _queue.push_back(job);

    cv.notify_one();
}

thread_pool::~thread_pool()
{
    shutdown = true;
    {
        lock_guard<mutex> lk(mx);
        cv.notify_all();
    }

    pool.join_all();
}
