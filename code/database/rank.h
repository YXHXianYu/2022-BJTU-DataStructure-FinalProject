#ifndef RANK_H
#define RANK_H
#include <QDebug>
#include <utility>

#include "sqliteDb.h"

class Rank {
   private:
    static Rank* rank;

   private:
    Rank(){};
    ~Rank(){};
    Rank(const Rank&);
    Rank& operator=(const Rank&);

   public:
    static Rank* GetInstance() {
        if (rank == nullptr) {
            rank = new Rank();
        }
        return rank;
    }

    void Insert(std::string id, int score);
    void Query(std::vector<std::pair<std::string, int>>& pairs);
    int Query(std::string id);
};

#endif  // RANK_H
