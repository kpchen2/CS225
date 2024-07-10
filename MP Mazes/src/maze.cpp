/* Your code here! */
#include "maze.h"
#include <numeric>
#include <bits/stdc++.h>
#include <algorithm>

SquareMaze::SquareMaze() {
    width_ = 0;
    height_ = 0;
    destination = {-1,-1};
}

void SquareMaze::makeMaze(int width, int height) {
    // initializing privates
    width_ = width;
    height_ = height;

    // check if empty
    if (width == 0 || height == 0) {
        return;
    }

    // initialize 2D vector
    for (int i = 0; i < width; i++) {
        std::vector<std::pair<bool,bool>> toAdd;
        for (int k = 0; k < height; k++) {
            // pair<right,down>
            std::pair<bool,bool> pair = {true,true};
            toAdd.push_back(pair);
        }
        vec.push_back(toAdd);
    }

    // create disjoint set
    dset.addelements(width*height);

    // initializing randomized vector
    std::vector<int> pos(width*height);
    std::iota(pos.begin(),pos.end(),0);
    std::random_shuffle(pos.begin(),pos.end());

    // find and remove random walls
    int count = 0;
    while (dset.size(0) < width*height) {
        int x = pos[count]%width;
        int y = pos[count]/width;
        if (vec[x][y].first == true && x+1 < width) {
            if (dset.find(y*width + x) != dset.find(y*width + x+1)) {
                vec[x][y].first = false;
                dset.setunion(y*width + x, y*width + x+1);
            }
        }
        if (vec[x][y].second == true && y+1 < height) {
            if (dset.find(y*width + x) != dset.find((y+1)*width + x)) {
                vec[x][y].second = false;
                dset.setunion(y*width + x, (y+1)*width + x);
            }
        }
        count++;
    }
}

bool SquareMaze::canTravel(int x, int y, int dir) const {
    // right
    if (dir == 0) {
        if (x >= 0 && x+1 < width_ && y >= 0 && y < height_) {
            if (vec[x][y].first == false) {
                return true;
            }
        }
    // down
    } else if (dir == 1) {
        if (x >= 0 && x < width_ && y >= 0 && y+1 < height_) {
            if (vec[x][y].second == false) {
                return true;
            }
        }
    // left
    } else if (dir == 2) {
        if (x >= 1 && x < width_ && y >= 0 && y < height_) {
            if (vec[x-1][y].first == false) {
                return true;
            }
        }
    // up
    } else if (dir == 3) {
        if (x >= 0 && x < width_ && y >= 1 && y < height_) {
            if (vec[x][y-1].second == false) {
                return true;
            }
        }
    }
    return false;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    if (x >= 0 && x < width_ && y >= 0 && y < width_) {
        // right
        if (dir == 0) {
            vec[x][y].first = exists;
        // down
        } else {   
            vec[x][y].second = exists;
        }
    }
}

std::vector<int> SquareMaze::solveMaze() {
    // initialize queue
    std::queue<std::pair<int,int>> queue;
    queue.push({0,0});
    queue.push({-1,-1});

    // initialize 2D vector for distances
    std::vector<std::vector<int>> dist;
    for (int i = 0; i < width_; i++) {
        std::vector<int> toAdd;
        for (int k = 0; k < height_; k++) {
            toAdd.push_back(-1);
        }
        dist.push_back(toAdd);
    }
    dist[0][0] = 0;

    // find farthest end point + label distances
    int sum = 1;
    std::pair<int,int> farthest = {-1,-1};
    while (!queue.empty()) {
        if (queue.size() == 1 && queue.front().first == -1 && queue.front().second == -1) {
            break;
        }
        int x, y;
        std::queue<std::pair<int,int>> ref = queue;
        while (!ref.empty()) {
            std::pair<int,int> cur = ref.front();
            ref.pop();
            queue.pop();
            x = cur.first;
            y = cur.second;
            // right
            if (canTravel(x,y,0) && dist[x+1][y] == -1) {
                queue.push({x+1,y});
                dist[x+1][y] = sum;
            }
            // down
            if (canTravel(x,y,1) && dist[x][y+1] == -1) {
                queue.push({x,y+1});
                dist[x][y+1] = sum;
            }
            // left
            if (canTravel(x,y,2) && dist[x-1][y] == -1) {
                queue.push({x-1,y});
                dist[x-1][y] = sum;
            }
            // up
            if (canTravel(x,y,3) && dist[x][y-1] == -1) {
                queue.push({x,y-1});
                dist[x][y-1] = sum;
            }
            // something wrong with cur.second; cur.first passes more tests
            if (y == height_-1) {
                farthest = cur;
            }
            if (ref.front().first == -1 && ref.front().second == -1) {
                sum++;
                ref.pop();
                queue.pop();
                queue.push({-1,-1});
            }
        }
    }

    // find path from end to start
    std::vector<int> ans;
    std::pair<int,int> cur = farthest;
    destination = farthest;
    int x, y;
    while (cur.first != 0 || cur.second != 0) {
        x = cur.first;
        y = cur.second;
        // goes right
        if (canTravel(x,y,0) && dist[x][y] > dist[x+1][y]) {
            ans.push_back(2);
            cur = {x+1,y};
        // goes down
        } else if (canTravel(x,y,1) && dist[x][y] > dist[x][y+1]) {
            ans.push_back(3);
            cur = {x,y+1};
        // goes left
        } else if (canTravel(x,y,2) && dist[x][y] > dist[x-1][y]) {
            ans.push_back(0);
            cur = {x-1,y};
        // goes up
        } else if (canTravel(x,y,3) && dist[x][y] > dist[x][y-1]) {
            ans.push_back(1);
            cur = {x,y-1};
        }
    }
    std::reverse(ans.begin(), ans.end());
    return ans;
}

