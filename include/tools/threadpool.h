#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <boost/thread.hpp>
#include <boost/phoenix.hpp>
#include <boost/optional.hpp>

using namespace boost;
using namespace boost::phoenix::arg_names;

class thread_pool
{
  private:
      mutex mx;
      condition_variable cv;

      typedef function<void()> job_t;
      std::deque<job_t> _queue;

      thread_group pool;

      boost::atomic_bool shutdown;
      static void worker_thread(thread_pool& q)
      {
          while (optional<job_t> job = q.dequeue())
              (*job)();
      }
  public:
      thread_pool(int threads);
      void enqueue(job_t job);
      optional<job_t> dequeue()
      {
          unique_lock<mutex> lk(mx);
          namespace phx = boost::phoenix;

          cv.wait(lk, phx::ref(shutdown) || !phx::empty(phx::ref(_queue)));

          if (_queue.empty())
              return none;

          job_t job = _queue.front();
          _queue.pop_front();

          return job;
      }

      ~thread_pool();
};

#endif // THREADPOOL_H
