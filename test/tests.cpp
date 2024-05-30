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

// Test case 1: Test unlocking the door
TEST_F(TimedDoorTest, UnlockDoor) {
  EXPECT_FALSE(door->isOpen());
  door->unlock();
  EXPECT_TRUE(door->isOpen());
}

// Test case 2: Test locking the door
TEST_F(TimedDoorTest, LockDoor) {
  door->unlock();
  EXPECT_TRUE(door->isOpen());
  door->lock();
  EXPECT_FALSE(door->isOpen());
}

// Test case 3: Test door timeout exception
TEST_F(TimedDoorTest, DoorTimeoutThrowsException) {
  door->unlock();
  EXPECT_THROW(door->checkState(), std::runtime_error);
}

// Test case 4: Test door remains closed after timeout
TEST_F(TimedDoorTest, DoorRemainsClosedAfterTimeout) {
  door->lock();
  EXPECT_NO_THROW(door->checkState());
}

// Test case 5: Test setting timeout
TEST_F(TimedDoorTest, GetTimeout) {
  EXPECT_EQ(door->getTimeout(), 5);
}

// Test case 6: Test unlocking already opened door throws exception
TEST_F(TimedDoorTest, UnlockAlreadyOpenedDoorThrowsException) {
  door->unlock();
  EXPECT_THROW(door->unlock(), std::logic_error);
}

// Test case 7: Test locking already closed door throws exception
TEST_F(TimedDoorTest, LockAlreadyClosedDoorThrowsException) {
  door->lock();
  EXPECT_THROW(door->lock(), std::logic_error);
}

// Test case 8: Test timer calls onTimeout on client
TEST_F(TimedDoorTest, TimerCallsOnTimeoutOnClient) {
  EXPECT_CALL(*mockClient, onTimeout()).Times(1);
  timer->registerClient(1, mockClient);
}

// Test case 9: Test timer sleep function
TEST_F(TimedDoorTest, TimerSleepFor) {
  timer->sleepFor(1);
  EXPECT_TRUE(true); // Just ensuring no exceptions or errors
}

// Test case 10: Test adapter timeout handling
TEST_F(TimedDoorTest, AdapterTimeoutHandling) {
  door->unlock();
  DoorTimeoutAdapter adapter(*door);
  EXPECT_THROW(adapter.onTimeout(), std::runtime_error);
}

// Test case 11: Test door remains closed after lock
TEST_F(TimedDoorTest, DoorRemainsClosedAfterLock) {
  door->lock();
  EXPECT_FALSE(door->isOpen());
}

// Test case 12: Test door remains open after unlock
TEST_F(TimedDoorTest, DoorRemainsOpenAfterUnlock) {
  door->unlock();
  EXPECT_TRUE(door->isOpen());
}

// Test case 13: Test door is initially closed
TEST_F(TimedDoorTest, DoorInitiallyClosed) {
  EXPECT_FALSE(door->isOpen());
}

// Test case 14: Test door throws exception on timeout when open
TEST_F(TimedDoorTest, DoorThrowsOnTimeoutWhenOpen) {
  door->unlock();
  EXPECT_THROW(door->checkState(), std::runtime_error);
}

// Test case 15: Test timer registration with valid client
TEST_F(TimedDoorTest, TimerRegisterClientWithValidClient) {
  MockTimerClient validClient;
  EXPECT_CALL(validClient, onTimeout()).Times(1);
  timer->registerClient(1, &validClient);
}