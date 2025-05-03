#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIntValidator>
#include <QLibrary>
//#include <functional>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_stringLib("myString"), ui(new Ui::MainWindow)
{
    QLibrary("myString");

    ui->setupUi(this);
    if (!m_stringLib.load()) {
        qDebug() << "Error:" << m_stringLib.errorString();
        return;
    }

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    QVBoxLayout *textLayout = new QVBoxLayout();
    QVBoxLayout *buttonsLayout = new QVBoxLayout();

    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    str_1 = new QPlainTextEdit(this);
    str_1->setPlaceholderText("String 1");
    str_1->setStyleSheet("QPlainTextEdit {"
                         "   min-width: 200px;"
                         "   min-height: 20px;"
                         "   font-size: 23px;"
                         "}"
                         "QPlainTextEdit::placeholder {"
                         "   font-style: italic;"
                         "}");

    str_2 = new QPlainTextEdit(this);
    str_2->setPlaceholderText("String 2");
    str_2->setStyleSheet(str_1->styleSheet());

    log = new QPlainTextEdit(this);
    log->setStyleSheet("QPlainTextEdit {"
                       "   min-width: 200px;"
                       "   min-height: 20px;"
                       "   font-size: 23px;"
                       "}");
    log->setReadOnly(true);
    log->setPlaceholderText("Result");

    textLayout->addWidget(str_1);
    textLayout->addWidget(str_2);
    textLayout->addWidget(log);

    inputN = new QLineEdit(this);
    QIntValidator *validator = new QIntValidator(0, INT_MAX, this);
    inputN->setValidator(validator);
    inputN->setFixedSize(150,40);
    buttonsLayout->addWidget(inputN);
    inputN->setPlaceholderText("input n");
    initializedButton(buttonsLayout);

    inputChar = new QLineEdit(this);
    inputChar->setFixedSize(150, 40);
    inputChar->setPlaceholderText("Enter an int for memset");
    buttonsLayout->addWidget(inputChar);

    mainLayout->addLayout(textLayout, 3);
    mainLayout->addLayout(buttonsLayout, 1);
}

void MainWindow::initializedButton(QVBoxLayout* layout)
{
    layout->setSpacing(5);

    // Создаем кнопки и связываем их
    auto createButton = [this, layout](const QString& name) {
        QPushButton* btn = new QPushButton(name, this);
        btn->setFixedSize(150, 40);
        layout->addWidget(btn);

        connect(btn, &QPushButton::clicked, this, [this, name]() {
            handleStringFunction(name);
        });
    };

    createButton("strcpy");
    createButton("strncpy");
    createButton("strcat");
    createButton("strncat");
    createButton("memcmp");
    createButton("strcmp");
    createButton("strcoll");
    createButton("strncmp");
    createButton("strxfrm");
    createButton("strtok");
    createButton("memset");
    createButton("strlen");
}

