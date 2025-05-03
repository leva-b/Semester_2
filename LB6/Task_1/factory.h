#ifndef FACTORY_H
#define FACTORY_H
#include <QChar>
struct Factory {

    int workshopCount;       // Количество цехов (целое число)
    double productionVolume; // Объем производства (дробное число)
    QChar category;           // Категория(A, B, C...)
    bool isOperating; // Логическое поле (работает ли завод)
    static const int MAX_NAME_SIZE = 16;
    static const int PERFORMANCE_STATISTICS_SIZE = 5;
    QChar factoryName[MAX_NAME_SIZE];

    // Статический числовой массив (производственные показатели за 5 дней)
    double dailyProduction[PERFORMANCE_STATISTICS_SIZE];   // Данные за 5 дней
};

#endif // FACTORY_H
