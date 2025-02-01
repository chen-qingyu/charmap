#include <QApplication>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget* parent = nullptr)
        : QMainWindow(parent)
    {
        this->setWindowTitle("GreekChar: A quick Greek alphabet lookup table.");
    }
};

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
