#include "date.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <QApplication>

Date::Date(): days_(0), months_(0), years_(0), numberOfDayInYear_(-1) {}

Date::Date(int day, int month, int year): days_(day), months_(month), years_(year){
    numberOfDayInYear_ = days_;
    for(int i = 1; i < months_; i++){
        numberOfDayInYear_ += daysInMonth(i, year);
    }
}

bool Date::operator==(const Date& other) const {
    return other.days_ == days_ && other.months_ == months_ && other.years_ == years_;
}

bool Date::operator>(const Date& other) const {
    if (years_ > other.years_) {
        return true;
    } else if (years_ < other.years_) {
        return false;
    } else if (months_ > other.months_) {
        return true;
    } else if (months_ < other.months_) {
        return false;
    } else {
        return days_ > other.days_;
    }
}

bool Date::operator<(const Date& other) const {
    if (years_ < other.years_) {
        return true;
    } else if (years_ > other.years_) {
        return false;
    } else if (months_ < other.months_) {
        return true;
    } else if (months_ > other.months_) {
        return false;
    } else {
        return days_ < other.days_;
    }
}

bool Date::isLeap(){
    return years_ % 4 == 0 && (years_ % 400 == 0 || years_ % 100 != 0);
}

bool Date::isLeap(int year){
    return year % 4 == 0 && (year % 400 == 0 || year % 100 != 0);
}

int Date::daysInMonth(int month, int year) {
    switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
    case 4: case 6: case 9: case 11: return 30;
    case 2: return isLeap(year) ? 29 : 28;
    default: return 0;
    }
}

Date Date::NextDay(){
    Date nextDate = *this;
    nextDate.days_++;
    if(nextDate.days_ > daysInMonth(months_, years_)){
        nextDate.days_ = 1;
        nextDate.months_++;
        if(nextDate.months_ == 13){
            nextDate.months_ = 1;
            nextDate.years_++;
            if(nextDate.years_ == 0){
                nextDate.years_++;
            }
        }
    }
    return nextDate;
}

Date Date::PreviousDay(){
    Date previousDate = *this;
    previousDate.days_--;
    if(previousDate.days_ <= 0){
        previousDate.months_--;
        if(previousDate.months_ <= 0){
            previousDate.months_ = 12;
            previousDate.years_--;
            if(previousDate.years_ == 0){
                previousDate.years_--;
            }
        }
        previousDate.days_ = daysInMonth(previousDate.months_, previousDate.years_);
    }

    return previousDate;
}

short Date::WeekNumber(){
    short dayOfWeek;
    short countOfDays = 0;
    for(int i = 0; i < months_; i++){
        countOfDays += daysInMonth(i, years_);
    }
    countOfDays += days_;
    dayOfWeek = DayOfWeek();
    if(dayOfWeek == 0)
        return ceil(countOfDays / 7.0);
    else{
        countOfDays -= dayOfWeek;
        return ceil(countOfDays / 7.0) + 1;
    }
}

short Date::DayOfWeek() {
    if (months_ < 3) {
        months_ += 12;
        years_--;
    }

    int k = years_ % 100;
    int j = years_ / 100;

    // Формула Зеллера
    int dayOfWeek = (days_ + (13 * (months_ + 1)) / 5 + k + (k / 4) + (j / 4) - (2 * j)) % 7;

    dayOfWeek = (dayOfWeek + 7) % 7;

    return (dayOfWeek + 6) % 7 ;
}

QVector<Date> Date::readDatesFromFile(const std::string& filename) {
    QVector<Date> dates;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int day, month, year;
        char dot;
        iss >> day >> dot >> month >> dot >> year;
        dates.emplace_back(day, month, year);
    }
    return dates;
}

void Date::writeDatesToFile(const std::string& filename, const std::vector<Date>& dates) {
    std::ofstream file(filename, std::ios::app);
    for (const auto& date : dates) {
        file << date.days_ << "." << date.months_ << "." << date.years_ << "\n";
    }
}

int Date::getDay() const {
    return days_;
}

int Date::getMonth() const {
    return months_;
}

int Date::getYear() const {
    return years_;
}

int Date::DaysTillYourBithday(Date birthdayDate) {
    int currentYear = getYear();
    int birthdayDay = birthdayDate.getDay();
    int birthdayMonth = birthdayDate.getMonth();

    // Проверяем, является ли текущий год невисокосным, если день рождения - 29 февраля.
    if (birthdayDay == 29 && birthdayMonth == 2 && !isLeap(currentYear)) {
        birthdayDay = 1;
        birthdayMonth = 3; // Считаем как 1 марта
    }

    Date birthdayThisYear(birthdayDay, birthdayMonth, currentYear);

    // Проверяем, если текущая дата совпадает с днем рождения
    if (*this > birthdayThisYear) {
        qDebug() << this->days_ << '.' << this->months_ << '.' << this->years_;
        qDebug() << birthdayDate.days_ << '.' << birthdayDate.months_ << '.' << birthdayDate.years_;
        birthdayThisYear = Date(birthdayDay, birthdayMonth, currentYear + 1);
    }


    return Duration(birthdayThisYear);
}

int Date::Duration(Date date) {
    // Получаем текущую дату
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm *local_time = std::localtime(&now_c);

    int currentDay = local_time->tm_mday;
    int currentMonth = local_time->tm_mon + 1; // Месяцы начинаются с 0
    int currentYear = local_time->tm_year + 1900;
    Date currentDate(currentDay, currentMonth, currentYear);

    if (*this == date) {
        return 0;
    }

    // Находим разницу в днях между текущей датой и каждой из данных дат
    int daysFromCurrentToThis = this->getDaysFromCurrent(currentDate);
    int daysFromCurrentToDate = date.getDaysFromCurrent(currentDate);

    // Вычисляем разницу между двумя датами, основываясь на найденных значениях
    return -daysFromCurrentToDate + daysFromCurrentToThis;
}

int Date::getDaysFromCurrent(Date date) {

    if (*this == date) {
        return 0;
    }

    // Установим начальную и конечную даты
    Date startDate = *this < date ? *this : date;
    Date endDate = *this < date ? date : *this;

    int duration = 0;
    if(this->years_ != date.years_){
        // Считаем дни до конца первого года
        duration += (isLeap(startDate.years_) ? 366 : 365) - startDate.numberOfDayInYear_;

        // Считаем дни между полными годами
        for (int year = startDate.years_ + 1; year < endDate.years_; ++year) {
            duration += (isLeap(year) ? 366 : 365);
        }

        // Считаем дни в последнем году
        duration += endDate.numberOfDayInYear_;
    }else{
        duration = endDate.getNumberOfDayInYear() - startDate.getNumberOfDayInYear();
    }
    // Если текущая дата больше, возвращаем отрицательные дни
    return *this < date ? duration : -duration;
}

int Date::getNumberOfDayInYear(){
    return numberOfDayInYear_;
}


bool Date::isCorrectDate(int day, int month, int year){
    if(day > daysInMonth(month,year) || month > 12 || month <= 0){
        throw false;
    }else {
        return true;
    }
}
