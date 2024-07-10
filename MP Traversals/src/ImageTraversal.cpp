#include <cmath>
#include <iterator>
#include <iostream>


#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "Point.h"


#include "ImageTraversal.h"


namespace Traversals {
 /**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 *
 * @param p1 First pixel
 * @param p2 Second pixel
 * @return the difference between two HSLAPixels
 */
 double calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
   double h = fabs(p1.h - p2.h);
   double s = p1.s - p2.s;
   double l = p1.l - p2.l;


   // Handle the case where we found the bigger angle between two hues:
   if (h > 180) { h = 360 - h; }
   h /= 360;


   return sqrt( (h*h) + (s*s) + (l*l) );
 }
  /**
 * Adds a Point for the bfs traversal to visit at some point in the future.
 * @param work_list the structure which stores the list of points which need to be visited by the traversal
 * @param point the point to be added
 */
 void bfs_add(std::deque<Point> & work_list, const Point & point) {
   /** @todo [Part 1] */
   work_list.push_front(point);
 }


 /**
 * Adds a Point for the dfs traversal to visit at some point in the future.
 * @param work_list the structure which stores the list of points which need to be visited by the traversal
 * @param point the point to be added
 */
 void dfs_add(std::deque<Point> & work_list, const Point & point) {
   /** @todo [Part 1] */
   work_list.push_back(point);
 }


 /**
 * Removes and returns the current Point in the bfs traversal
 * @param work_list the structure which stores the list of points which need to be visited by the traversal
 */
 Point bfs_pop(std::deque<Point> & work_list) {
   /** @todo [Part 1] */
   if (work_list.empty()) {
     return Point();
   }
   Point p = work_list.back();
   work_list.pop_back();
   return p;
 }


 /**
 * Removes and returns the current Point in the dfs traversal
 * @param work_list the structure which stores the list of points which need to be visited by the traversal
 */
 Point dfs_pop(std::deque<Point> & work_list) {
   /** @todo [Part 1] */
   if (work_list.empty()) {
     return Point();
   }
   Point p = work_list.back();
   work_list.pop_back();
   return p;
 }


 /**
 * Initializes a ImageTraversal on a given `png` image,
 * starting at `start`, and with a given `tolerance`.
 * @param png The image this traversal is going to traverse
 * @param start The start point of this traversal
 * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
 * it will not be included in this traversal
 * @param traversal_add a function pointer to an implemented function which takes in a reference to the work list
 * and the point to be added
 * @param traversal_pop a function pointer to an implemented function which takes in a reference to the work list
 * and returns the next point to be processed in the traversal
 */
  ImageTraversal::ImageTraversal(const PNG & png, const Point & start, double tolerance, add_function traversal_add, pop_function traversal_pop) { 
    /** @todo [Part 1] */
    image_ = png;
    start_ = start;
    tol = tolerance;
    add = traversal_add;
    pop = traversal_pop;
    add(dq, start_);
  }

 /**
 * Returns an iterator for the traversal starting at the first point.
 */
  ImageTraversal::Iterator ImageTraversal::begin() {
    /** @todo [Part 1] */
    return ImageTraversal::Iterator(this->dq, image_, start_, tol, add, pop);
  }


 /**
 * Returns an iterator for the traversal one past the end of the traversal.
 */
  ImageTraversal::Iterator ImageTraversal::end() {
    /** @todo [Part 1] */
    return ImageTraversal::Iterator();
  }


 /**
 * Default iterator constructor.
 */
 ImageTraversal::Iterator::Iterator() {
   /** @todo [Part 1] */
   curData = Point();
   start_ = Point();
   end_flag = true;
   tol = 0.0;
   add = nullptr;
   pop = nullptr;


 }

  ImageTraversal::Iterator::Iterator(std::deque<Point>& work, const PNG & png, Point & start, double tolerance, add_function traversal_add, pop_function traversal_pop) {
    curData = start;
    start_ = start;
    tol = tolerance;
    add = traversal_add;
    pop = traversal_pop;
    image_ = png;
    end_flag = false;

    for (unsigned i = 0; i < png.height(); i++) {
      std::vector<int> xpos;
      for (unsigned k = 0; k < png.width(); k++) {
        xpos.push_back(0);
      }
      visited.push_back(xpos);
    }
    visited[start.y][start.x] = 1;
  }


bool ImageTraversal::Iterator::inWorkList(Point p) {
  for (unsigned i = 0; i < work_list.size(); i++) {
    if (work_list[i] == p) {
      return true;
    }
  }
  return false;
}

void ImageTraversal::Iterator::updateWorkList(Point p) {
  unsigned index = 0;
  while (index < work_list.size()) {
    if (work_list[index] == p) {
      work_list.erase(work_list.begin()+index);
    } else {
      index++;
    }
  }
}

std::deque<Point> ImageTraversal::Iterator::getWorkList() {
  return work_list;
}

bool ImageTraversal::Iterator::getEndFlag() {
  return end_flag;
}

 /**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
  ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
   /** @todo [Part 1] */
    if (firstTime || !work_list.empty()) {
      Point cur;
      if (firstTime) {
        cur = curData;
        firstTime = false;
      } else {
        cur = pop(work_list);
        while (visited[cur.y][cur.x] == 1) {
          if (work_list.empty()) {
            curData = Point();
            return *this;
          } else {
            cur = pop(work_list);
          }
        }
        visited[cur.y][cur.x] = 1;
        updateWorkList(cur);
      }

      HSLAPixel startPixel = image_.getPixel(start_.x, start_.y);

      // right
      if (cur.x + 1 < image_.width()) {
        if (tol >= calculateDelta(startPixel, image_.getPixel(cur.x+1, cur.y))) {
          if (visited[cur.y][cur.x+1] == 0) {
            Point toAdd(cur.x+1, cur.y);
            add(work_list, toAdd);
          }
        }
      }
      // down
      if (cur.y + 1 < image_.height()) {
        if (tol >= calculateDelta(startPixel, image_.getPixel(cur.x, cur.y+1))) {
          if (visited[cur.y+1][cur.x] == 0) {
            Point toAdd(cur.x, cur.y+1);
            add(work_list, toAdd);
          }
        }
      }
      // left
      if (cur.x - 1 >= 0 && cur.x - 1 < image_.width()) {
        if (tol >= calculateDelta(startPixel, image_.getPixel(cur.x-1, cur.y))) {
          if (visited[cur.y][cur.x-1] == 0) {
            Point toAdd(cur.x-1, cur.y);
            add(work_list, toAdd);
          }
        }
      }
      // up
      if (cur.y - 1 >= 0 && cur.y - 1 < image_.height()) {
        if (tol >= calculateDelta(startPixel, image_.getPixel(cur.x, cur.y-1))) {
          if (visited[cur.y-1][cur.x] == 0) {
            Point toAdd(cur.x, cur.y-1);
            add(work_list, toAdd);
          }
        }
      }
      if (!work_list.empty()) {
        curData = work_list[work_list.size()-1];
      } else {
        curData = Point();
        end_flag = true;
      }
    }

    return *this;
  }


 /**
 * Iterator accessor opreator.
 *
 * Accesses the current Point in the ImageTraversal.
 */
  Point ImageTraversal::Iterator::operator*() {
    /** @todo [Part 1] */
    if (curData == Point()) {
      return Point();
    }
    return curData;
  }


 /**
 * Iterator inequality operator.
 *
 * Determines if two iterators are not equal.
 */
  bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
    /** @todo [Part 1] */
    if (this->end_flag == true && other.end_flag == true) {
      return false;
    }
    return true;
  }
}