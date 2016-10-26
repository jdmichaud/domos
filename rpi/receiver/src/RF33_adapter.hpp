#ifndef __RF33_H__
#define __RF33_H__

#include "RCSwitch.h"
#include "radio_adapter.hpp"

class RF33Adapter : public RadioAdapter {
public:
  RF33Adapter() {}
  ~RF33Adapter() {}
  virtual int init(int pin);
  virtual int receiveMessage(std::function<void(const packet_s &)> callback);

private:
  RCSwitch _rfSwitch;
};

#endif // __RF33_H__