/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>
#include "math.h"

using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim)
{
    /**
     * @todo Implement this function!
     */
    if (first[curDim] < second[curDim]) {
      return true;
    } else if (first[curDim] > second[curDim]) {
      return false;
    }
    return first < second;
}

template <int Dim>
bool shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential)
{
    /**
     * @todo Implement this function!
     */
    int curSum = 0;
    int potSum = 0;
    for (int i = 0; i < Dim; i++) {
      curSum = (target[i] - currentBest[i])*(target[i] - currentBest[i]);
      potSum = (target[i] - potential[i])*(target[i] - potential[i]);
    }
    if (potSum < curSum) {
      return true;
    } else if (potSum > curSum) {
      return false;
    }
    return potential < currentBest;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
    if (newPoints.size() > 0) {
      std::vector<Point<Dim>> vec = newPoints;
      KDTreeRecursive(vec, 0, newPoints.size(), 0, root);
    }
}

// KDTree helper function
template <int Dim>
void KDTree<Dim>::KDTreeRecursive(vector<Point<Dim>>& vec, int left, int right, int curDim, KDTreeNode*& parent) {
  if (left < right) {
    int medianIndex = left + (right-left-1)/2;
    auto cmp = [curDim](const Point<Dim>& first, const Point<Dim>& second){return smallerDimVal(first, second, curDim);};
    select(vec.begin()+left, vec.begin()+right, vec.begin()+medianIndex, cmp);
    parent = new KDTreeNode(*(vec.begin()+medianIndex));

    KDTreeRecursive(vec, left, medianIndex, (curDim+1)%Dim, parent->left);
    KDTreeRecursive(vec, medianIndex+1, right, (curDim+1)%Dim, parent->right);
  }
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */

  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    if (root != nullptr) {
      return findNearestNeighborHelper(query, 0, root);
    }
    return NULL;
}

template<int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighborHelper(const Point<Dim>& query, int dim, KDTreeNode* curRoot) const {
  if (curRoot->left == nullptr && curRoot->right == nullptr) {
    return curRoot->point;
  }
  
  bool leftSide = false;
  Point<Dim> nearest;
  if (query[dim] < curRoot->point[dim] || (query[dim] == curRoot->point[dim] && query < curRoot->point)) {
    if (curRoot->left == nullptr) {
      nearest = curRoot->point;
    } else {
      nearest = findNearestNeighborHelper(query, (dim+1)%Dim, curRoot->left);
    }
    leftSide = true;
  } else {
    if (curRoot->right == nullptr) {
      nearest = curRoot->point;
    } else {
      nearest = findNearestNeighborHelper(query, (dim+1)%Dim, curRoot->right);
    }
  }

  if (distanceFrom(curRoot->point, query) < distanceFrom(nearest, query)) {
    nearest = curRoot->point;
  } else if (distanceFrom(curRoot->point, query) == distanceFrom(nearest, query) && curRoot->point < nearest) {
    nearest = curRoot->point;
  }

  double radius = pow(distanceFrom(query, nearest), 2);
  double splitDist = pow(curRoot->point[dim] - query[dim], 2);

  Point<Dim> tempNearest;
  if (radius >= splitDist) {
    if (leftSide) {
      if (curRoot->right != nullptr) {
        tempNearest = findNearestNeighborHelper(query, (dim+1)%Dim, curRoot->right);
      } else {
        tempNearest = curRoot->point;
      }
    } else {
      if (curRoot->left != nullptr) {
        tempNearest = findNearestNeighborHelper(query, (dim+1)%Dim, curRoot->left);
      } else {
        tempNearest = curRoot->point;
      }
    }
    if (distanceFrom(tempNearest, query) < distanceFrom(nearest, query)) {
      nearest = tempNearest;
    } else if (distanceFrom(tempNearest, query) == distanceFrom(nearest, query) && tempNearest < nearest) {
      nearest = tempNearest;
    }
  }
  return nearest;
}

template<int Dim>
double KDTree<Dim>::distanceFrom(Point<Dim> first, Point<Dim> second) const {
  double dist = 0;
  for (int i = 0; i < Dim; i++) {
    dist += pow(first[i]-second[i], 2);
  }
  dist = pow(dist, 0.5);
  return dist;
}

template <typename RandIter, typename Comparator>
void select(RandIter start, RandIter end, RandIter k, Comparator cmp)
{
    /**
     * @todo Implement this function!
     */    
    if (start != end) {
      end--;
    }
    while (start != end) {
      RandIter pivotIter = partition(start, end, k, cmp);
      
      if (k == pivotIter) {
        break;
      } else if (cmp(*k, *pivotIter)) {
        end = pivotIter-1;
      } else {
        start = pivotIter+1;
      }
    }
}

template <typename RandIter, typename Comparator>
RandIter partition(RandIter start, RandIter end, RandIter k, Comparator cmp) {
  auto temp = *k;
  *k = *end;
  *end = temp;

  RandIter storeIndex = start;

  for (RandIter i = start; i != end; ++i) {
    if (cmp(*i, *end)) {
      temp = *storeIndex;
      *storeIndex = *i;
      *i = temp;
      storeIndex++;
    }
  }
  temp = *storeIndex;
  *storeIndex = *end;
  *end = temp;
  return storeIndex;
}