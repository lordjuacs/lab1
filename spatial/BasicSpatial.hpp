
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
    this->coordinates.push_back(new_point);
  }

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor(const Point& reference) override {
    /*std::map<distance_t, Point> dis_point;
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

    // se opto por esta implementacion pues es muy simple de alterar para cuando
    // se piden devolver K neighbors (solo es offset desde begin)
    */

    // se haya el punto con la menor distancia a la referencia respecto a una
    // variable que mantiene la minima (la cual empieza siendo el primer
    // elemento del vector)
    distance_t min_dis = this->coordinates[0].distance(reference);
    Point min_point = this->coordinates[0];
    for (size_t i = 1; i < this->coordinates.size(); i++) {
      auto current_dis = this->coordinates[i].distance(reference);
      if (current_dis < min_dis) {
        min_dis = current_dis;
        min_point = this->coordinates[i];
      }
    }
    return min_point;
  }
};

}  // namespace spatial
}  // namespace utec
