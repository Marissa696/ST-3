// Copyright 2024 Saratova Marina

#include <stdexcept>
#include <thread>
#include <chrono>
#include "TimedDoor.h"

DoorTimeoutAdapter::DoorTimeoutAdapter(TimedDoor &door) : door_(door) {}

void DoorTimeoutAdapter::onTimeout() {
    if (door_.isOpen()) {
        throw std::runtime_error("Time's up!");
    }
}

TimedDoor::TimedDoor(int timeout) : timeout_(timeout), isOpen_(false) {
    adapter_ = new DoorTimeoutAdapter(*this);
}

bool TimedDoor::isOpen() const {
    return isOpen_;
}

void TimedDoor::unlock() {
    if (isOpen_) {
        throw std::logic_error("Door is already open!");
    }
    isOpen_ = true;
}

void TimedDoor::lock() {
    if (!isOpen_) {
        throw std::logic_error("Door is already closed!");
    }
    isOpen_ = false;
}

int TimedDoor::getTimeout() const {
    return timeout_;
}

void TimedDoor::checkState() {
    adapter_->onTimeout();
}

TimedDoor::~TimedDoor() {
    delete adapter_;
}

void Timer::sleepFor(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void Timer::registerClient(int seconds, TimerClient* client) {
    client_ = client;
    sleepFor(seconds);
    client_->onTimeout();
}
