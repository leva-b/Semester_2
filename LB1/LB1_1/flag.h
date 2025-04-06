#ifndef FLAG_H
#define FLAG_H
#include <QWidget>

class Flag: public QWidget
{
public:
    Flag();
protected:
    void paintEvent(QPaintEvent* );
    void raisingLoweringFlag();
};

#endif // FLAG_H
