#include <gtest/gtest.h>

#include <iostream>

#include "BasicSpatial.hpp"
#include "Point.h"
#include "Validator.h"
#include "utils/DataGenerator.hpp"

using namespace utec::spatial;

TEST(SimpleTest, basicTest) {
  using data_t = double;
  using point_t = Point<data_t, 1>;

  const std::size_t num_points = 10000;
  const data_t min = 0, max = 1000;

  DataGenerator1D<data_t> generator(num_points, min, max);
  auto points = generator.get_data();

  point_t query_min = point_t({max * 0.2});
  point_t query_max = point_t({max * 0.8});

  BasicSpatial<point_t> instancia;
  Validator<point_t> validator;

  for (auto& p : points) {
    instancia.insert(p);
    validator.insert(p);
  }

  auto reference_result = validator.range(query_min, query_max);
  auto result = instancia.range(query_min, query_max);

  std::cout << reference_result.size() << "\n";
  EXPECT_EQ(reference_result.size(), result.size());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}