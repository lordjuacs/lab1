#include <gtest/gtest.h>

#include <iostream>

#include "BasicSpatial.hpp"
#include "Point.h"
#include "Validator.h"
#include "utils/DataGenerator.hpp"

using namespace utec::spatial;

TEST(SimpleTest, basicTest) {
  using data_t = int;
  using point_t = Point<data_t, 2>;

  DataGenerator generator;

  Validator<point_t> validator;
  BasicSpatial<point_t> instancia;

  const std::size_t num_points = 10000;
  const std::size_t min = 0, max = 1000;

  auto points = generator.generate(num_points, min, max);

  for (auto& p : points) {
    validator.insert(p);
    instancia.insert(p);
  }

  auto reference_result = validator.nearest_neighbor(point_t({50, 50}));
  auto result = instancia.nearest_neighbor(point_t({50, 50}));

  EXPECT_EQ(reference_result, result);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}