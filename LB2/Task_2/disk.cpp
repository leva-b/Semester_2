#include "disk.h"
#include <QVector>
#include <QFile>
#include <QMessageBox>

Disk::Disk() {}

Disk::Disk(const QString type, const QString name, const QString author, const float price, const QString remark):
    _type(type), _name(name), _author(author), _price(price), _remark(remark){}

bool Disk::compare_by_name(const Disk& a, const Disk& b){
    return a._name.toLower() < b._name.toLower();
}

bool Disk::compare_by_author(const Disk& a, const Disk& b){
    return a._author.toLower() < b._author.toLower();
}

QVector<Disk> Disk::readDisksFromFile(const QString& filePath){
    QVector<Disk> diskArray;
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Cannot open file for reading: " + file.errorString().toStdString());
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString type = in.readLine();
        QString name = in.readLine();
        QString author = in.readLine();
        QString priceLine = in.readLine();
        QString remark = in.readLine();
        if (type.isEmpty() || name.isEmpty() || author.isEmpty() || priceLine.isEmpty() || remark.isEmpty()) {
            continue;
        }
        bool ok;
        float price = priceLine.toFloat(&ok);
        if(!ok){
            price = 0;
        }
        diskArray.append(Disk(type, name, author, price, remark));
    }

    file.close();
    return diskArray;
}

QString Disk::GetName()const{return _name;}

QString Disk::GetType()const {return _type;}

QString Disk::GetAuthor()const {return _author;}

float Disk::GetPrice()const {return _price;}

QString Disk::GetRemark()const{return _remark;}
