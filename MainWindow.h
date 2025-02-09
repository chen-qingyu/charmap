#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QTimer>

#include "defs.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QAction* actUsage;
    QAction* actAbout;
    QTableWidget* table;
    QTableWidgetItem* selected;
    QLabel* result;
    QTimer* timer;
    QLabel* note;
    bool defaultUpper;
    QLineEdit* search;
    QPushButton* btnCopy;

    void initUi();
    void loadChars();
    void connectSignals();

private slots:
    void select(QTableWidgetItem* item);
    void updateTable(const QString& text);
    void copyToClipboard();

    void showUsage();
    void showAbout();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

public:
    MainWindow(QWidget* parent = nullptr);
};

#endif // MAINWINDOW_H
