/**
 * @file	event_handler.hpp
 * @brief	Implements an event handler
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include <functional>
#include <list>

namespace jungles {

// --------------------------------------------------------------------------------------------------------------------
// DECLARATIONS FOR PUBLIC USE
// --------------------------------------------------------------------------------------------------------------------

/**
 * \brief       This class stores event handlers and has a method raise()
 *              which allows to mark the place where the event occurs.
 *
 * Use this class when you want to expose some event like pushing a button or scheduled wake up in the system.
 * When the event occurs then call the function raise() it will call all the registered handlers.
 * The handlers must return policy which tells the event handler whether to keep the handler or to deregister the
 * handler. This allows to control when should be the handler be deregistered.
 */
class event_handler
{
  public:
    enum policy
    {
        keep,
        deregister
    };

    void raise();
    void register_handler(std::function<policy(void)> handler);

  private:
    std::list<std::function<policy(void)>> m_handlers;
};

// --------------------------------------------------------------------------------------------------------------------
// DEFINITIONS OF PUBLIC STUFF
// --------------------------------------------------------------------------------------------------------------------
void event_handler::raise()
{
    for (auto it = m_handlers.begin(); it != m_handlers.end();)
    {
        if ((*it)() == policy::deregister)
            it = m_handlers.erase(it);
        else
            it++;
    }
}

void event_handler::register_handler(std::function<policy(void)> handler)
{
    m_handlers.emplace_back(std::move(handler));
}

} // namespace jungles

#endif /* EVENT_HANDLER_HPP */
