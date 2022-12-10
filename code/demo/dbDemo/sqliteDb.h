#ifndef SQLITEDB_H
#define SQLITEDB_H


#include <QDir>
#include <QDate>
#include <QDateTime>
#include <QFileInfo>
#include <QString>
#include <QTime>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QVariant>

class sqliteDb
{
public:
    sqliteDb();
    ~sqliteDb();
public:
    bool setDbDir(QString dirPath);//设置数据库存放路径
    bool creatDbFile(QString dbName);//生成一个db文件
    bool reOpenSql(QString dbName);//打开连接
    bool closeSql();//关闭连接
    bool queryExec(QString dbName,QString sqlStr);//执行sql语句，不获取结果
    bool queryExec(QString dbName,QString sqlStr,QList<QHash<QString,QString>> &data);//执行sql语句，并获取结果
    bool getData(QString dbName,QString tableName,QHash<QString,QString> &data,QString sqlWhere=""); //获取数据
    bool getData(QString dbName,QString table,QList<QHash<QString,QString>> &data,QString sqlWhere=""); //获取数据
    bool getData(QString dbName,QString tableName,QHash<QString,QString> columndata,QList<QHash<QString,QString>> &data,QString sqlWhere=""); //获取数据
    bool addData(QString dbName,QString tableName,QHash<QString,QString> data);//增加
    bool delData(QString dbName,QString tableName,QString sqlWhere);//删除
    bool updateData(QString dbName,QString tableName,QHash<QString,QString> data,QString sqlWhere="");//修改

    bool queryExec(QString sqlStr);//执行sql语句，不获取结果
    bool queryExec(QString sqlStr,QList<QHash<QString,QString>> &data);//执行sql语句，并获取结果
    bool getData(QString tableName,QHash<QString,QString> &data,QString sqlWhere=""); //获取数据
    bool getData(QString table,QList<QHash<QString,QString>> &data,QString sqlWhere=""); //获取数据
    bool getData(QString tableName,QHash<QString,QString> columndata,QList<QHash<QString,QString>> &data,QString sqlWhere=""); //获取数据
    bool addData(QString tableName,QHash<QString,QString> data);//增加
    bool delData(QString tableName,QString sqlWhere);//删除
    bool updateData(QString tableName,QHash<QString,QString> data,QString sqlWhere="");//修改
    bool transaction();
    bool commit();
    QString getErrorSql();//获取错误的数据库语句
private:
    QSqlDatabase db;
    QString dbFilePath;//数据库路径
    QString dbDir; //数据库文件夹
    QString databaseName;//数据库名字
    QString errorSqlText;//错误语句
private:
    void errorSql(QString sql);//错误打印
};

#endif // SQLITEDB_H


