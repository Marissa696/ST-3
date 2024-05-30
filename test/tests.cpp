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

TEST(TimedDoorTest, DoorInitialState) {
  TimedDoor door(5);
  EXPECT_FALSE(door.isOpen());
}

TEST(TimedDoorTest, DoorUnlock) {
  TimedDoor door(5);
  door.unlock();
  EXPECT_TRUE(door.isOpen());
}

TEST(TimedDoorTest, DoorLock) {
  TimedDoor door(5);
  door.unlock();
  door.lock();
  EXPECT_FALSE(door.isOpen());
}

TEST(TimedDoorTest, UnlockWhenAlreadyOpenThrows) {
  TimedDoor door(5);
  door.unlock();
  EXPECT_THROW(door.unlock(), std::logic_error);
}

TEST(TimedDoorTest, LockWhenAlreadyClosedThrows) {
  TimedDoor door(5);
  EXPECT_THROW(door.lock(), std::logic_error);
}

TEST(TimedDoorTest, TimeoutThrowsWhenDoorOpen) {
  TimedDoor door(5);
  door.unlock();
  EXPECT_THROW(door.checkState(), std::runtime_error);
}

TEST(TimedDoorTest, TimeoutDoesNotThrowWhenDoorClosed) {
  TimedDoor door(5);
  EXPECT_NO_THROW(door.checkState());
}

TEST(TimerTest, RegisterClientAndTimeout) {
  MockTimerClient client;
  Timer timer;
  EXPECT_CALL(client, onTimeout()).Times(1);
  timer.registerClient(1, &client);
}

TEST(TimerTest, SleepFunction) {
  Timer timer;
  auto start = std::chrono::steady_clock::now();
  timer.registerClient(2, nullptr);
  auto end = std::chrono::steady_clock::now();
  EXPECT_GE(std::chrono::duration_cast
  <std::chrono::seconds>(end - start).count(), 2);
}

TEST(DoorTimeoutAdapterTest, AdapterCallsOnTimeout) {
  TimedDoor door(5);
  DoorTimeoutAdapter adapter(door);
  door.unlock();
  EXPECT_THROW(adapter.onTimeout(), std::runtime_error);
}

TEST(TimedDoorTest, GetTimeout) {
  TimedDoor door(5);
  EXPECT_EQ(door.getTimeout(), 5);
}

TEST(TimedDoorTest, MultipleLockUnlock) {
  TimedDoor door(5);
  door.unlock();
  door.lock();
  EXPECT_FALSE(door.isOpen());
  door.unlock();
  EXPECT_TRUE(door.isOpen());
}

TEST(TimedDoorTest, CheckStateWithoutThrowing) {
  TimedDoor door(5);
  EXPECT_NO_THROW(door.checkState());
}

TEST(TimedDoorTest, CheckStateWithThrowing) {
  TimedDoor door(5);
  door.unlock();
  EXPECT_THROW(door.checkState(), std::runtime_error);
}

TEST(DoorTimeoutAdapterTest, AdapterWithClosedDoor) {
  TimedDoor door(5);
  DoorTimeoutAdapter adapter(door);
  EXPECT_NO_THROW(adapter.onTimeout());
}
