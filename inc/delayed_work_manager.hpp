/**
 * @file	delayed_work_manager.hpp
 * @brief	Defines the class which handles delayed or periodic work.
 * @author	Kacper Kowalski - kacper.kowalski@lerta.energy
 */

#ifndef DELAYED_WORK_MANAGER_HPP
#define DELAYED_WORK_MANAGER_HPP

#include <cassert>
#include <functional>
#include <list>

// Those structures are defined under the delayed_work_manager class.
enum class delayed_work_policy;
struct delayed_work;

/**
 * \brief Allows to execute periodically some jobs or single shot jobs with a delay.
 *
 * ACHTUNG#1: this is not thread safe.
 * ACHTUNG#2: Keep the invocation period higher than one second as the internal variables can overflow if the
 *            invocation period is high.
 *
 * \tparam InvocationPeriod The period with which the invoke() method is called.
 */
template <unsigned InvocationPeriod> class delayed_work_manager
{
  public:
    delayed_work_manager();

    //! The function which must be called periodically to invoke registered handlers properly.
    void invoke();

    /**
     * \brief Register a handler which will be called with a period or a delay.
     *
     * \param[in] calling_period A period (for delayed_work_policy::keep) or a delay (for
     *                           delayed_work_policy::deregister) with which the handler will be invoked.
     *                           MUST BE a multiply of the invocation period.
     * \param[in] handler        The function which will be called with the specific period or delay.
     *                           The handler returns delayed_work_policy which can be equal to
     *                           delayed_work_policy::keep if you want to keep this handler registered and invoke it
     *                           periodically or it can be equal to delayed_work_policy::deregister if you want to
     *                           invoke the handler finite number of times.
     */
    void register_work(unsigned calling_period, std::function<delayed_work_policy(void)> handler);

  private:
    //! Contains all the handlers and the corresponding periods.
    std::list<delayed_work> m_delayed_work_list;

    //! Counts the time passed.
    unsigned m_time_counter;
};

//! Defines what to do with the handler after it has been called
enum class delayed_work_policy
{
    //! Keep the handler registered so after the next period it will be also called.
    keep,

    //! Deregister the handler and do not call it any more.
    deregister
};

//! A container to store the handler and all its related data.
struct delayed_work
{
    std::function<delayed_work_policy(void)> m_handler;
    const unsigned m_calling_period;
    const unsigned m_start_time;

    delayed_work(std::function<delayed_work_policy(void)> handler, unsigned calling_period, unsigned start_time)
        : m_handler(handler), m_calling_period(calling_period), m_start_time(start_time)
    {
    }
};

template <unsigned InvocationPeriod> delayed_work_manager<InvocationPeriod>::delayed_work_manager() : m_time_counter(0)
{
}

template <unsigned InvocationPeriod> void delayed_work_manager<InvocationPeriod>::invoke()
{
    m_time_counter += InvocationPeriod;
    for (auto it = m_delayed_work_list.begin(); it != m_delayed_work_list.end();)
    {
        if (((m_time_counter - it->m_start_time) % it->m_calling_period) == 0)
        {
            if (it->m_handler() == delayed_work_policy::deregister)
            {
                it = m_delayed_work_list.erase(it);
                continue;
            }
        }
        ++it;
    }
}

template <unsigned InvocationPeriod>
void delayed_work_manager<InvocationPeriod>::register_work(unsigned calling_period,
                                                           std::function<delayed_work_policy(void)> handler)
{
    assert(calling_period != 0);

    // The calling period must be a multiply of the invocation period.
    assert(calling_period % InvocationPeriod == 0);

    m_delayed_work_list.emplace_back(handler, calling_period, m_time_counter);
}

#endif /* DELAYED_WORK_MANAGER_HPP */
