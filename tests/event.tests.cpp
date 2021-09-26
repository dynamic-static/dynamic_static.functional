
/*
==========================================
  Copyright (c) 2016-2020 Dynamic_Static
    Patrick Purcell
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

class Publisher final
{
public:
    void publish(const std::string& str)
    {
        on_publish(str);
    }
    
    Event<Publisher, const std::string&> on_publish;
};

class Listener final
{
public:
    Listener()
    {
        setup_publish_handler();
    }
    
    Listener(Listener&& other) noexcept
    {
        *this = std::move(other);
    }
    
    Listener& operator=(Listener&& other) noexcept
    {
        publish_handler = std::move(other.publish_handler);
        sentence = std::move(other.sentence);
        setup_publish_handler();
        return *this;
    }
    
    void setup_publish_handler()
    {
        publish_handler = [this](const std::string& str)
        {
            sentence += (sentence.empty() ? std::string() : " ") + str;
        };
    }
    
    Delegate<const std::string&> publish_handler;
    std::string sentence;
};

/**
Validates that Delegate<> objects can sbuscribed to and be called via Event<>
*/
TEST_CASE("Event<>::operator+=()", "[Event<>]")
{
    Publisher publisher;
    std::vector<Listener> listeners(TestCount);
    for (auto& listener : listeners) {
        publisher.on_publish += listener.publish_handler;
    }
    publisher.publish("the");
    publisher.publish("quick");
    publisher.publish("brown");
    publisher.publish("fox");
    for (const auto& listener : listeners) {
        if (listener.sentence != "the quick brown fox") {
            FAIL();
        }
    }
}

/**
Validates that Event<> move ctor unsubscribes and resubscribes at the new address
*/
TEST_CASE("Event<>::operator=(Event<>&&)", "[Event<>]")
{
    Publisher publisher;
    std::vector<Listener> listeners(TestCount);
    for (auto& listener : listeners) {
        publisher.on_publish += listener.publish_handler;
    }
    auto movedPublisher = std::move(publisher);
    std::vector<Listener> movedListeners(TestCount);
    for (size_t i = 0; i < TestCount; ++i) {
        movedListeners[i] = std::move(listeners[i]);
    }
    publisher.publish("the");
    publisher.publish("quick");
    publisher.publish("brown");
    publisher.publish("fox");
    movedPublisher.publish("jumps");
    movedPublisher.publish("over");
    movedPublisher.publish("the");
    movedPublisher.publish("lazy");
    movedPublisher.publish("dog");
    for (size_t i = 0; i < TestCount; ++i) {
        if (!listeners[i].sentence.empty()) {
            FAIL();
        }
        if (movedListeners[i].sentence != "jumps over the lazy dog") {
            FAIL();
        }
    }
}

} // namespace tests
} // namespace dst
