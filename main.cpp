#include <QApplication>
#include <QClipboard>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

class MainWindow : public QMainWindow
{
private:
    QTableWidget* table;
    QLabel* result;
    QPushButton* btn;
    QLineEdit* search;

    void initUi()
    {
        auto title = new QLabel();
        title->setText("<h2>GreekChar</h2><i>A quick Greek alphabet lookup table.</i>");
        title->setAlignment(Qt::AlignCenter);

        table = new QTableWidget();
        table->setColumnCount(3);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setHorizontalHeaderLabels({"Upper Case", "Lower Case", "Description"});
        table->verticalHeader()->setFixedWidth(40);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        result = new QLabel();
        result->setFixedHeight(30);
        result->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        btn = new QPushButton("Copy");
        btn->setFixedHeight(30);

        auto hlayout = new QHBoxLayout();
        hlayout->addWidget(result);
        hlayout->addWidget(btn);

        search = new QLineEdit();
        search->setFixedHeight(30);
        search->setPlaceholderText("Search...");

        QVBoxLayout* layout = new QVBoxLayout();
        layout->addWidget(title);
        layout->addWidget(table);
        layout->addLayout(hlayout);
        layout->addWidget(search);

        QWidget* centralWidget = new QWidget();
        centralWidget->setLayout(layout);
        this->setCentralWidget(centralWidget);
        this->resize(400, 300);
    }

    void loadLetters()
    {
        QList<QString> uppers = {
            "Α", "Β", "Γ", "Δ", "Ε", "Ζ", "Η", "Θ",
            "Ι", "Κ", "Λ", "Μ", "Ν", "Ξ", "Ο", "Π",
            "Ρ", "Σ", "Τ", "Υ", "Φ", "Χ", "Ψ", "Ω"};

        QList<QString> lowers = {
            "α", "β", "γ", "δ", "ε", "ζ", "η", "θ",
            "ι", "κ", "λ", "μ", "ν", "ξ", "ο", "π",
            "ρ", "σ", "τ", "υ", "φ", "χ", "ψ", "ω"};

        QList<QString> desc = {
            "alpha", "beta", "gamma", "delta",
            "epsilon", "zeta", "eta", "theta",
            "iota", "kappa", "lambda", "mu",
            "nu", "xi", "omicron", "pi",
            "rho", "sigma", "tau", "upsilon",
            "phi", "chi", "psi", "omega"};

        table->setRowCount(desc.size());

        for (int i = 0; i < desc.size(); ++i)
        {
            table->setItem(i, 0, new QTableWidgetItem(uppers[i]));
            table->setItem(i, 1, new QTableWidgetItem(lowers[i]));
            table->setItem(i, 2, new QTableWidgetItem(desc[i]));
        }
    }

    void connectSignals()
    {
        connect(table, &QTableWidget::itemClicked, this, [this](QTableWidgetItem* item)
                { result->setText(item->text()); });

        connect(btn, &QPushButton::clicked, this, &MainWindow::copyToClipboard);

        connect(search, &QLineEdit::textChanged, this, &MainWindow::updateTable);
    }

    void copyToClipboard()
    {
        if (!result->text().isEmpty())
        {
            QGuiApplication::clipboard()->setText(result->text());
        }
    }

    void updateTable(const QString& text)
    {
        for (int i = 0; i < table->rowCount(); ++i)
        {
            QString desc = table->item(i, 2)->text(); // lower description
            table->setRowHidden(i, !(desc.contains(text.toLower()) ||
                                     text == table->item(i, 0)->text() ||
                                     text == table->item(i, 1)->text() ||
                                     text == QString::number(i + 1)));
        }
    }

public:
    MainWindow(QWidget* parent = nullptr)
        : QMainWindow(parent)
    {
        initUi();
        loadLetters();
        connectSignals();
    }
};

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("GreekChar");
    window.show();

    return app.exec();
}
