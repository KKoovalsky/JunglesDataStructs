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
