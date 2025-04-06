#ifndef PARAMETERDIALOG_H
#define PARAMETERDIALOG_H
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
class ParameterDialog : public QDialog {
    Q_OBJECT

public:
    ParameterDialog(const QStringList &labels, const QList<double> &initialValues, QWidget *parent = nullptr);
    ParameterDialog(const QStringList &labels, const QList<QPoint> &initialVertices, QWidget *parent = nullptr);
    QList<double> values() const;
    QComboBox *pointSelector;

private slots:
    void updateCoordinates(int index);

private:
    QList<QLineEdit*> inputs;
    QList<QPoint> vertices;
};

#endif // PARAMETERDIALOG_H
