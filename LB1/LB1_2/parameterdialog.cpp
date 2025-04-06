#include "parameterdialog.h"
ParameterDialog::ParameterDialog(const QStringList &labels, const QList<double> &initialValues, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Просмотреть/Изменить");

    QVBoxLayout *layout = new QVBoxLayout(this);

    for (int i = 0; i < labels.size(); ++i) {
        QLabel *label = new QLabel(labels[i], this);
        QLineEdit *input = new QLineEdit(this);
        input->setText(QString::number(initialValues[i]));
        inputs.append(input);

        layout->addWidget(label);
        layout->addWidget(input);
    }

    QPushButton *okButton = new QPushButton("OK", this);
    layout->addWidget(okButton);

    connect(okButton, &QPushButton::clicked, this, &ParameterDialog::accept);
}


QList<double> ParameterDialog::values() const {
    QList<double> result;
    for (auto *input: inputs) {
        result.append(input->text().toDouble());
    }
    return result;
}

ParameterDialog::ParameterDialog(const QStringList &labels, const QList<QPoint> &initialVertices, QWidget *parent)
    : QDialog(parent), vertices(initialVertices) {
    setWindowTitle("Просмотреть/Изменить");

    QVBoxLayout *layout = new QVBoxLayout(this);

    pointSelector = new QComboBox(this);
    for (int i = 0; i < initialVertices.size(); ++i) {
        pointSelector->addItem(QString("Точка %1").arg(i + 1));
    }
    layout->addWidget(pointSelector);

    for (int i = 0; i < labels.size(); ++i) {
        QLabel *label = new QLabel(labels[i], this);
        QLineEdit *input = new QLineEdit(this);
        input->setText(QString::number(initialVertices[i].x())); // Инициализируем x
        inputs.append(input);
        layout->addWidget(label);
        layout->addWidget(input);
    }

    QPushButton *okButton = new QPushButton("OK", this);
    layout->addWidget(okButton);

    connect(okButton, &QPushButton::clicked, this, &ParameterDialog::accept);
    connect(pointSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ParameterDialog::updateCoordinates);

    updateCoordinates(0); // Инициализация координат первой точки
}

void ParameterDialog::updateCoordinates(int index) {
    if (index < vertices.size()) {
        inputs[0]->setText(QString::number(vertices[index].x())); // Установка x
        inputs[1]->setText(QString::number(vertices[index].y())); // Установка y
    }
}
