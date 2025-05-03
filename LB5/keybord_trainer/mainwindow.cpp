#include "mainwindow.h"
#include "wordgenerator.h"
#include <QMessageBox>
#include <QPalette>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    elapsedTime(0),
    totalCharsTyped(0),
    wordGenerator(new WordGenerator()),
    currentCharIndex(0),
    errors(0),
    isRunning(false)
{
    initKeyboardLayouts();
    setupUI();
    createKeyboard();

    redFormat.setForeground(Qt::red);
    greenFormat.setForeground(Qt::darkGreen);
    grayFormat.setForeground(Qt::gray);

    updateLanguage(BELARUSIAN);
    updateTrainingText(BELARUSIAN);

    timer = new QTimer(this);

    connect(inputLine, &QLineEdit::textEdited, this, &MainWindow::changeLetterColor);
    connect(languageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::updateLanguage);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startTraining);
    connect(openFileButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(timer, &QTimer::timeout, [this]() {
        elapsedTime += 100;
    });
}

MainWindow::~MainWindow()
{
    delete wordGenerator;
}

void MainWindow::initKeyboardLayouts()
{

    belarusianLayout = {
        "Й", "Ц", "У", "К", "Е", "Н", "Г", "Ш", "Ў", "З", "Х", "'",
        "Ф", "Ы", "В", "А", "П", "Р", "О", "Л", "Д", "Ж", "Э",
        "Я", "Ч", "С", "М", "І", "Т", "Ь", "Б", "Ю", "."
    };

    germanLayout = {
        "Q", "W", "E", "R", "T", "Z", "U", "I", "O", "P", "Ü", "+",
        "A", "S", "D", "F", "G", "H", "J", "K", "L", "Ö", "Ä",
        "Y", "X", "C", "V", "B", "N", "M", ",", ".", "-"
    };

    frenchLayout = {
        "Q q", "W w", "E e", "R é r", "T è t", "Y y", "U u", "I i", "O e ô p", "P c", "\" \"", "\" \"",
        "A a", "À s", "S a", "D d", "E é f", "F é g", "G h", "H j", "J k", "K i", "L l", ": :",
        "> <", "Z z", "Ä x", "X c", "C ç", "V v", "B b", "N n", "M m", ", <", ". >", "/ ?"
    };

    chineseLayout = {
        "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "{", "}",
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ":", "\"",
        "Z", "X", "C", "V", "B", "N", "M", "<", ">", "?"
    };

    arabicLayout = {
        "ض", "ص", "ث", "ق", "ف", "غ", "ع", "ه", "خ", "ح", "ج", "د",
        "ش", "س", "ي", "ب", "ل", "ا", "ت", "ن", "م", "ك", "ط",
        "ئ", "ء", "ؤ", "ر", "لا", "ى", "ة", "و", "ز", "ظ"
    };

    hebrewLayout = {
        "/", "'", "ק", "ר", "א", "ט", "ו", "ן", "ם", "פ", "]", "[",
        "ש", "ד", "ג", "כ", "ע", "י", "ח", "ל", "ך", "ף", ",",
        "ז", "ס", "ב", "ה", "נ", "מ", "צ", "ת", "ץ", "."
    };

    englishLayout = {
        "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]",
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'",
        "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/"
    };

    russianLayout = {
        "Й", "Ц", "У", "К", "Е", "Н", "Г", "Ш", "Щ", "З", "Х", "Ъ",
        "Ф", "Ы", "В", "А", "П", "Р", "О", "Л", "Д", "Ж", "Э",
        "Я", "Ч", "С", "М", "И", "Т", "Ь", "Б", "Ю", "."
    };
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(centralWidget);

    // Текстовое поле для отображения текста
    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    textEdit->setFont(QFont("Arial", 14));
    mainLayout->addWidget(textEdit);

    // Поле ввода
    inputLine = new QLineEdit(this);
    inputLine->setFont(QFont("Arial", 14));
    mainLayout->addWidget(inputLine);

    QHBoxLayout *controlLayout = new QHBoxLayout();

    languageCombo = new QComboBox(this);
    languageCombo->addItem("Беларуская", BELARUSIAN);
    languageCombo->addItem("Русский", RUSSIAN);
    languageCombo->addItem("English", ENGLISH);
    languageCombo->addItem("Deutsch", GERMAN);
    languageCombo->addItem("Français", FRENCH);
    languageCombo->addItem("中文", CHINESE);
    languageCombo->addItem("العربية", ARABIC);
    languageCombo->addItem("עברית", HEBREW);
    controlLayout->addWidget(languageCombo);

    startButton = new QPushButton("Start", this);
    openFileButton = new QPushButton("Open File", this);

    controlLayout->addWidget(startButton);
    controlLayout->addWidget(openFileButton);


    mainLayout->addLayout(controlLayout);

    keyboardLayout = new QGridLayout();
    mainLayout->addLayout(keyboardLayout);

    setCentralWidget(centralWidget);
    resize(800, 600);
}

