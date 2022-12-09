#include "stone.h"

#include <cstdlib>
#include <ctime>

int Stone::MAX_TYPE = 8;  // 这里有修改，为了测试方便，多放几种宝石（yxh）

void Stone::init_rand() { srand((unsigned int)time(0)); }

Stone::Stone() {
    id_ = 0;
    type_ = rand() % MAX_TYPE + 1;
    empty_ = 0;
}

Stone::Stone(int id) : id_(id) {
    empty_ = 0;
    type_ = rand() % MAX_TYPE + 1;
}

Stone::Stone(int id, int type) : type_(type), id_(id) { empty_ = 0; }

Stone::Stone(int id, int type, int empty) : type_(type), empty_(empty), id_(id) {}

int Stone::GetMaxType() { return MAX_TYPE; }

void Stone::SetMaxType(int new_max_type) { MAX_TYPE = new_max_type; }

int Stone::GetType() { return type_; }

int Stone::GetId() { return id_; }

void Stone::SetId(int id) { id_ = id; }

bool Stone::Empty() { return empty_; }

void Stone::SetType(int type) { type_ = type; }

void Stone::SetEmpty(bool empty) { empty_ = empty; }
