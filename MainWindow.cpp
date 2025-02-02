#include "MainWindow.h"

#include <QApplication>
#include <QClipboard>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLineEdit>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    initUi();
    loadLetters();
    connectSignals();

    select(table->item(0, 1));
    search->setFocus();
}

void MainWindow::initUi()
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

    defaultUpper = false;
    search = new QLineEdit();
    search->setFixedHeight(30);
    search->setPlaceholderText("Search...");
    search->installEventFilter(this); // switch case

    btnCopy = new QPushButton("Copy");
    btnCopy->setFixedHeight(30);

    auto lyInput = new QHBoxLayout();
    lyInput->addWidget(search);
    lyInput->addWidget(btnCopy);

    selected = nullptr;
    result = new QLabel();
    result->setFixedHeight(30);
    result->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    timer = new QTimer();
    timer->setInterval(1000);
    note = new QLabel();
    note->setFixedHeight(30);
    note->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    auto lyLabel = new QHBoxLayout();
    lyLabel->addStretch();
    lyLabel->addWidget(result);
    lyLabel->addWidget(note);
    lyLabel->addStretch();

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(title);
    layout->addWidget(table);
    layout->addLayout(lyInput);
    layout->addLayout(lyLabel);

    QWidget* centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    this->setCentralWidget(centralWidget);
    this->resize(400, 300);
}

void MainWindow::loadLetters()
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

void MainWindow::connectSignals()
{
    connect(table, &QTableWidget::itemClicked, this, &MainWindow::select);

    connect(search, &QLineEdit::textChanged, this, &MainWindow::updateTable);
    connect(search, &QLineEdit::returnPressed, this, &MainWindow::copyToClipboard);

    connect(btnCopy, &QPushButton::clicked, this, &MainWindow::copyToClipboard);

    connect(timer, &QTimer::timeout, note, &QLabel::clear);
}

void MainWindow::select(QTableWidgetItem* item)
{
    selected = item;
    note->clear();
    if (selected)
    {
        result->setText("<h3>" + selected->text() + "</h3>");
        table->setCurrentItem(selected);
    }
    else
    {
        result->clear();
    }
}

void MainWindow::updateTable(const QString& text)
{
    // filter table rows
    for (int i = 0; i < table->rowCount(); ++i)
    {
        QString desc = table->item(i, 2)->text(); // lower description
        table->setRowHidden(i, !(desc.contains(text.toLower()) ||
                                 text == table->item(i, 0)->text() ||
                                 text == table->item(i, 1)->text() ||
                                 text == QString::number(i + 1)));
    }

    // select first visible row
    for (int i = 0; i < table->rowCount(); ++i)
    {
        if (!table->isRowHidden(i))
        {
            select(table->item(i, defaultUpper ? 0 : 1));
            return;
        }
    }

    // no visible row
    select(nullptr);
}

void MainWindow::copyToClipboard()
{
    if (selected)
    {
        QGuiApplication::clipboard()->setText(selected->text());
        note->setText("copied!");
    }
    else
    {
        note->setText("no selected char");
    }

    timer->stop();
    timer->start();
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key())
        {
            case Qt::Key_Tab:
                defaultUpper = !defaultUpper;
                if (selected)
                {
                    select(table->item(selected->row(), defaultUpper ? 0 : 1));
                }
                note->setText(QString("default ") + (defaultUpper ? "upper case" : "lower case"));
                timer->stop();
                timer->start();
                return true;

            case Qt::Key_Up:
                if (selected)
                {
                    int row = selected->row() - 1;
                    while (row >= 0 && table->isRowHidden(row))
                    {
                        row--;
                    }
                    if (row >= 0)
                    {
                        select(table->item(row, selected->column()));
                    }
                }
                return true;

            case Qt::Key_Down:
                if (selected)
                {
                    int row = selected->row() + 1;
                    while (row < table->rowCount() && table->isRowHidden(row))
                    {
                        row++;
                    }
                    if (row < table->rowCount())
                    {
                        select(table->item(row, selected->column()));
                    }
                }

                return true;

            case Qt::Key_Left:
                if (selected)
                {
                    int column = selected->column() - 1;
                    if (column >= 0)
                    {
                        select(table->item(selected->row(), column));
                    }
                }
                return true;

            case Qt::Key_Right:
                if (selected)
                {
                    int column = selected->column() + 1;
                    if (column < table->columnCount())
                    {
                        select(table->item(selected->row(), column));
                    }
                }
                return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}
