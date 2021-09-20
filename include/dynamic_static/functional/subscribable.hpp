
/*
==========================================
  Copyright (c) 2016-2021 dynamic_static
      Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#pragma once

#include <cassert>
#include <set>
#include <utility>

namespace dst {

/**
Encapsulates a collection of mutual references
*/
class Subscribable
{
public:
    /**
    Constructs an instance of Subscribable
    */
    Subscribable() = default;

    /**
    Moves an instance of Subscribable
    @param [in] other The Subscribable to move from
    */
    inline Subscribable(Subscribable&& other) noexcept
    {
        *this = std::move(other);
    }

    /**
    Destroys this instance of Subscribable
    */
    inline virtual ~Subscribable()
    {
        clear();
    }

    /**
    Moves an instance of Subscribable
    @param [in] other The Subscribable to move from
    @return A reference to this Subscribable
    */
    inline Subscribable& operator=(Subscribable&& other) noexcept
    {
        mSubscribers = std::move(other.mSubscribers);
        for (auto pSubscriber : mSubscribers) {
            assert(pSubscriber);
            pSubscriber->mSubscriptions.erase(&other);
            pSubscriber->mSubscriptions.insert(this);
        }
        mSubscriptions = std::move(other.mSubscriptions);
        for (auto pSubscription : mSubscriptions) {
            assert(pSubscription);
            pSubscription->mSubscribers.erase(&other);
            pSubscription->mSubscribers.insert(this);
        }
        return *this;
    }

    /**
    Adds a subscriber to this Subscribable
    @param [in] subscriber The Subscribable subscribing to this Subscribable
    @return A reference to this Subscribable
        @note This method is a noop if it would cause a duplicate subscription
        @note This method is a noop if it would cause a self subscription
    */
    inline Subscribable& operator+=(Subscribable& subscriber)
    {
        if (this != &subscriber) {
            mSubscribers.insert(&subscriber);
            subscriber.mSubscriptions.insert(this);
        }
        return *this;
    }

    /**
    Removes a subscriber from this Subscribable
    @param [in] subscriber The Subscribable unsubscribing from this Subscribable
    @return A reference to this Subscribable
        @note This method is a noop if the given Subscribable is not subscribed to this Subscribable
    */
    inline Subscribable& operator-=(Subscribable& subscriber)
    {
        subscriber.mSubscriptions.erase(this);
        mSubscribers.erase(&subscriber);
        return *this;
    }

    /**
    Gets this Subscribable subscribers
    @return This Subscribable object's subscribers
        @note Adding or removing sbuscribers invalidates the returned collection's iterators
        @note The order of subscribers is nondeterministic; ie. it is not necessarily the order they were subscribed in
    */
    inline const std::set<Subscribable*>& get_subscribers() const
    {
        return mSubscribers;
    }

    /**
    Gets this Subscribable subscriptions
    @return This Subscribable object's subscriptions
        @note Adding or removing subscriptions invalidates the returned collection's iterators
        @note The order of subscriptions is nondeterministic; ie it is not necessarily the order they were subscribed in
    */
    inline const std::set<Subscribable*>& get_subscriptions() const
    {
        return mSubscriptions;
    }

    /**
    Removes all subscribers from this Subscribable
    */
    inline void clear_subscribers()
    {
        for (auto pSubscriber : mSubscribers) {
            assert(pSubscriber);
            pSubscriber->mSubscriptions.erase(this);
        }
        mSubscribers.clear();
    }

    /**
    Removes all subscriptions to this Subscribable
    */
    inline void clear_subscriptions()
    {
        for (auto pSubscription : mSubscriptions) {
            assert(pSubscription);
            pSubscription->mSubscribers.erase(this);
        }
        mSubscriptions.clear();
    }

    /**
    Removes all subscribers from and subscriptions to this Subscribable
    */
    inline void clear()
    {
        clear_subscribers();
        clear_subscriptions();
    }

private:
    std::set<Subscribable*> mSubscribers;
    std::set<Subscribable*> mSubscriptions;
    Subscribable(const Subscribable&) = delete;
    Subscribable& operator=(const Subscribable&) = delete;
};

} // namespace dst
