#ifndef TIPLABEL_H
#define TIPLABEL_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QLabel>
class TipLabel : public QWidget
{
    Q_OBJECT
public:
    explicit TipLabel(QWidget *parent = nullptr);
    QLabel *m_TipLb;
signals:

};

#endif // TIPLABEL_H
