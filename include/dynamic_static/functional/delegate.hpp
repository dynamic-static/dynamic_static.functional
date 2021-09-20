
/*
==========================================
  Copyright (c) 2011-2021 dynamic_static
      Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#pragma once

#include "dynamic_static/functional/action.hpp"
#include "dynamic_static/functional/subscribable.hpp"

#include <cassert>
#include <functional>
#include <utility>

namespace dst {

/**
Encapsulates a Subscribable multicast Action<>
@param <...Args> The argument types of thie Delegate<> object's Action<>
*/
template <typename ...Args>
class Delegate
    : private Subscribable
{
public:
    /**
    Constructs an instance of Delegate<>
    */
    Delegate() = default;

    /**
    Constructs an instance of Delegate<>
    @param <ActionType> The type of object to assign to this Delegate<> object's Action<>
    @param [in] action This Delegate<> object's Action<>
        @note ActionType must have a signautre compatible with this Delegate<> object's <...Args> parameter
        @note Passing nullptr for action will clear this Delegate<> object's Action<>
    */
    template <typename ActionType>
    inline Delegate(ActionType action)
        : mAction { action }
    {
    }

    /**
    Assigns this Delegate<> object's Action<>
    @param <ActionType> The type of object to assign to this Delegate<> object's Action<>
    @param [in] action This Delegate<> object's Action<>
    @return A reference to this Delegate<>
        @note ActionType must have a signautre compatible with this Delegate<> object's <...Args> parameter
        @note Passing nullptr for action will clear this Delegate<> object's Action<>
    */
    template <typename ActionType>
    inline Delegate<Args...>& operator=(ActionType action)
    {
        mAction = action;
        return *this;
    }

    /**
    Moves an instance of Delegate<>
    @param [in] other The Delegate<> to move from
    */
    inline Delegate(Delegate<Args...>&& other) noexcept
    {
        *this = std::move(other);
    }

    /**
    Moves an instance of Delegate<>
    @param [in] other The Delegate<> to move from
    @return A reference to this Delegate<>
    */
    inline Delegate<Args...>& operator=(Delegate<Args...>&& other) noexcept
    {
        Subscribable::operator=(std::move(other));
        mAction = std::move(other.mAction);
        other.mAction = nullptr;
        return *this;
    }

    /**
    Adds a subscriber to this Delegate<>
    @param [in] subscriber The Delegate<> subscribing to this Delegate<>
    @return A reference to this Delegate<>
        @note This method is a noop if it would cause a duplicate subscription
        @note This method is a noop if it would cause a self subscription
    */
    inline Delegate<Args...>& operator+=(Delegate<Args...>& subscriber)
    {
        Subscribable::operator+=(subscriber);
        return *this;
    }

    /**
    Removes a subscriber from this Delegate<>
    @param [in] subscriber The Delegate<> unsubscribing from this Delegate<>
    @return A reference to this Delegate<>
        @note This method is a noop if the given Delegate<> is not subscribed to this Delegate<>
    */
    inline Delegate<Args...>& operator-=(Delegate<Args...>& subscriber)
    {
        Subscribable::operator-=(subscriber);
        return *this;
    }

    /**
    Calls this Delegate<> object's Action<> and that of all subscribed Delegate<> objects (recursively) with the given arguments
    @param [in] args The arguments to call this Delegate<> object's Action<> and all subscribed Delegate<> objects (recursively) with
        @note The order that subscribed Delegate<> objects are called in is nondetermninistic; ie. it is not necessarily the order they were subscribed in
        @note This Delegate<> object and subscribed Delegate<> objects (recursively) must not add or remove subscribers during the scope of this method
        @note This Delegate<> object and subscribed Delegate<> objects (recursively) must not std::move() during the scope of this method
        @note This Delegate<> object and subscribed Delegate<> objects (recursively) must not be destroyed during the scope of this method
    */
    inline void operator()(Args&&... args) const
    {
        if (mAction) {
            mAction(std::forward<Args>(args)...);
        }
        for (auto pSubscriber : Subscribable::get_subscribers()) {
            assert(pSubscriber);
            (*(const Delegate<Args...>*)pSubscriber)(std::forward<Args>(args)...);
        }
    }

    /**
    Removes all subscribers from this Delegate<>
    */
    inline void clear_subscribers()
    {
        Subscribable::clear_subscribers();
    }

    /**
    Removes all subscriptions to this Delegate<>
    */
    inline void clear_subscriptions()
    {
        Subscribable::clear_subscriptions();
    }

    /**
    Clears this Delegate<> object's Action<> and removes all subscribers from and subscriptions to this Delegate<>
    */
    inline void clear()
    {
        Subscribable::clear();
        mAction = nullptr;
    }

private:
    Action<Args...> mAction;
};

} // namespace dst
