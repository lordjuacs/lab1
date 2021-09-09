#pragma once

#include <map>

#include "SpatialBase.h"
namespace utec {
namespace spatial {
using distance_t = double;
/**
 * BasicSpatial implementation
 */
template <typename Point>
class BasicSpatial : public SpatialBase<Point> {
 private:
  std::vector<Point> coordinates;

 public:
  BasicSpatial(){};
  void insert(const Point& new_point) override {
    // se asume que los puntos a insertar son unicos, por lo que no se valida la
    // preexistencia de alguno
    coordinates.push_back(new_point);
  }

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor(const Point& reference) override {
    std::map<distance_t, Point> dis_point;
    distance_t distance;
    // para cada coordenada hayamos la distancia al punto de referencia
    // como es un map, se ordena bajo key y "less", estando ordenado de menor a
    // mayor
    for (auto& p : coordinates) {
      distance = p.distance(reference);
      dis_point.insert(std::make_pair(distance, p));
    }
    // devolvemos el primer punto, pues es el mas cercano
    return dis_point.begin()->second;
  }
};

}  // namespace spatial
}  // namespace utec
