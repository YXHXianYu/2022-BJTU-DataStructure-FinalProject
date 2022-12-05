#include "stone.h"

#include <cstdlib>
#include <ctime>

const int Stone::MAX_TYPE = 4;

Stone::Stone() {
    x_ = -1;
    y_ = -1;
    type_ = 1;
    empty_ = 1;
}

Stone::Stone(int x, int y) : x_(x), y_(y) {
    std::srand((unsigned int)time(0));
    empty_ = 0;
    type_ = rand() % MAX_TYPE + 1;
}

Stone::Stone(int x, int y, int type) : x_(x), y_(y), type_(type) { empty_ = 0; }

Stone::Stone(int x, int y, int type, int empty) : x_(x), y_(y), type_(type), empty_(empty) {}

int Stone::GetX() { return x_; }

int Stone::GetY() { return y_; }

int Stone::GetType() { return type_; }

bool Stone::Empty() { return empty_; }

void Stone::SetX(int x) { x_ = x; }

void Stone::SetY(int y) { y_ = y; }

void Stone::SetType(int type) { type_ = type; }

void Stone::SetEmpty(bool empty) { empty_ = empty; }
