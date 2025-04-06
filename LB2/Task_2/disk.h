#ifndef DISK_H
#define DISK_H
#include <QString>


using std::string;

class Disk
{
    QString _type;
    QString _name;
    QString _author;
    float _price;
    QString _remark;
public:
    enum types {
        film,
        video,
        game,
        music,
        application,
        software
    };

    QString GetName() const;
    QString GetType() const;
    QString GetAuthor() const;
    float GetPrice() const;
    QString GetRemark() const;
    Disk();
    Disk(const QString type, const QString name, const QString author, const float price, const QString remark);
    static bool compare_by_name(const Disk& a, const Disk& b);
    static bool compare_by_author(const Disk& a, const Disk& b);
    static QVector<Disk> readDisksFromFile(const QString& filePath);

};

#endif // DISK_H
