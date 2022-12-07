#include "ranking.h"
/*排行榜
1.初始化
2.插入一条新的记录（序号、名字、分数）
3.删除一条记录（根据序号删除）
4.查询所有记录（返回一个QList<QHash<QString,QString>>）
 */
bool Rank::init(){
    sqliteDb db;
    db.setDbDir(QDir::currentPath());
    if(!db.reOpenSql("test")) {
        db.creatDbFile("test");
        if(!db.reOpenSql("test")) {
            qDebug() <<  "打开数据库失败";
            return false;
        }
    }
    if(!db.queryExec("test","CREATE TABLE RANKING("
                                      "   ID INT PRIMARY KEY     NOT NULL,"
                                      "   NAME           TEXT    NOT NULL,"
                                      "   RANK            INT     NOT NULL,"
                                      ");")) {
        qDebug() << db.getErrorSql();
        return false;
    }
    return true;
}

void Rank::addRank(int id,std::string name,int rank){
    //传int的id和string的name和int的rank
    sqliteDb db;
    db.setDbDir(QDir::currentPath());
    if(!db.reOpenSql("test")) {
        qDebug() <<  "打开数据库失败";
        return;
    }
    db.transaction();//开启事务
    //增加记录
    QHash<QString,QString> addHash;

    addHash["ID"]=QString::number(id);
    addHash["NAME"] = name.c_str();
    addHash["RANK"]=QString::number(rank);

    if(!db.addData("RANKING", addHash)) {
            qDebug() << db.getErrorSql();
            return;
    }

    db.commit();
    db.closeSql();
}


void Rank::deleteRank(int id){
    sqliteDb db;
    db.setDbDir(QDir::currentPath());
    if(!db.reOpenSql("test")) {
        qDebug() <<  "打开数据库失败";
        return;
    }
    db.transaction();//开启事务
    QString sqlWhere = QString(" where ID='%1'").arg(id);
    if(!db.delData(QString("RANKING"),  sqlWhere)) {
        qDebug() << db.getErrorSql();
        return;
    }

    db.commit();
    db.closeSql();
}
/*
void Rank::qweryRank(int id){
    sqliteDb db;
    db.setDbDir(QDir::currentPath());
    if(!db.reOpenSql("test")) {
        qDebug() <<  "打开数据库失败";
        return;
    }
    db.transaction();//开启事务
    QList<QHash<QString,QString>> data;
        QHash<QString,QString> getHash;
        if(!db.getData("RANKING", data))
        {
            qDebug() << db.getErrorSql();
            return;
        }
    db.closeSql();
}*/
QList<QHash<QString,QString>> Rank::wholeRank(){
    sqliteDb db;
    db.setDbDir(QDir::currentPath());
    if(!db.reOpenSql("test")) {
        qDebug() <<  "打开数据库失败";
    }
    db.transaction();//开启事务
    QList<QHash<QString,QString>> data;
    QHash<QString,QString> getHash;
    if(!db.getData("RANKING", data)) {
        qDebug() << db.getErrorSql();
    }
    return data;
}
