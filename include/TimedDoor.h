// Copyright 2024 Saratova Marina

#ifndef INCLUDE_TIMEDDOOR_H_
#define INCLUDE_TIMEDDOOR_H_

class TimerClient {
 public:
  virtual void onTimeout() = 0;
  virtual ~TimerClient() = default;
};

class Door {
 public:
  virtual void lock() = 0;
  virtual void unlock() = 0;
  virtual bool isOpen() const = 0;
  virtual ~Door() = default;
};

class TimedDoor;

class DoorTimeoutAdapter : public TimerClient {
 private:
  TimedDoor& door_;
 public:
  explicit DoorTimeoutAdapter(TimedDoor &door);
  void onTimeout() override;
};

class TimedDoor : public Door {
 private:
  DoorTimeoutAdapter* adapter_;
  int timeout_;
  bool isOpen_;
 public:
  explicit TimedDoor(int timeout);
  bool isOpen() const override;
  void unlock() override;
  void lock() override;
  int getTimeout() const;
  void checkState();
  ~TimedDoor();
};

class Timer {
 private:
  TimerClient* client_;
  void sleepFor(int seconds);
 public:
  void registerClient(int seconds, TimerClient* client);
};

#endif  // INCLUDE_TIMEDDOOR_H_
