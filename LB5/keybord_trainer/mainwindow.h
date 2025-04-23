#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTimer>
#include <QTextCharFormat>
#include <QGridLayout>
#include <QVBoxLayout>

class WordGenerator;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum Language {
        BELARUSIAN = 0,
        RUSSIAN,
        ENGLISH,
        GERMAN,
        FRENCH,
        CHINESE,
        ARABIC,
        HEBREW
    };

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateLanguage(int langIndex);
    void changeLetterColor(const QString &text);
    void startTraining();
    void stopTraining();
    void showStats();
    void openFile();

private:
    QString buttonStyle;
    QTimer *timer;
    int elapsedTime; // в миллисекундах
    int totalCharsTyped;

    void setupUI();
    void createKeyboard();
    void updateKeyboardLayout(Language lang);
    void updateTrainingText(Language lang);
    void colorizeButton(const QString &letter);

    // UI элементы
    QTextEdit *textEdit;
    QLineEdit *inputLine;
    QComboBox *languageCombo;
    QPushButton *startButton;
    QPushButton *openFileButton;
    QVector<QPushButton*> keyboardButtons;
    QPushButton *spaceButton;


    WordGenerator *wordGenerator;
    int currentCharIndex;
    int errors;
    bool isRunning;

    // Форматирование текста
    QTextCharFormat redFormat;
    QTextCharFormat greenFormat;
    QTextCharFormat grayFormat;

    QVBoxLayout *mainLayout;
    QGridLayout *keyboardLayout;

    QVector<QString> germanLayout;
    QVector<QString> frenchLayout;
    QVector<QString> arabicLayout;
    QVector<QString> chineseLayout;
    QVector<QString> belarusianLayout;
    QVector<QString> hebrewLayout;
    QVector<QString> englishLayout;
    QVector<QString> russianLayout;
    void initKeyboardLayouts();
};

#endif // MAINWINDOW_H