void MainWindow::createKeyboard() {
    qDeleteAll(keyboardButtons);
    keyboardButtons.clear();

    const int btnWidth = 50;
    const int btnHeight = 50;

    buttonStyle = "QPushButton {"
                          "    font-size: 14px;"
                          "    text-align: center;"
                          "    padding: 3px;"
                          "    border-radius: 5px;"
                          "    border: 1px solid #ccc;"
                          "}";

    // --- Верхний ряд ---
    QHBoxLayout *topRowLayout = new QHBoxLayout();
    topRowLayout->setSpacing(5);
    for (int i = 0; i < 12; ++i) {
        QPushButton *btn = new QPushButton(this);
        btn->setFixedSize(btnWidth, btnHeight);
        btn->setStyleSheet(buttonStyle);
        keyboardButtons.append(btn);
        topRowLayout->addWidget(btn);
    }
    keyboardLayout->addLayout(topRowLayout, 0, 0, 1, 12);

    // --- Средний ряд ---
    QHBoxLayout *middleRowLayout = new QHBoxLayout();
    middleRowLayout->setSpacing(5);
    middleRowLayout->addSpacing(btnWidth / 2);
    for (int i = 0; i < 11; ++i) {
        QPushButton *btn = new QPushButton(this);
        btn->setFixedSize(btnWidth, btnHeight);
        btn->setStyleSheet(buttonStyle);
        keyboardButtons.append(btn);
        middleRowLayout->addWidget(btn);
    }
    keyboardLayout->addLayout(middleRowLayout, 1, 0, 1, 12);

    // --- Нижний ряд ---
    QHBoxLayout *bottomRowLayout = new QHBoxLayout();
    bottomRowLayout->setSpacing(5);
    bottomRowLayout->addSpacing(btnWidth);
    for (int i = 0; i < 10; ++i) {
        QPushButton *btn = new QPushButton(this);
        btn->setFixedSize(btnWidth, btnHeight);
        btn->setStyleSheet(buttonStyle);
        keyboardButtons.append(btn);
        bottomRowLayout->addWidget(btn);
    }
    keyboardLayout->addLayout(bottomRowLayout, 2, 0, 1, 12);

    // --- Пробел ---
    spaceButton = new QPushButton("Space", this);
    spaceButton->setFixedHeight(btnHeight);
    spaceButton->setStyleSheet(buttonStyle + " font-size: 16px;");
    keyboardLayout->addWidget(spaceButton, 3, 2, 1, 8);
}

void MainWindow::updateKeyboardLayout(Language lang)
{
    QVector<QString> layout;

    switch(lang) {
    case GERMAN:
        layout = germanLayout;
        break;
    case FRENCH:
        layout = frenchLayout;
        break;
    case ARABIC:
        layout = arabicLayout;
        break;
    case CHINESE:
        layout = chineseLayout;
        break;
    case BELARUSIAN:
        layout = belarusianLayout;
        break;
    case HEBREW:
        layout = hebrewLayout;
        break;
    case RUSSIAN:
        layout = russianLayout;
        break;
    case ENGLISH:
        layout = englishLayout;
        break;
    }


    for (int i = 0; i < keyboardButtons.size() && i < layout.size(); ++i) {
        keyboardButtons[i]->setText(layout[i]);
    }
}

void MainWindow::updateTrainingText(Language lang)
{
    currentCharIndex = 0;
    QStringList words = wordGenerator->shuffleWords(lang);
    QString text = words.join(" ");

    textEdit->clear();
    textEdit->setCurrentCharFormat(grayFormat);
    textEdit->setPlainText(text);
}

void MainWindow::startTraining()
{
    if (isRunning) return;

    totalCharsTyped = 0;
    elapsedTime = 0;
    currentCharIndex = 0;
    errors = 0;
    isRunning = true;
    inputLine->clear();
    inputLine->setFocus();
    timer->start(100);

}

