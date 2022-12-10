#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "sqliteDb.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    sqliteDb db;
    db.setDbDir(QDir::currentPath());
    if(!db.reOpenSql("test"))
    {
        db.creatDbFile("test");
        if(!db.reOpenSql("test"))
        {
            qDebug() <<  "打开数据库失败";
            return;
        }
        //新建表
        if(!db.queryExec("test","CREATE TABLE RANK("
                                  "   ID INT PRIMARY KEY     NOT NULL,"
                                  "   NAME           TEXT    NOT NULL,"
                                  "   SCORE            INT     NOT NULL"
                                  ");"))
        {
            qDebug() << db.getErrorSql();
            return;
        }
    }
    db.transaction();//开启事务

    //增加记录
    QHash<QString,QString> addHash;
    addHash["ID"] = QString::number(1);
    addHash["NAME"] = "yxh01";
    addHash["SCORE"] = "100";

    if(!db.addData("RANK", addHash))
    {
        qDebug() << db.getErrorSql();
        return;
    }
    addHash["ID"] = QString::number(2);
    addHash["NAME"] = "yxh02";
    addHash["SCORE"] = "99";
    if(!db.addData("RANK", addHash))
    {
        qDebug() << db.getErrorSql();
        return;
    }
    addHash["ID"] = QString::number(3);
    addHash["NAME"] = "yxh03";
    addHash["SCORE"] = "98";
    if(!db.addData("RANK", addHash))
    {
        qDebug() << db.getErrorSql();
        return;
    }
    addHash["ID"] = QString::number(4);
    addHash["NAME"] = "yxh04";
    addHash["SCORE"] = "97";
    if(!db.addData("RANK", addHash))
    {
        qDebug() << db.getErrorSql();
        return;
    }

    //删除记录
    QString sqlWhere = QString(" where ID='%1'").arg( 4);
    if(!db.delData(QString("RANK"),  sqlWhere))
    {
        qDebug() << db.getErrorSql();
        return;
    }

    //查询
    QList<QHash<QString,QString>> data;
    QHash<QString,QString> getHash;
    if(!db.getData("RANK", data))
    {
        qDebug() << db.getErrorSql();
        return;
    }
    db.commit();//结束事务

    for(auto it:data)
    {
        qDebug() << QString("ID=%1,    NAME=%2,    SCORE=%3").arg(it["ID"]).arg(it["NAME"]).arg(it["SCORE"]);
    }

    db.closeSql();//关闭数据库
}

Widget::~Widget()
{
    delete ui;
}



