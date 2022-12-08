#ifndef STONE_H
#define STONE_H

class Stone {
   private:
    static int MAX_TYPE;
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

    static int GetMaxType();
    static void SetMaxType(int new_max_type);
};

#endif  // STONE_H
