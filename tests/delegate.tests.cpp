
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

/**
Validates that an Action<> can be assigned to and called via Delegate<>
*/
TEST_CASE("Delegate<>::operator=()", "[Delegate<>]")
{
    int i = 0;
    Delegate<int&> delegate = [](int& value) { ++value; };
    delegate(i);
    CHECK(i == 1);
    delegate = [](int& value) { --value; };
    delegate(i);
    CHECK(i == 0);
    delegate = nullptr;
    delegate(i);
    CHECK(i == 0);
}

/**
Validates that Delegate<> objects can be subscribed to and be called via Delegate<>
*/
TEST_CASE("Delegate<>::operator+=()", "[Delegate<>]")
{
    int targetValue = 0;
    int actualValue = 0;
    Delegate<int&> delegate;
    std::vector<Delegate<int&>> delegates(TestCount);
    for (size_t i = 0; i < delegates.size(); ++i) {
        delegate += delegates[i];
        delegates[i] = [i](int& value) { value += (int)i; };
        targetValue += (int)i;
    }
    delegate(actualValue);
    CHECK(actualValue == targetValue);
}

/**
Validates that Delegate<> move ctor unsubscribes and resubscribes at the new address
*/
TEST_CASE("Delegate<>::operator=(Delegate<>&&)", "[Delegate<>]")
{
    int targetValue = 0;
    int actualValue = 0;
    Delegate<int&> delegate;
    std::vector<Delegate<int&>> delegates(TestCount);
    for (size_t i = 1; i < delegates.size(); ++i) {
        delegates[i] = [i](int& value) { value += (int)i; };
        targetValue += (int)i;
        if (i) {
            delegates[i - 1] += delegates[i];
        }
    }
    delegate += delegates[0];
    auto movedDelegate = std::move(delegate);
    std::vector<Delegate<int&>> movedDelegates(TestCount);
    for (size_t i = 0; i < TestCount; ++i) {
        movedDelegates[i] = std::move(delegates[i]);
    }
    delegate(actualValue);
    CHECK(actualValue == 0);
    movedDelegate(actualValue);
    CHECK(actualValue == targetValue);
}

/**
Validates that destroyed Delegate<> objects remove their subscriptions
*/
TEST_CASE("Delegate<>::~Delegate()", "[Delegate<>]")
{
    int targetValue = 0;
    int actualValue = 0;
    Delegate<int&> delegate;
    {
        std::vector<Delegate<int&>> delegates(TestCount);
        for (size_t i = 0; i < delegates.size(); ++i) {
            delegate += delegates[i];
            delegates[i] = [i](int& value) { value += (int)i; };
            targetValue += (int)i;
        }
        delegate(actualValue);
        CHECK(actualValue == targetValue);
    }
    delegate(actualValue);
    CHECK(actualValue == targetValue);
}

} // namespace tests
} // namespace dst
