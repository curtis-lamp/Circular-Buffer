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

TEST_CASE("Capacity 1 behavior") {
  CircularBuffer<int> b(1);

  CHECK(b.is_empty());
  CHECK(b.push(1));
  CHECK(b.is_full());
  CHECK(!b.push(2));

  CHECK(b.pop() == 1);
  CHECK(b.is_empty());

  CHECK(b.push(3));
  CHECK(b.pop() == 3);
  CHECK(b.is_empty());
}

TEST_CASE("Size and capacity") {
  CircularBuffer<int> b(5);

  CHECK(b.size() == 0);
  CHECK(b.capacity() == 5);

  b.push(1);
  b.push(2);
  CHECK(b.size() == 2);

  b.pop();
  CHECK(b.size() == 1);

  b.pop();
  CHECK(b.size() == 0);
}

TEST_CASE("Stress wrap-around with many operations") {
  CircularBuffer<int> b(4);

  for (int round = 0; round < 1000; ++round) {
    CHECK(b.push(1));
    CHECK(b.push(2));
    CHECK(b.push(3));
    CHECK(b.push(4));
    CHECK(!b.push(5)); // full

    CHECK(b.pop() == 1);
    CHECK(b.pop() == 2);
    CHECK(b.pop() == 3);
    CHECK(b.pop() == 4);
    CHECK(b.is_empty());
    CHECK(b.pop() == std::nullopt);
  }
}

TEST_CASE("Move-only types work") {
  CircularBuffer<std::unique_ptr<int>> b(2);

  auto p1 = std::make_unique<int>(10);
  auto p2 = std::make_unique<int>(20);

  CHECK(b.push(std::move(p1)));
  CHECK(p1 == nullptr);

  CHECK(b.push(std::move(p2)));
  CHECK(p2 == nullptr);

  auto v1 = b.pop();
  REQUIRE(v1.has_value());
  CHECK(**v1 == 10);

  auto v2 = b.pop();
  REQUIRE(v2.has_value());
  CHECK(**v2 == 20);
}

TEST_CASE("Copy types work") {
  CircularBuffer<std::string> b(2);

  std::string s1 = "hello";
  std::string s2 = "world";

  CHECK(b.push(s1));
  CHECK(b.push(s2));

  auto v1 = b.pop();
  auto v2 = b.pop();

  CHECK(v1 == "hello");
  CHECK(v2 == "world");
}

TEST_CASE("Alternating push/pop") {
  CircularBuffer<int> b(3);

  for (int i = 0; i < 100; ++i) {
    CHECK(b.push(i));
    auto v = b.pop();
    REQUIRE(v.has_value());
    CHECK(*v == i);
    CHECK(b.is_empty());
  }
}

struct Big {
  std::array<int, 1024> data;
  int id;
};

TEST_CASE("Large object behavior") {
  CircularBuffer<Big> b(2);

  Big a{{}, 1};
  Big c{{}, 2};

  CHECK(b.push(a));
  CHECK(b.push(c));

  auto v1 = b.pop();
  auto v2 = b.pop();

  CHECK(v1->id == 1);
  CHECK(v2->id == 2);
}

TEST_CASE("Failed push does not alter state") {
  CircularBuffer<int> b(2);

  CHECK(b.push(1));
  CHECK(b.push(2));
  CHECK(b.is_full());

  CHECK(!b.push(3)); // should fail cleanly

  CHECK(b.pop() == 1);
  CHECK(b.pop() == 2);
  CHECK(b.is_empty());
}

TEST_CASE("FIFO ordering under wrap pressure") {
  CircularBuffer<int> b(3);

  b.push(1);
  b.push(2);
  b.push(3);
  b.pop();
  b.push(4);
  b.pop();
  b.push(5);

  CHECK(b.pop() == 3);
  CHECK(b.pop() == 4);
  CHECK(b.pop() == 5);
  CHECK(b.is_empty());
}
