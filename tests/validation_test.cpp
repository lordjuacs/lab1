#include <gtest/gtest.h>

#include <iostream>

#include "BasicSpatial.hpp"
#include "Point.h"
#include "Validator.h"
#include "utils/DataGenerator.hpp"
#include <fstream>

using namespace utec::spatial;

TEST(SimpleTest, basicTest) {
  using data_t = int;
  using point_t = Point<data_t, 2>;


  Validator<point_t> validator;
  BasicSpatial<point_t> instancia;

  const std::size_t num_points = 10000;
  const std::size_t min = 0, max = 1000;

  DataGenerator generator(num_points, min, max);
  auto points = generator.get_data();

  for (auto& p : points) {
    validator.insert(p);
    instancia.insert(p);
  }

  std::ofstream dataset;
  dataset.open("dataset.txt");
  generator.print(dataset);
  dataset.close();

  auto reference_result = validator.nearest_neighbor(point_t({50, 50}));
  auto result = instancia.nearest_neighbor(point_t({50, 50}));

  std::ofstream output;
  output.open("output.txt");
  output<<50<<" "<<50<<" "<<0<<"\n";
  output<<reference_result.get(0)<<" "<<reference_result.get(1)<<" 1"<<"\n";
  output<<result.get(0)<<" "<<result.get(1)<<" 2"<<"\n";
  output.close();

  EXPECT_EQ(reference_result, result);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}