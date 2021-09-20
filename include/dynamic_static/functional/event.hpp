
/*
==========================================
  Copyright (c) 2011-2021 dynamic_static
      Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#pragma once

#include "dynamic_static/functional/delegate.hpp"

#include <functional>
#include <utility>

namespace dst {

/**
Encapsulates a Subscribable multicast Action<> that can be exectued only by a specified type
@param <CallerType> The type that can execute this Event<>
@param <...Args> This Event<> object's argument types
*/
template <typename CallerType, typename ...Args>
class Event
    : private Delegate<Args...>
{
public:
    /**
    Adds a subscriber to this Event<>
    @param [in] subscriber The Delegate<> subscribing to this Event<>
    @return A reference to this Event<>
        @note This method is a noop if it would cause a duplicate subscription
        @note This method is a noop if it would cause a self subscription
    */
    inline Event<CallerType, Args...>& operator+=(Delegate<Args...>& subscriber)
    {
        Delegate<Args...>::operator+=(subscriber);
        return *this;
    }

    /**
    Removes a subscriber from this Event<>
    @param [in] subscriber The Delegate<> unsubscribing from this Event<>
    @return A reference to this Event<>
        @note This method is a noop if the given Delegate<> is not subscribed to this Event<>
    */
    inline Event<CallerType, Args...>& operator-=(Delegate<Args...>& subscriber)
    {
        Delegate<Args...>::operator-=(subscriber);
        return *this;
    }

private:
    friend CallerType;

    /**
    Constructs an instance of Event<>
    */
    Event() = default;

    /**
    Moves an instance of Event<>
    @param [in] other The Event<> to move from
    */
    inline Event(Event<CallerType, Args...>&& other) noexcept
    {
        *this = std::move(other);
    }

    /**
    Moves an instance of Delegate<>
    @param [in] other The Delegate<> to move from
    @return A reference to this Delegate<>
    */
    inline Event<CallerType, Args...>& operator=(Event<CallerType, Args...>&& other) noexcept
    {
        Delegate<Args...>::operator=(std::move((Delegate<Args...>&&)other));
        return *this;
    }

    /**
    Calls this Event<> object's subscribed Delegate<> objects (recursively) with the given arguments
    @param [in] args The arguments to call this Event<> object's subscribed Delegate<> objects (recursively) with
        @note The order that subscribed Delegate<> objects are called in is nondetermninistic; ie. it is not necessarily the order they were subscribed in
        @note This Event<> object and subscribed Delegate<> objects (recursively) must not add or remove subscribers during the scope of this method
        @note This Event<> object and subscribed Delegate<> objects (recursively) must not std::move() during the scope of this method
        @note This Event<> object and subscribed Delegate<> objects (recursively) must not be destroyed during the scope of this method
    */
    inline void operator()(Args&&... args) const
    {
        Delegate<Args...>::operator()(std::forward<Args>(args)...);
    }

    /**
    Removes all subscribers from this Event<>
    */
    inline void clear_subscribers()
    {
        Delegate<Args...>::clear_subscribers();
    }

    /**
    Removes all subscriptions to this Event<>
    */
    inline void clear_subscriptions()
    {
        Delegate<Args...>::clear_subscriptions();
    }

    /**
    Removes all subscribers from and subscriptions to this Event<>
    */
    inline void clear()
    {
        Delegate<Args...>::clear();
    }
};

} // namespace dst
