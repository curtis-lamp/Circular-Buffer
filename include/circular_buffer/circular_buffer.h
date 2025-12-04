#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>

namespace circular_buffer {

/**
 * This class implements a circular buffer. This is a simple implementation
 * which is not thread-safe.
 */
template <typename T> class CircularBuffer {
public:
  CircularBuffer(size_t n);
  ~CircularBuffer() = default;

  CircularBuffer(CircularBuffer const &) = delete;
  CircularBuffer &operator=(CircularBuffer const &) = delete;

  CircularBuffer(CircularBuffer &&) = delete;
  CircularBuffer &operator=(CircularBuffer &&) = delete;

  std::size_t capacity() const;

  std::size_t size() const;

  bool is_empty() const;

  bool is_full() const;

  bool push(T const &);
  bool push(T &&);

  std::optional<T> pop();

private:
  std::size_t size_ = 0;
  std::size_t head = 0;
  std::size_t tail = 0;
  std::size_t n;
  std::unique_ptr<T[]> buffer;

  std::size_t wrapped_increment(std::size_t);
};

template <typename T> CircularBuffer<T>::CircularBuffer(size_t n) : n{n} {
  if (n == 0) {
    throw std::invalid_argument("CircularBuffer capacity must be > 0");
  }

  buffer = std::make_unique<T[]>(n);
}

template <typename T> std::size_t CircularBuffer<T>::capacity() const {
  return n;
}

template <typename T> std::size_t CircularBuffer<T>::size() const {
  return size_;
}

template <typename T> bool CircularBuffer<T>::is_empty() const {
  return size() == 0;
}

template <typename T> bool CircularBuffer<T>::is_full() const {
  return size() == n;
}

template <typename T> bool CircularBuffer<T>::push(T const &elem) {
  if (is_full()) {
    return false;
  }

  buffer[tail] = elem;
  tail = wrapped_increment(tail);
  ++size_;
  return true;
}

template <typename T> bool CircularBuffer<T>::push(T &&elem) {
  if (is_full()) {
    return false;
  }

  buffer[tail] = elem;
  tail = wrapped_increment(tail);
  ++size_;
  return true;
}

template <typename T> std::optional<T> CircularBuffer<T>::pop() {
  if (is_empty()) {
    return std::nullopt;
  }

  T value = std::move(buffer[head]);
  head = wrapped_increment(head);
  --size_;
  return value;
}

template <typename T>
std::size_t CircularBuffer<T>::wrapped_increment(std::size_t ptr) {
  ++ptr;
  if (ptr == n) {
    ptr = 0;
  }
  return ptr;
}

} // namespace circular_buffer
