#pragma once

#include "SpatialBase.h"

namespace utec {
namespace spatial {

// Node to store Point in BST
template <typename Point>
struct NodeBST {
  Point data;
  NodeBST<Point>* left;
  NodeBST<Point>* right;
  NodeBST() : left(nullptr), right(nullptr) {}
  explicit NodeBST(Point value) : data(value), left(nullptr), right(nullptr) {}
};

/**
 * RangeBST implementation
 */
template <typename Point>
class RangeBST : public SpatialBase<Point> {
 private:
  NodeBST<Point>* root;

  void insert(NodeBST<Point>*& node, const Point& value) {
    if (node == nullptr)
      node = new NodeBST<Point>(value);
    else if (value < node->data)
      insert(node->left, value);
    else
      insert(node->right, value);
  }
  void range(const Point& min, const Point& max, NodeBST<Point>*& node,
             std::vector<Point>& points) {
    if (node == nullptr) {
      return;
    }
    if (min.get(0) <= node->data.get(0) && max.get(0) >= node->data.get(0)) {
      this->range(min, max, node->left, points);
      this->range(min, max, node->right, points);
      points.push_back(node->data);
      return;
    } else if (node->data.get(0) < min.get(0)) {
      this->range(min, max, node->right, points);
      return;
    } else if (node->data.get(0) > max.get(0)) {
      this->range(min, max, node->left, points);
      return;
    }
  }

 public:
  RangeBST() : root(nullptr) {}
  void insert(const Point& new_point) override {
    this->insert(this->root, new_point);
  }

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor(const Point& reference) override { return Point({0}); }
  std::vector<Point> range(const Point& min, const Point& max) override {
    std::vector<Point> points;
    this->range(min, max, this->root, points);
    return points;
  };
};

}  // namespace spatial
}  // namespace utec
