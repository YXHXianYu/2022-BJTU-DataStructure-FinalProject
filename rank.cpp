#include "rank.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QPalette>
Ranking::Ranking(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pal;
    QPixmap pixmap(":/image/images/background.png");
    pal.setBrush(QPalette::Background, QBrush(pixmap));;
    setPalette(pal);

    QString string[5];

    Return = new QPushButton("Return");
    Return->setFont(QFont("Algerian",16));
    Return->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label1 = new QLabel(this);
    Label1->setText("Ranking    Easy1   Easy2   Hard1   Hard2");
    Label1->setFont(QFont("Algerian",16));
    Label1->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label2 = new QLabel(this);
    Label2->setFont(QFont("Algerian",16));
    Label2->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label3 = new QLabel(this);
    Label3->setFont(QFont("Algerian",16));
    Label3->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label4 = new QLabel(this);
    Label4->setFont(QFont("Algerian",16));
    Label4->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label5 = new QLabel(this);
    Label5->setFont(QFont("Algerian",16));
    Label5->setStyleSheet("QLabel{background:transparent;color:white;}");

    Label6 = new QLabel(this);
    Label6->setFont(QFont("Algerian",16));
    Label6->setStyleSheet("QLabel{background:transparent;color:white;}");

    if(QSqlDatabase::contains("qt_sql_default_connection"))
        database = QSqlDatabase::database("qt_sql_default_connection");
    else {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("Rank.db");
    }
    if (!database.open())
        qDebug() << "Error: Failed to connect database." << database.lastError();
    else {
        QSqlQuery sql_query(database);
        QString create_sql = "create table Score (Ranking int primary key, Easy1 int, Easy2 int, Hard1 int, Hard2 int)";
        sql_query.prepare(create_sql);
        if(!sql_query.exec())
            qDebug() << "Error: Fail to create table." << sql_query.lastError();
        else
            qDebug() << "Table created!";
        sql_query.exec("INSERT INTO Score VALUES(1, 0, 0, 0, 0)");
        sql_query.exec("INSERT INTO Score VALUES(2, 0, 0, 0, 0)");
        sql_query.exec("INSERT INTO Score VALUES(3, 0, 0, 0, 0)");
        sql_query.exec("INSERT INTO Score VALUES(4, 0, 0, 0, 0)");
        sql_query.exec("INSERT INTO Score VALUES(5, 0, 0, 0, 0)");
        QString select_all_sql = "select * from Score";
        sql_query.prepare(select_all_sql);
        if(!sql_query.exec()) {
            qDebug()<<sql_query.lastError();
        }
        else {
            for(int i=0;sql_query.next();i++) {
                int Rank = sql_query.value(0).toInt();
                int Easy1 = sql_query.value(1).toInt();
                int Easy2 = sql_query.value(2).toInt();
                int Hard1 = sql_query.value(3).toInt();
                int Hard2 = sql_query.value(4).toInt();
                string[i]=QString("%1           %2      %3      %4      %5").arg(Rank).arg(Easy1).arg(Easy2).arg(Hard1).arg(Hard2);
            }
        }
        Label2->setText(string[0]);
        Label3->setText(string[1]);
        Label4->setText(string[2]);
        Label5->setText(string[3]);
        Label6->setText(string[4]);
        QVBoxLayout *lay = new QVBoxLayout;
        lay->addWidget(Label1);
        lay->addWidget(Label2);
        lay->addWidget(Label3);
        lay->addWidget(Label4);
        lay->addWidget(Label5);
        lay->addWidget(Label6);
        lay->addWidget(Return);

        setLayout(lay);
    }

    connect(Return,&QPushButton::clicked,this,&Ranking::ReturnClicked);
}


Ranking::~Ranking() {
}

void Ranking::updateRanking(QString s, int Score) {
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        database = QSqlDatabase::database("qt_sql_default_connection");
    else {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("Rank.db");
    }
    if (!database.open())
        qDebug() << "Error: Failed to connect database." << database.lastError();
    else {
        QSqlQuery sql_query(database);	//建表
        if(s=="Easy1") {
            int rank;
            int scores[5];
            for(int i=0;i<5;i++)
                scores[i]=0;
            QString select_sql = "select Ranking, Easy1 from Score";//查询表中数据
            if(!sql_query.exec(select_sql)) {
                qDebug()<<sql_query.lastError();
            }
            else {
                while(sql_query.next()){
                    rank = sql_query.value(0).toInt();
                    scores[rank-1] = sql_query.value(1).toInt();
                }
            }
            for(int i=0;i<5;i++) {
                if(scores[i]>=Score)
                    continue;
                else {
                    for(int j=4;j>i;j--)
                        scores[j] = scores[j-1];
                    scores[i] = Score;
                    break;
                }
            }
            for(int i=0;i<5;i++) {
                QString update_sql = "update Score set Easy1 = :Easy1 where Ranking = :Ranking";	//将数据插入表中
                sql_query.prepare(update_sql);
                sql_query.bindValue(":Easy1", scores[i]);
                sql_query.bindValue(":Ranking", i+1);
                if(!sql_query.exec()) {
                    qDebug() << sql_query.lastError();
                }
                else {
                    qDebug() << "updated!";
                }
            }
        }
    }
}
