#ifndef DATE_H
#define DATE_H
#include <vector>
#include <string>
#include <QVector>
class Date
{
    int days_, months_, years_;
    int numberOfDayInYear_;
    bool isLeap_ = false;
    short DayOfWeek();

    bool operator>(const Date& other)const;
    bool operator<(const Date& other)const;
    bool operator==(const Date& other) const;
    int getDaysFromCurrent(Date date);
public:
    Date();
    Date(int day, int month, int year);
    static bool isLeap(int year);
    short WeekNumber();
    int DaysTillYourBithday(Date birthdayDate);
    int Duration(Date date);
    bool isLeap();
    Date NextDay();
    Date PreviousDay();

    static int daysInMonth(int month, int year);
    static QVector<Date> readDatesFromFile(const std::string& filename);
    static void writeDatesToFile(const std::string& filename, const std::vector<Date>& dates);
    static bool isCorrectDate(int day, int month, int year);

    int getDay() const;
    int getMonth() const;
    int getYear() const;
    int getNumberOfDayInYear();

};

#endif // DATE_H
