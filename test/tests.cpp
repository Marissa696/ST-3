// Copyright 2024 Saratova Marina

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include <chrono> // NOLINT [build/c++11]
#include <thread> // NOLINT [build/c++11]

#include "TimedDoor.h"

class MockTimerClient : public TimerClient {
 public:
    MOCK_METHOD(void, Timeout, (), (override));
};

class TimedDoorTest : public ::testing::Test {
 protected:
    TimedDoor door;
    MockTimerClient mockClient;
    Timer timer;

    TimedDoorTest() : door(1), timer() {}

    void SetUp() override {
        timer.tregister(door.getTimeOut(), &mockClient);
    }

    void TearDown() override {
        testing::Mock::VerifyAndClearExpectations(&mockClient);
    }
};

TEST(TimedDoorTest, DefaultDoorState) {
    TimedDoor door(10);
    EXPECT_FALSE(door.isDoorOpened());
    EXPECT_EQ(door.getTimeOut(), 10);
}

TEST(TimedDoorTest, UnlockLockDoor) {
    TimedDoor door(10);
    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());
    door.lock();
    EXPECT_FALSE(door.isDoorOpened());
}

TEST(TimedDoorTest, DoorRemainsOpenAfterTimeout) {
    TimedDoor door(2);
    door.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_TRUE(door.isDoorOpened());
}

TEST(TimedDoorTest, DoorClosesAutomaticallyAfterTimeout) {
    TimedDoor door(2);
    door.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_TRUE(door.isDoorOpened());
}

TEST(TimedDoorTest, UnlockWhileDoorAlreadyOpen) {
    TimedDoor door(10);
    door.unlock();
    EXPECT_THROW(door.unlock(), std::logic_error);
}

TEST(TimedDoorTest, LockWhileDoorAlreadyClosed) {
    TimedDoor door(10);
    EXPECT_THROW(door.lock(), std::logic_error);
}

TEST(TimedDoorTest, ThrowStateAfterTimeout) {
    TimedDoor door(2);
    door.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_THROW(door.throwState(), std::runtime_error);
}

TEST(TimedDoorTest, DoorRemainsClosedIfNotUnlocked) {
    TimedDoor door(5);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_FALSE(door.isDoorOpened());
}

TEST(TimedDoorTest, DoorRemainsOpenedAfterUnlock) {
    TimedDoor door(10);
    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());
}

TEST(TimedDoorTest, ThrowStateWhenTimeoutReachedWhileOpen) {
    TimedDoor door(2);
    door.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_THROW(door.throwState(), std::runtime_error);
}

TEST(TimedDoorTest, NoThrowStateWhenTimeoutNotReachedWhileOpen) {
    TimedDoor door(5);
    door.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_THROW(door.throwState());
}

TEST(TimedDoorTest, ThrowStateWhenTimeoutReachedWhileClosed) {
    TimedDoor door(2);
    door.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_THROW(door.throwState(), std::runtime_error);
}