void MainWindow::stopTraining()
{
    if (!isRunning) return;
    timer->stop();
    isRunning = false;
    showStats();
    updateTrainingText(static_cast<Language>(languageCombo->currentData().toInt()));
}

void MainWindow::changeLetterColor(const QString &text)
{
    if (text.isEmpty()) return;

    if (!isRunning) {
        startTraining();
        return;
    }

    QString trainingText = textEdit->toPlainText();

    if (trainingText.isEmpty()) {
        return;
    }

    // Обработка backspace
    if (text.length() < currentCharIndex) {
        currentCharIndex = qMax(0, currentCharIndex - 1);
        QTextCursor cursor = textEdit->textCursor();
        cursor.setPosition(currentCharIndex);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        cursor.setCharFormat(grayFormat);
        return;
    }

    if (currentCharIndex >= trainingText.length() || text.length() <= currentCharIndex) {
        return;
    }

    if (text.length() > currentCharIndex) {
        totalCharsTyped++;
    }

    QTextCursor cursor = textEdit->textCursor();
    cursor.setPosition(currentCharIndex);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

    QChar currentChar = trainingText.at(currentCharIndex);
    if (currentChar == text.at(currentCharIndex)) {
        cursor.setCharFormat(greenFormat);
    } else {
        cursor.setCharFormat(redFormat);
        errors++;
    }

    colorizeButton(text.at(currentCharIndex).toUpper());
    currentCharIndex++;

    // Проверка завершения тренировки
    if (currentCharIndex >= trainingText.length()) {
        stopTraining();
    }
}

void MainWindow::colorizeButton(const QString &letter) {
    QString highlightStyle = "QPushButton {"
                             "    background-color: green;"
                             "    color: white;"
                             "    font-size: 20px;"
                             "    text-align: center;"
                             "    padding: 5px;"
                             "    border-radius: 5px;"
                             "    border: 1px solid #ccc;"
                             "}";

    for (QPushButton *btn : keyboardButtons) {
        if (btn->text() == letter) {
            btn->setStyleSheet(highlightStyle);
            QTimer::singleShot(100, [btn, this]() {
                btn->setStyleSheet(this->buttonStyle);
            });
            return;
        }
    }

    if (letter == " ") {
        spaceButton->setStyleSheet(highlightStyle);
        QTimer::singleShot(100, [this]() {
            spaceButton->setStyleSheet(this->buttonStyle + " font-size: 16px;");
        });
    }
}

void MainWindow::showStats()
{
    int totalChars = currentCharIndex;
    int correctChars = totalChars - errors;
    double accuracy = totalChars > 0 ? (correctChars * 100.0 / totalChars) : 0;

    double minutes = elapsedTime / 60000.0;
    double speed = minutes > 0 ? (totalCharsTyped / minutes) : 0;

    QMessageBox statsDialog;
    statsDialog.setWindowTitle("statics");
    statsDialog.setText(QString("Mistakes: %1\nconsistency: %2%\nspeed: %3 сh./min")
                            .arg(errors)
                            .arg(accuracy, 0, 'f', 1)
                            .arg(static_cast<int>(speed)));
    statsDialog.exec();
}

void MainWindow::updateLanguage(int langIndex)
{
    if (isRunning) stopTraining();
    Language lang = static_cast<Language>(languageCombo->itemData(langIndex).toInt());
    updateKeyboardLayout(lang);
    updateTrainingText(lang);
}

void MainWindow::openFile() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open Text File", "", "Text Files (*.txt)");

    if (!filePath.isEmpty() && filePath.endsWith(".txt", Qt::CaseInsensitive)) {
        try{
            QFile file(filePath);

            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                throw std::runtime_error("Cannot open file for reading: " + file.errorString().toStdString());
            }
            QString str;
            QTextStream in(&file);
            while (!in.atEnd()) {
                str += in.readLine();
            }
            file.close();

            textEdit->clear();
            textEdit->setCurrentCharFormat(grayFormat);
            textEdit->setPlainText(str);
        }catch(const std::runtime_error& e){
            QMessageBox::warning(nullptr, "Error", "Cannot open file for reading:");
        }
    } else {
        QMessageBox::warning(this, "Invalid File", "Please select a valid .txt file.");
    }
}
