#ifndef RANKING_H
#define RANKING_H
#include <vector>
#include "sqliteDb.h"
/*排行榜
1.初始化
2.插入一条新的记录（序号、名字、分数）
3.删除一条记录（根据序号删除）
4.查询所有记录（返回一个QList<QHash<QString,QString>>）
 */
class Rank {
public:
    bool init();
    void addRank(int id,std::string name,int rank);
    void deleteRank(int id);
    void qweryRank(int id);
    QList<QHash<QString,QString>> wholeRank();

private:

};

#endif // RANKING_H
