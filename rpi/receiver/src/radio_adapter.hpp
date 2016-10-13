#ifndef __RADIO_ADAPTER_H__
#define __RADIO_ADAPTER_H__

#include <functional>

class RadioAdapter {
public:
  virtual ~RadioAdapter() {};
  virtual int init(int pin) = 0;
  virtual int receiveMessage(std::function<void(const packet_s &)>) = 0;
};

#endif // __RADIO_ADAPTER_H__