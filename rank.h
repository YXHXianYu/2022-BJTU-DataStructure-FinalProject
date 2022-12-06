#ifndef RANKING_H
#define RANKING_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QLabel>
#include <QPushButton>
class Ranking : public QWidget
{
    Q_OBJECT

public:
    QSqlDatabase database;
    QLabel* Label1;
    QLabel* Label2;
    QLabel* Label3;
    QLabel* Label4;
    QLabel* Label5;
    QLabel* Label6;
    QPushButton* Return;
    explicit Ranking(QWidget *parent = 0);
    void updateRanking(QString s,int Score);
    ~Ranking();
public slots:
    void ReturnClicked();
};

#endif // RANK_H

