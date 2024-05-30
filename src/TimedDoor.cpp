// Copyright 2024 Saratova Marina

#include <stdexcept>
#include <thread> // NOLINT [build/c++11]
#include <chrono> // NOLINT [build/c++11]

#include "TimedDoor.h"

DoorTimerAdapter::DoorTimerAdapter(TimedDoor & timedDoorRef_) : timedDoorRef
(timedDoorRef_) {}

void DoorTimerAdapter::Timeout() {
    if (timedDoorRef.isDoorOpened())
        throw std::runtime_error("Time's up!");
    return;
}

TimedDoor::TimedDoor(int timeoutValue_) : timeoutValue(timeoutValue_),
 isOpened(false) {
    timerAdapterPtr = new DoorTimerAdapter(*this);
}

bool TimedDoor::isDoorOpened() {
    return isOpened;
}

void TimedDoor::unlock() {
    if (isOpened)
        throw std::logic_error("Door is open!!!");
    isOpened = true;
}

void TimedDoor::lock() {
    if (!isOpened)
        throw std::logic_error("Door is close!!!");
    isOpened = false;
}

int TimedDoor::getTimeOut() const {
    return timeoutValue;
}

void TimedDoor::throwState() {
    timerAdapterPtr->Timeout();
}

void Timer::sleep(int time_) {
    std::this_thread::sleep_for(std::chrono::seconds(time_));
}

void Timer::tregister(int time_, TimerClient* client_) {
    this->clientPtr = client_;
    sleep(time_);
    client_->Timeout();
}
