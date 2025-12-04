#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "circular_buffer/circular_buffer.h"

using namespace circular_buffer;

TEST_CASE("Can't have zero size") { CHECK_THROWS(CircularBuffer<int>(0)); }

TEST_CASE("Test add and remove") {
  auto b = CircularBuffer<int>(10);

  CHECK(b.push(1));
  CHECK(b.push(2));
  CHECK(b.pop().value() == 1);
  CHECK(b.push(3));
  CHECK(b.pop().value() == 2);
  CHECK(b.pop().value() == 3);
}

TEST_CASE("Empty") {
  auto b = CircularBuffer<int>(10);

  CHECK(b.is_empty());
  CHECK(b.pop() == std::nullopt);
  CHECK(b.push(1));
  CHECK(!b.is_empty());
  CHECK(b.push(2));
  CHECK(!b.is_empty());
  CHECK(b.pop().value() == 1);
  CHECK(!b.is_empty());
  CHECK(b.pop().value() == 2);
  CHECK(b.is_empty());
  CHECK(b.pop() == std::nullopt);
  CHECK(b.is_empty());
  CHECK(b.pop() == std::nullopt);
}

TEST_CASE("Full") {
  auto b = CircularBuffer<int>(3);

  CHECK(!b.is_full());
  CHECK(b.push(1));
  CHECK(b.push(2));
  CHECK(b.push(3));
  CHECK(b.is_full());
  CHECK(!b.push(4));

  SUBCASE("Wrap around") {
    CHECK(b.pop() == 1);
    CHECK(b.push(4));
    CHECK(b.is_full());
    CHECK(b.pop() == 2);
    CHECK(b.pop() == 3);
    CHECK(b.push(5));
    CHECK(b.pop() == 4);
    CHECK(b.pop() == 5);
    CHECK(b.is_empty());
  }
}