cs225::PNG* SquareMaze::drawMaze() const {
    int mazeW = width_*10 + 1;
    int mazeH = height_*10 + 1;
    cs225::PNG* pic = new cs225::PNG(mazeW,mazeH);

    // set top/left pixels black
    for (int i = 10; i < mazeW; i++) {
        pic->getPixel(i,0).h = 0;
        pic->getPixel(i,0).s = 0;
        pic->getPixel(i,0).l = 0;
        pic->getPixel(i,0).a = 1;
    }
    for (int k = 0; k < mazeH; k++) {
        pic->getPixel(0,k).h = 0;
        pic->getPixel(0,k).s = 0;
        pic->getPixel(0,k).l = 0;
        pic->getPixel(0,k).a = 1;
    }

    //set walls black
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            if (vec[x][y].first == true) {
                for (int k = 0; k <= 10; k++) {
                    pic->getPixel((x+1)*10,y*10 + k).h = 0;
                    pic->getPixel((x+1)*10,y*10 + k).s = 0;
                    pic->getPixel((x+1)*10,y*10 + k).l = 0;
                    pic->getPixel((x+1)*10,y*10 + k).a = 1;
                }
            }
            if (vec[x][y].second == true) {
                for (int k = 0; k <= 10; k++) {
                    pic->getPixel(x*10 + k, (y+1)*10).h = 0;
                    pic->getPixel(x*10 + k, (y+1)*10).s = 0;
                    pic->getPixel(x*10 + k, (y+1)*10).l = 0;
                    pic->getPixel(x*10 + k, (y+1)*10).a = 1;
                }
            }
        }
    }

    return pic;
}

cs225::PNG* SquareMaze::drawMazeWithSolution() {
    cs225::PNG* pic = this->drawMaze();
    std::vector<int> dir = this->solveMaze();
    int x = 5;
    int y = 5;

    pic->getPixel(x,y).h = 0;
    pic->getPixel(x,y).s = 1;
    pic->getPixel(x,y).l = 0.5;
    pic->getPixel(x,y).a = 1;

    for (unsigned i = 0; i < dir.size(); i++) {
        if (dir[i] == 0) {
            for (int k = 1; k <= 10; k++) {
                x++;
                pic->getPixel(x,y).h = 0;
                pic->getPixel(x,y).s = 1;
                pic->getPixel(x,y).l = 0.5;
                pic->getPixel(x,y).a = 1;
            }
        } else if (dir[i] == 1) {
            for (int k = 1; k <= 10; k++) {
                y++;
                pic->getPixel(x,y).h = 0;
                pic->getPixel(x,y).s = 1;
                pic->getPixel(x,y).l = 0.5;
                pic->getPixel(x,y).a = 1;
            }
        } else if (dir[i] == 2) {
            for (int k = 1; k <= 10; k++) {
                x--;
                pic->getPixel(x,y).h = 0;
                pic->getPixel(x,y).s = 1;
                pic->getPixel(x,y).l = 0.5;
                pic->getPixel(x,y).a = 1;
            }
        } else if (dir[i] == 3) {
            for (int k = 1; k <= 10; k++) {
                y--;
                pic->getPixel(x,y).h = 0;
                pic->getPixel(x,y).s = 1;
                pic->getPixel(x,y).l = 0.5;
                pic->getPixel(x,y).a = 1;
            }
        }
    }

    x = destination.first;
    y = destination.second;
    for (int k = 1; k <= 9; k++) {
        pic->getPixel(x*10 + k,(y+1)*10).h = 0;
        pic->getPixel(x*10 + k,(y+1)*10).s = 0;
        pic->getPixel(x*10 + k,(y+1)*10).l = 1;
        pic->getPixel(x*10 + k,(y+1)*10).a = 1;
    }

    return pic;
}