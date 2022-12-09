#ifndef STONE_H
#define STONE_H

class Stone {
   private:
    static int MAX_TYPE;
    int type_;
    bool empty_;
    int id_;

   public:
    static void init_rand();
    Stone();
    Stone(int id);
    Stone(int id, int type);
    Stone(int id, int type, int empty);
    int GetType();
    int GetId();
    bool Empty();
    void SetType(int type);
    void SetEmpty(bool empty);
    void SetId(int id);
    static int GetMaxType();
    static void SetMaxType(int new_max_type);
};

#endif  // STONE_H
