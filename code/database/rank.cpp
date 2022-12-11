#include "rank.h"

#include <algorithm>
#include <cmath>
Rank* Rank::rank = NULL;
void Rank::Insert(std::string id, int score) {
    sqliteDb db;
    db.setDbDir("../2022-BJTU-DataStructure-FinalProject/resource/db");
    if (!db.reOpenSql("Rank")) {
        db.creatDbFile("Rank");
        if (!db.reOpenSql("Rank")) {
            qDebug() << "打开数据库失败";
            return;
        }
        // 新建表
        if (!db.queryExec("Rank",
                          "CREATE TABLE RANK("
                          "   ID TEXT PRIMARY KEY     NOT NULL,"
                          "   SCORE            INT     NOT NULL"
                          ");")) {
            qDebug() << db.getErrorSql();
            return;
        }
    }

    db.transaction();  // 开启事务

    // 增加记录
    QHash<QString, QString> addHash;
    addHash["ID"] = QString::fromStdString(id);
    addHash["SCORE"] = QString::number(score);
    if (!db.addData("RANK", addHash)) {
        QString sqlWhere = QString(" where ID='%1'").arg(addHash["ID"]);
        if (!db.updateData("RANK", addHash, sqlWhere)) {
            qDebug() << db.getErrorSql();
            return;
        }
    }
    db.commit();
    db.closeSql();
    return;
}

void Rank::Query(std::vector<std::pair<std::string, int>>& pairs) {
    // 返回前十，SELECT * from RANK ORDER BY SCORE DESC
    sqliteDb db;
    db.setDbDir("../2022-BJTU-DataStructure-FinalProject/resource/db");
    if (!db.reOpenSql("Rank")) {
        qDebug() << "打开数据库失败";
        for (auto pair : pairs) {
            pair = {"empty", -1};
        }
        db.commit();
        db.closeSql();
        return;
    }

    db.transaction();  // 开启事务
    QString sqlStr = QString("SELECT * FROM RANK ORDER BY SCORE DESC");
    QList<QHash<QString, QString>> allData;
    if (!db.queryExec("Rank", sqlStr, allData)) {
        qDebug() << "分数降序获取失败";
        for (auto pair : pairs) {
            pair = {"empty", -1};
        }
        db.commit();
        db.closeSql();
        return;
    }

    while (!allData.isEmpty()) {
        QString id = QString("%1").arg(allData.first()["ID"]);
        pairs.push_back({id.toStdString(), allData.takeFirst()["SCORE"].toInt()});
    }
    sort(pairs.begin(), pairs.end(),
         [&](std::pair<std::string, int> a, std::pair<std::string, int> b) -> bool { return a.second > b.second; });
    db.commit();
    db.closeSql();
    return;
}

int Rank::Query(std::string id) {
    sqliteDb db;
    db.setDbDir("../2022-BJTU-DataStructure-FinalProject/resource/db");
    if (!db.reOpenSql("Rank")) {
        qDebug() << "打开数据库失败";
        return -1;
    }

    db.transaction();  // 开启事务
    QHash<QString, QString> getHash;
    QString sqlWhere = QString(" where ID='%1'").arg(QString::fromStdString(id));
    if (!db.getData("RANK", getHash, sqlWhere)) {
        qDebug() << db.getErrorSql();
        return -1;
    }
    db.commit();
    db.closeSql();
    return getHash["SCORE"].toInt();
}
