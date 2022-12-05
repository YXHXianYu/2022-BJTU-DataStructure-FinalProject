#ifndef STONE_H
#define STONE_H

class Stone {
   private:
    static const int MAX_TYPE;
    int x_, y_;
    int type_;
    bool empty_;

   public:
    Stone();
    Stone(int x, int y);
    Stone(int x, int y, int type);
    Stone(int x, int y, int type, int empty);
    int GetX();
    int GetY();
    int GetType();
    bool Empty();
    void SetX(int x);
    void SetY(int y);
    void SetType(int type);
    void SetEmpty(bool empty);
};

#endif  // STONE_H
