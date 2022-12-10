#ifndef RANK_H
#define RANK_H
#include "sqliteDb.h"
#include <utility>
#include <QDebug>

class Rank{
private:
    static Rank* rank;
private:
    Rank() {};
    ~Rank() {};
    Rank(const Rank&);
    Rank& operator=(const Rank&);


public:
    static Rank* getRank() {
        if(rank == NULL) {
            rank = new Rank();
        }
            return rank;
    }

    void Insert(std::string id, int score);
    void Query(std::vector<std::pair<std::string, int>> &pairs);
    int Query(std::string id);
};

Rank* Rank::rank = NULL;

#endif // RANK_H
