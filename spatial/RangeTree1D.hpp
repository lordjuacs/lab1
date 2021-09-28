#pragma once

#include "SpatialBase.h"

namespace utec {
namespace spatial {

// Node to store Point in AVL Tree
template <typename Point>
struct NodeAVL {
  Point key;
  int height;
  NodeAVL<Point> *left;
  NodeAVL<Point> *right;

  NodeAVL() : left(nullptr), right(nullptr), height(0) {}

  NodeAVL(Point key) : key(key), left(nullptr), right(nullptr), height(0) {}

  void killSelf() {
    if (left != nullptr) left->killSelf();
    if (right != nullptr) right->killSelf();
    delete this;
  }
};

/**
 * RangeTree1D implementation
 */
template <typename Point>
class RangeTree1D : public SpatialBase<Point> {
 private:
  NodeAVL<Point> *root;

  void displayPreOrder(NodeAVL<Point> *node) {
    if (node == nullptr) return;
    displayPreOrder(node->left);
    std::cout << node.get(0) << "\n";
    displayPreOrder(node->right);
  }

  bool find(NodeAVL<Point> *node, Point key) {
    if (node == nullptr)
      return false;
    else if (key.get(0) < node->key.get(0))
      return find(node->left, key);
    else if (key.get(0) > node->key.get(0))
      return find(node->right, key);
    else
      return true;
  }

  int height(NodeAVL<Point> *node) {
    if (node == nullptr)
      return -1;
    else
      return node->height;
  }

  bool isBalanced(NodeAVL<Point> *node) {
    if (node == nullptr)
      return true;
    else
      return abs(height(node->left) - height(node->right)) <= 1 &&
             isBalanced(node->left) && isBalanced(node->right);
  }

  Point minValue(NodeAVL<Point> *node) {
    if (node == nullptr)
      throw std::out_of_range("The tree is empty");
    else if (node->left == nullptr)
      return node->key;
    else
      return minValue(node->left);
  }

  Point maxValue(NodeAVL<Point> *node) {
    if (node == nullptr)
      throw std::out_of_range("The tree is empty");
    else if (node->right == nullptr)
      return node->key;
    else
      return maxValue(node->right);
  }

  int size(NodeAVL<Point> *node) {
    if (node == nullptr)
      return 0;
    else
      return 1 + size(node->left) + size(node->right);
  }

  void remove(NodeAVL<Point> *&node, Point key) {
    if (node == nullptr)
      return;
    else if (key.get(0) < node->key.get(0)) {
      remove(node->left, key);
      balance(node);
    } else if (key.get(0) > node->key.get(0)) {
      remove(node->right, key);
      balance(node);
    } else {
      if (node->left == nullptr && node->right == nullptr) {
        delete node;  // free
        node = nullptr;
      } else if (node->left == nullptr) {
        NodeAVL<Point> *temp = node;
        node = node->right;
        delete temp;
      } else if (node->right == nullptr) {
        NodeAVL<Point> *temp = node;
        node = node->left;
        delete temp;
      } else {
        Point temp = maxValue(node->left);
        node->key = temp;
        remove(node->left, temp);
      }
    }
  }

  void displayPretty(NodeAVL<Point> *node, int level) {
    if (node == nullptr) return;
    displayPretty(node->right, level + 1);
    std::cout << std::endl;
    if (node == this->root)
      std::cout << "Root->:  ";
    else {
      for (int i = 0; i < level; i++) std::cout << "       ";
    }
    std::cout << node->key.get(0);
    displayPretty(node->left, level + 1);
  }

  /*add for avl*/
  int balancingFactor(NodeAVL<Point> *node) {
    return height(node->left) - height(node->right);
  }

  void insert(NodeAVL<Point> *&node, Point key, int &extra) {
    if (node == nullptr) {
      node = new NodeAVL<Point>(key);
      extra++;
    } else if (key.get(0) <= node->key.get(0)) {
      insert(node->left, key, extra);
      if (node->left->left == nullptr && extra == 1) {
        insert(node->left, key, extra);
      }
      balance(node);
    } else {
      insert(node->right, key, extra);
      if (node->right->right == nullptr && extra == 1) {
        insert(node->right, node->key, extra);
      }
      balance(node);
    }
    node->height = std::max(height(node->left), height(node->right)) + 1;
  }

  void balance(NodeAVL<Point> *&node) {
    int hb = balancingFactor(node);
    if (hb >= 2) {  // cargado por la izquierda
      if (balancingFactor(node->left) >= 1)
        rotateRight(node);  // rotacion simple
      else {                //<=-1, rotacion doble izquierda-derecha
        rotateLeft(node->left);
        rotateRight(node);
      }
    } else if (hb <= -2) {  // cargado por la derecha
      if (balancingFactor(node->right) <= -1)
        rotateLeft(node);  // rotacion simple
      else {               //>=1, rotacion doble derecha-izquierda
        rotateRight(node->right);
        rotateLeft(node);
      }
    }
  }

  void rotateLeft(NodeAVL<Point> *&parent) {
    NodeAVL<Point> *temp = parent->right;
    parent->right = temp->left;
    temp->left = parent;
    parent->height = std::max(height(parent->left), height(parent->right)) + 1;
    temp->height = std::max(height(temp->left), height(temp->right)) + 1;
    parent = temp;
  }

  void rotateRight(NodeAVL<Point> *&parent) {
    NodeAVL<Point> *temp = parent->left;
    parent->left = temp->right;
    temp->right = parent;
    parent->height = std::max(height(parent->left), height(parent->right)) + 1;
    temp->height = std::max(height(temp->left), height(temp->right)) + 1;
    parent = temp;
  }

 public:
  RangeTree1D() : root(nullptr) {}

  void insert(const Point &new_point) override {
    int extra = 0;
    this->insert(this->root, new_point, extra);
  }

  // El punto de referencia no necesariamente es parte del dataset
  Point nearest_neighbor(const Point &reference) override { return Point({0}); }

  // TODO
  std::vector<Point> range(const Point &min, const Point &max) override {
    return {};
  };

  bool find(Point key) { return find(this->root, key); }

  void displayPreOrder() { displayPreOrder(this->root); }

  int height() { return height(this->root); }

  Point minValue() { return minValue(this->root); }

  Point maxValue() { return maxValue(this->root); }

  bool isBalanced() { return isBalanced(this->root); }

  int size() { return size(this->root); }

  void remove(Point key) { remove(this->root, key); }

  void displayPretty() { displayPretty(this->root, 1); }
};

}  // namespace spatial
}  // namespace utec
