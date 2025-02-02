#include <QApplication>
#include <QCheckBox>
#include <QClipboard>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

class MainWindow : public QMainWindow
{
private:
    QTableWidget* table;

    QTableWidgetItem* selected;
    QLabel* result;
    QLabel* note;

    QCheckBox* chkUpper;
    QLineEdit* search;
    QPushButton* btnCopy;

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

        selected = nullptr;
        result = new QLabel();
        result->setFixedHeight(30);
        result->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        note = new QLabel();
        note->setFixedHeight(30);
        note->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        auto lyLabel = new QHBoxLayout();
        lyLabel->addStretch();
        lyLabel->addWidget(result);
        lyLabel->addWidget(note);
        lyLabel->addStretch();

        chkUpper = new QCheckBox("Upper Case");
        chkUpper->setFixedHeight(30);

        search = new QLineEdit();
        search->setFixedHeight(30);
        search->setPlaceholderText("Search...");

        btnCopy = new QPushButton("Copy");
        btnCopy->setFixedHeight(30);

        auto lyInput = new QHBoxLayout();
        lyInput->addWidget(chkUpper);
        lyInput->addWidget(search);
        lyInput->addWidget(btnCopy);

        QVBoxLayout* layout = new QVBoxLayout();
        layout->addWidget(title);
        layout->addWidget(table);
        layout->addLayout(lyLabel);
        layout->addLayout(lyInput);

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
        connect(table, &QTableWidget::itemClicked, this, &MainWindow::select);

        connect(chkUpper, &QCheckBox::checkStateChanged, this, &MainWindow::updateChar);

        connect(search, &QLineEdit::textChanged, this, &MainWindow::updateTable);
        connect(search, &QLineEdit::returnPressed, this, &MainWindow::copyToClipboard);

        connect(btnCopy, &QPushButton::clicked, this, &MainWindow::copyToClipboard);
    }

    void select(QTableWidgetItem* item)
    {
        selected = item;
        if (selected)
        {
            result->setText("<h3>" + selected->text() + "</h3>");
        }
        else
        {
            result->clear();
        }
    }

    void updateChar(Qt::CheckState state)
    {
        if (selected != nullptr)
        {
            int row = selected->row();
            auto item = state == Qt::Checked ? table->item(row, 0) : table->item(row, 1);
            select(item);
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
        for (int i = 0; i < table->rowCount(); ++i)
        {
            if (!table->isRowHidden(i))
            {
                auto item = chkUpper->isChecked() ? table->item(i, 0) : table->item(i, 1);
                select(item);
                return;
            }
        }
        select(nullptr);
    }

    void copyToClipboard()
    {
        if (selected)
        {
            QGuiApplication::clipboard()->setText(selected->text());
            note->setText("copied!");
        }
        else
        {
            note->setText("no selected char.");
        }

        QTimer::singleShot(1000, this, [this]()
                           { note->clear(); });
    }

public:
    MainWindow(QWidget* parent = nullptr)
        : QMainWindow(parent)
    {
        initUi();
        loadLetters();
        connectSignals();

        search->setFocus();
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
