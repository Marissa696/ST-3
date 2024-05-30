// Copyright 2024 Saratova Marina

#include <stdexcept>
#include <thread>  // NOLINT [build/c++11]
#include <chrono>  // NOLINT [build/c++11]

#include "TimedDoor.h"

DoorTimeoutAdapter::DoorTimeoutAdapter(TimedDoor &door) : door_(door) {}

void DoorTimeoutAdapter::onTimeout() {
    if (door_.isOpen())
        throw std::runtime_error("Timeout reached and door is still open!");
}

TimedDoor::TimedDoor(int timeout) : timeout_(timeout), isOpen_(false) {
    adapter_ = new DoorTimeoutAdapter(*this);
}

bool TimedDoor::isOpen() const {
    return isOpen_;
}

void TimedDoor::unlock() {
    if (isOpen_)
        throw std::logic_error("The door is already open!");
    isOpen_ = true;
}

void TimedDoor::lock() {
    if (!isOpen_)
        throw std::logic_error("The door is already closed!");
    isOpen_ = false;
}

int TimedDoor::getTimeout() const {
    return timeout_;
}

void TimedDoor::checkState() {
    adapter_->onTimeout();
}

void Timer::sleepFor(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void Timer::registerClient(int seconds, TimerClient* client) {
    client_ = client;
    sleepFor(seconds);
    client_->onTimeout();
}
