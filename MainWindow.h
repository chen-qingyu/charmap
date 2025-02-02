#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTableWidget* table;
    QTableWidgetItem* selected;
    QLabel* result;
    QLabel* note;
    bool defaultUpper;
    QLineEdit* search;
    QPushButton* btnCopy;

    void initUi();
    void loadLetters();
    void connectSignals();

private slots:
    void select(QTableWidgetItem* item);
    void updateTable(const QString& text);
    void copyToClipboard();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

public:
    MainWindow(QWidget* parent = nullptr);
};

#endif // MAINWINDOW_H
