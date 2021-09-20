
/*
==========================================
  Copyright (c) 2016-2021 Dynamic_Static
      Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#include "dynamic_static/functional.hpp"
#include "dynamic_static/random.hpp"

#include "catch2/catch.hpp"

#include <utility>
#include <vector>

namespace dst {
namespace tests {

static constexpr int TestCount { 16 };

using SubscriptionPtrPair = std::pair<Subscribable*, Subscribable*>;

static std::vector<SubscriptionPtrPair> randomize_subscriptions(std::vector<Subscribable>& subscribables)
{
    RandomNumberGenerator rng;
    std::vector<SubscriptionPtrPair> subscriptions;
    for (auto itr = subscribables.begin(); itr != subscribables.end(); ++itr) {
        for (auto jtr = subscribables.begin(); jtr != subscribables.end(); ++jtr) {
            if (rng.probability(0.5f)) {
                subscriptions.push_back({ &*itr, &*jtr });
                *itr += *jtr;
            }
        }
    }
    return subscriptions;
}

static void validate_subscriptions(const std::vector<SubscriptionPtrPair>& subscriptionPtrPairs, bool expect)
{
    for (const auto& subscriptionPtrPair : subscriptionPtrPairs) {
        auto subscription0 = 0 < subscriptionPtrPair.first->get_subscribers().count(subscriptionPtrPair.second);
        auto subscription1 = 0 < subscriptionPtrPair.second->get_subscriptions().count(subscriptionPtrPair.first);
        if (subscriptionPtrPair.first != subscriptionPtrPair.second) {
            if (subscription0 && subscription1 != expect) {
                FAIL();
            }
        } else if (subscription0 || subscription1) {
            FAIL();
        }
    }
}

/**
Validates that Subscribable can subscribe and unsubscribe
*/
TEST_CASE("Subscribable::operator+=() and Subscribable::operator-=()", "[Subscribable]")
{
    std::vector<Subscribable> subscribables(TestCount);
    auto subscriptions = randomize_subscriptions(subscribables);
    validate_subscriptions(subscriptions, true);
    SECTION("operator-=()")
    {
        for (auto itr = subscribables.begin(); itr != subscribables.end(); ++itr) {
            for (auto jtr = itr + 1; jtr != subscribables.end(); ++jtr) {
                *itr -= *jtr;
                *jtr -= *itr;
            }
        }
        validate_subscriptions(subscriptions, false);
    }
    SECTION("clear()")
    {
        for (auto& subscribable : subscribables) {
            subscribable.clear_subscriptions();
        }
        validate_subscriptions(subscriptions, false);
    }
}

/**
Validates that Subscribable move ctor unsubscribes and resubscribes at the new address
*/
TEST_CASE("Subscribable::Subscribable(Subscribable&&)", "[Subscribable]")
{
    Subscribable subscribable0;
    Subscribable subscribable1;
    subscribable0 += subscribable1;
    auto move0 = std::move(subscribable0);
    auto move1 = std::move(subscribable1);
    CHECK(subscribable0.get_subscribers().empty());
    CHECK(subscribable1.get_subscriptions().empty());
    CHECK(move0.get_subscribers().count(&move1));
    CHECK(move1.get_subscriptions().count(&move0));
}

/**
Validates that Subscribable dtor unsubscribes
*/
TEST_CASE("Subscribable::~Subscribable()", "[Subscribable]")
{
    Subscribable subscribable;
    std::vector<Subscribable> tempSubscribables(TestCount);
    for (auto& tempSubscribable : tempSubscribables) {
        subscribable += tempSubscribable;
    }
    CHECK(subscribable.get_subscribers().size() == tempSubscribables.size());
    tempSubscribables.clear();
    CHECK(subscribable.get_subscribers().empty());
}

} // namespace tests
} // namespace dst