void MainWindow::handleStringFunction(const QString &funcName){
    typedef void* (*StringFuncThreeArgsVoid)(void*, const void*, size_t);
    typedef char* (*StringFuncTwoArgsChar)(char*, const char*);
    typedef char* (*StringFuncThreeArgsChar)(char *, const char* , size_t);
    typedef int   (*ConstStringFuncTwo)(const char*, const char*);
    typedef int   (*ConstStringFuncThree)(const char*, const char*, size_t);
    typedef int   (*ConstVoidFuncThree)(const void*, const void*, size_t);
    typedef size_t (*StringFuncThreeArgsSize)(char *, const char* , size_t);
    typedef void* (*memset)(void*, int, size_t);

    QByteArray funcNameBytes = funcName.toUtf8();
    QByteArray byteArray2 = str_2->toPlainText().toUtf8();
    QByteArray byteArray1 = str_1->toPlainText().toUtf8();
    int n = inputN->text().toInt();
    qDebug() << n << "\n";
    int memsetN = (inputChar->text()).toInt();
    log->clear();
    if (funcName == "strcpy" || funcName == "strcat" || funcName == "strtok") {
        StringFuncTwoArgsChar func = (StringFuncTwoArgsChar)m_stringLib.resolve(funcNameBytes.constData());
        if (!func) {
            log->setPlainText(QString("Error: function %1 not found in the library!").arg(funcName));
            return;
        }

        size_t bufSize = byteArray2.size() + 1;
        if(funcName == "strcat"){
            bufSize += byteArray1.size();
        }
        std::unique_ptr<char[]> resultBuffer(new char[bufSize]);

        strncpy(resultBuffer.get(), byteArray1.constData(), byteArray1.size());
        if(funcName == "strtok"){
            log->setPlainText("Result:\n");
            char* pch = func(resultBuffer.get(), byteArray2.constData());

            while (pch != NULL)
            {
                log->appendPlainText(QString(pch) + QString("\n"));
                pch = func(NULL, byteArray2.constData());
            }
            return;
        }

        char* result = func(resultBuffer.get(), byteArray2.constData());
        log->setPlainText(QString("Result: %1").arg(result));
    }else if(funcName == "strncpy" || funcName == "strncat"){
        StringFuncThreeArgsChar func = (StringFuncThreeArgsChar)m_stringLib.resolve(funcNameBytes.constData());
        if (!func) {
            log->setPlainText(QString("Error: function %1 not found in the library!").arg(funcName));
            return;
        }

        size_t bufSize = byteArray2.size() + 1;
        if(funcName == "strcat"){
            bufSize += byteArray1.size();
        }
        std::unique_ptr<char[]> resultBuffer(new char[bufSize]);
        strncpy(resultBuffer.get(), byteArray1.constData(), byteArray1.size());

        log->appendPlainText(QString("Result: ") + QString(func(resultBuffer.get(), byteArray2.constData(), n)));
    }else if(funcName == "memcpy" || funcName == "memmove"){
        StringFuncThreeArgsVoid func = (StringFuncThreeArgsVoid)m_stringLib.resolve(funcNameBytes.constData());
        if (!func) {
            log->setPlainText(QString("Error: function %1 not found in the library!").arg(funcName));
            return;
        }
        size_t bufSize = byteArray2.size() + 1;
        if(funcName == "strcat"){
            bufSize += byteArray1.size();
        }
        std::unique_ptr<char[]> resultBuffer(new char[bufSize]);
        strncpy(resultBuffer.get(), byteArray1.constData(), bufSize);

        log->appendPlainText(QString("Result") + QString((char*)func(resultBuffer.get(), byteArray2.constData(), n)));
    }else if(funcName == "strcmp" || funcName == "strcoll"){
        ConstStringFuncTwo func = (ConstStringFuncTwo)m_stringLib.resolve("strcmp");
        if (!func) {
            log->setPlainText(QString("Error: function %1 not found in the library!").arg(funcName));
            return;
        }

        log->appendPlainText(QString("Result: ") + QString::number(func(byteArray1.constData(), byteArray2.constData())));
    }else if(funcName == "strncmp"){
        ConstStringFuncThree func = (ConstStringFuncThree)m_stringLib.resolve(funcNameBytes.constData());
        if (!func) {
            log->setPlainText(QString("Error: function %1 not found in the library!").arg(funcName));
            return;
        }

        log->appendPlainText(QString("Result: ") + QString::number(func(byteArray1.constData(), byteArray2.constData(), n)));
    }
    else if(funcName == "memcmp"){
        ConstVoidFuncThree func = (ConstVoidFuncThree)m_stringLib.resolve(funcNameBytes.constData());
        if (!func) {
            log->setPlainText(QString("Error: function %1 not found in the library!").arg(funcName));
            return;
        }

        log->appendPlainText(QString("Result: ") + QString::number(func(byteArray1.constData(), byteArray2.constData(), n)));
    }else if(funcName == "memset"){
        memset func = (memset)m_stringLib.resolve(funcNameBytes.constData());
        size_t bufSize = std::max(byteArray1.size(), byteArray2.size());
        std::unique_ptr<char[]> resultBuffer(new char[bufSize]);
        strncpy(resultBuffer.get(), byteArray1.constData(), bufSize);
        log->appendPlainText(QString("Result: ") + QString(static_cast<char*>(func(resultBuffer.get(), memsetN, n))));
    }else if(funcName == "strxfrm"){

        StringFuncThreeArgsSize func = (StringFuncThreeArgsSize)m_stringLib.resolve(funcNameBytes.constData());
        if (!func) {
            log->setPlainText(QString("Error: function %1 not found in the library!").arg(funcName));
            return;
        }
        size_t bufSize = byteArray1.size();
        std::unique_ptr<char[]> resultBuffer(new char[bufSize]);
        strncpy(resultBuffer.get(), byteArray1.constData(), bufSize);
        log->setPlainText(QString("Result: ") + QString::number(func(resultBuffer.get(), byteArray2.constData(), n)) + "\n" + QString(resultBuffer.get()));

    }else if(funcName == "strlen"){

        typedef size_t (*funcStrLen)(const char*);
        funcStrLen func = (funcStrLen)m_stringLib.resolve(funcNameBytes.constData());
        if (!func) {
            log->setPlainText(QString("Error: function %1 not found in the library!").arg(funcName));
            return;
        }
        log->setPlainText(QString("String 1: ") + QString::number(func(byteArray1.constData())) + "\nString 2: " + QString::number(func(byteArray2.constData())));

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
