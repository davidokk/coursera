#pragma once

#include <memory>
#include <iostream>

namespace RAII {

template<typename Provider>
struct Booking {

  Provider* provider;
  int& counter;

  Booking(Provider* prov, int& counter_) : provider(prov), counter(counter_) {}

  Booking(const Booking&) = delete;
  Booking& operator=(const Booking&) = delete;

  Booking(Booking&& other) : counter(other.counter), provider(other.provider) {
    other.provider = nullptr;
  }
  Booking& operator=(Booking&& other) {
    if (this == &other)
      return *this;
    provider = other.provider;
    other.provider = nullptr;
    counter = other.counter;
    return *this;
  }

  ~Booking() {
    if (provider)
      provider->CancelOrComplete(*this);
  }

};

}  // namespace RAII