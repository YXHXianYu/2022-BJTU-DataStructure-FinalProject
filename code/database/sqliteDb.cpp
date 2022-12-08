#include "sqliteDb.h"

sqliteDb::sqliteDb()
{
    dbDir = QDir::currentPath() ;
}
sqliteDb::~sqliteDb()
{

}
//设置数据库存放路径
bool sqliteDb::setDbDir(QString dirPath)
{
    QDir dir(dirPath);
    if(dir.exists())
    {
        dbDir = dirPath;
        return true;
    }
    else
    {
        return false;
    }
}
//打开连接
bool sqliteDb::reOpenSql(QString dbName)
{
    QString fileName = (dbDir + "/"+dbName + ".db");
    if(!QFile::exists(fileName))
    {
        return false;//数据库不存在
    }
    QFileInfo file(fileName);
    if(file.suffix() != "db")
        return false;
    db = QSqlDatabase::database(dbName);
    if(!db.isValid())
    {
        db = QSqlDatabase::addDatabase("QSQLITE",dbName);
        db.setDatabaseName(fileName);
        if (!db.open())
        {
            return false;//打开失败
        }
    }
    dbFilePath = fileName;
    databaseName = dbName;//数据库名字
    return true;
}
//关闭连接
bool sqliteDb::closeSql()
{
    if(databaseName.isEmpty())
        return true;
    if(!QFile::exists(dbFilePath))
    {
        return false;//数据库不存在
    }
    db = QSqlDatabase::database(databaseName);
    if(!db.isValid())
    {
        return true;
    }
    db.close();
    db = QSqlDatabase::database();
    QSqlDatabase::removeDatabase(databaseName);
    databaseName = "";
    dbFilePath = "";
    return true;
}
//生成一个db文件
bool sqliteDb::creatDbFile(QString dbName)
{
    QString fileName = (dbDir + "/"+dbName + ".db");
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        file.close();
    }
    else
    {
        return false;
    }
    return true;
}
//错误打印
void sqliteDb::errorSql(QString sql)
{
    errorSqlText = sql;
    //QString("数据库执行错误：%1 ")+sql.toUtf8().constData();
}
//获取错误的数据库语句
QString sqliteDb::getErrorSql()
{
    if(databaseName.isEmpty())
    {
        return "db  not setting";//数据库未设置
    }
    return errorSqlText;
}
//执行sql语句，不获取结果
bool sqliteDb::queryExec(QString dbName,QString queryStr)
{
    if(databaseName.isEmpty())
    {
        if(!reOpenSql(dbName))
        {
            return false;
        }
    }
    QSqlQuery query(QSqlDatabase::database(dbName, true));
    if(!query.exec(queryStr))
    {
        errorSql(queryStr);
        return false;
    }
    return  true;
}
//执行sql语句，并获取结果
bool sqliteDb::queryExec(QString dbName,QString queryStr,QList<QHash<QString,QString>> &data)
{
    data.clear();
    if(databaseName.isEmpty())
    {
        if(!reOpenSql(dbName))
        {
            return false;
        }
    }
    QSqlQuery query(QSqlDatabase::database(dbName, true));
    if(!query.exec(queryStr))
    {
        errorSql(queryStr);
        return false;
    }
    QSqlRecord rec = query.record();
    while(query.next())
    {
        QHash<QString,QString> rowData;
        for(int i =0;i<rec.count();i++)
        {
            QVariant::Type ty =  query.value(i).type();
            if( QVariant::Type::Date == ty)
            {
                QDate temp = query.value(i).toDate();
                rowData[rec.fieldName(i)]=temp.toString("yyyy-MM-dd");
            }
            else if( QVariant::Type::Time == ty)
            {
                QTime temp = query.value(i).toTime();
                rowData[rec.fieldName(i)]=temp.toString("hh:mm:ss");
            }
            else if( QVariant::Type::DateTime == ty)
            {
                QDateTime temp = query.value(i).toDateTime();
                rowData[rec.fieldName(i)]=temp.toString("yyyy-MM-dd hh:mm:ss");
            }
            else
                rowData[rec.fieldName(i)]=query.value(i).toString();
        }
        data.append(rowData);
    }
    return  true;
}
//获取数据
bool sqliteDb::getData(QString dbName,QString tableName,QHash<QString,QString> &data,QString sqlWhere)
{
    data.clear();
    QList<QHash<QString,QString>> dataList;
    if(!getData(dbName,tableName,dataList,sqlWhere))
    {
        return false;
    }
    if(dataList.count() > 0)
    {
        data = dataList[0];
    }
    return true;
}
//获取数据
bool sqliteDb::getData(QString dbName,QString tableName,QList<QHash<QString,QString>> &data,QString sqlWhere)
{
    QString queryStr="select * from "+tableName;
    if(!sqlWhere.isEmpty())
        queryStr+=" "+sqlWhere;
    if(!queryExec(dbName,queryStr,data))
    {
        return  false;
    }
    return  true;
}
//获取数据
bool sqliteDb::getData(QString dbName,QString tableName,QHash<QString,QString> columndata,QList<QHash<QString,QString>> &data,QString sqlWhere)
{
    QString colunmStr;
    if(columndata.count() == 0)
        colunmStr = "*";
    else
    {
        QStringList keys = columndata.keys();
        for(auto key : keys)
        {
            QString column = QString("%1 AS `%2`").arg(key).arg(columndata[key]);
            if(!colunmStr.isEmpty())
                colunmStr += ",";
            colunmStr += column;
        }
    }
    QString queryStr = QString("SELECT %1 FROM %2 %3").arg(colunmStr).arg(tableName).arg( sqlWhere);
    if(!queryExec(dbName,queryStr,data))
    {
        return  false;
    }
    return  true;
}
//增加
bool sqliteDb::addData(QString dbName,QString tableName,QHash<QString,QString> data)
{
    if(data.isEmpty())
        return false;
    QString queryStr="insert into "+tableName+" ";
    QString fieldStr="(",valueStr="values(";
    QHash<QString,QString>::iterator it;
    for ( it = data.begin(); it != data.end(); ++it )
    {
        fieldStr+=it.key()+",";
        valueStr+="'"+it.value()+"',";
    }
    fieldStr=fieldStr.left(fieldStr.length()-1);
    valueStr=valueStr.left(valueStr.length()-1);
    fieldStr+=")";
    valueStr+=")";
    queryStr+=fieldStr+" "+valueStr;
    if(!queryExec(dbName,queryStr))
    {
        return false;
    }
    return true;
}
//删除
bool sqliteDb::delData(QString dbName, QString tableName, QString sqlWhere)
{
    QString queryStr="delete from "+tableName;
    if(!sqlWhere.isEmpty())
        queryStr+=" "+sqlWhere;
    if(!queryExec(dbName,queryStr))
    {
        return false;
    }
    return true;
}
//修改
bool sqliteDb::updateData(QString dbName,QString tableName,QHash<QString,QString> data,QString sqlWhere)
{
    QString queryStr="update "+tableName+" ";
    QHash<QString,QString>::iterator it;
    QString setStr="set ";
    for ( it = data.begin(); it != data.end(); ++it )
    {
        setStr+=it.key()+"='"+it.value()+"'";
        setStr+=",";
    }
    setStr=setStr.left(setStr.length()-1);
    queryStr+=setStr;
    if(!sqlWhere.isEmpty())
        queryStr+=" "+sqlWhere;
    if(!queryExec(dbName,queryStr))
    {
        return false;
    }
    return true;
}
bool sqliteDb::transaction()
{
    if(databaseName.isEmpty())
        return false;
    return  db.transaction();
}
bool sqliteDb::commit()
{
    if(databaseName.isEmpty())
        return false;
    return  db.commit();
}
//执行sql语句，不获取结果
bool sqliteDb::queryExec(QString sqlStr)
{
    if(databaseName.isEmpty())
        return false;
    return  queryExec(databaseName,sqlStr);
}
//执行sql语句，并获取结果
bool sqliteDb::queryExec(QString sqlStr,QList<QHash<QString,QString>> &data)
{
    if(databaseName.isEmpty())
        return false;
    return  queryExec(databaseName,sqlStr,data);
}
//获取数据
bool sqliteDb::getData(QString tableName,QHash<QString,QString> &data,QString sqlWhere)
{
    if(databaseName.isEmpty())
        return false;
    return  getData(databaseName,tableName,data,sqlWhere);
}
//获取数据
bool sqliteDb::getData(QString table,QList<QHash<QString,QString>> &data,QString sqlWhere)
{
    if(databaseName.isEmpty())
        return false;
    return  getData(databaseName,table,data,sqlWhere);
}
//获取数据
bool sqliteDb::getData(QString tableName,QHash<QString,QString> columndata,QList<QHash<QString,QString>> &data,QString sqlWhere)
{
    if(databaseName.isEmpty())
        return false;
    return  getData(databaseName,tableName,columndata,data,sqlWhere);
}
//增加
bool sqliteDb::addData(QString tableName,QHash<QString,QString> data)
{
    if(databaseName.isEmpty())
        return false;
    return  addData(databaseName,tableName,data);
}
//删除
bool sqliteDb::delData(QString tableName,QString sqlWhere)
{
    if(databaseName.isEmpty())
        return false;
    return  delData(databaseName,tableName,sqlWhere);
}
//修改
bool sqliteDb::updateData(QString tableName,QHash<QString,QString> data,QString sqlWhere)
{
    if(databaseName.isEmpty())
        return false;
    return  updateData(databaseName,tableName,data,sqlWhere);
}
//

