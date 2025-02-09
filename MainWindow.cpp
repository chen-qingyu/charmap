#include "MainWindow.h"

#include <QApplication>
#include <QClipboard>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    initUi();
    loadChars();
    connectSignals();

    select(table->item(0, 1));
    search->setFocus();
}

void MainWindow::initUi()
{
    actUsage = new QAction("Usage(&U)");
    actUsage->setStatusTip("Usage...");

    actAbout = new QAction("About(&A)");
    actAbout->setStatusTip("About...");

    QMenu* menuHelp = new QMenu("Help(&H)");
    menuHelp->addAction(actUsage);
    menuHelp->addAction(actAbout);

    QMenuBar* menu = new QMenuBar();
    menu->addMenu(menuHelp);
    this->setMenuBar(menu);

    table = new QTableWidget();
    table->setColumnCount(3);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setHorizontalHeaderLabels({"Upper Case", "Lower Case", "Description"});
    table->verticalHeader()->setFixedWidth(30);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->installEventFilter(this);

    defaultUpper = false;
    search = new QLineEdit();
    search->setFixedHeight(30);
    search->setPlaceholderText("Search...");
    search->installEventFilter(this);

    btnCopy = new QPushButton("Copy");
    btnCopy->setFixedHeight(30);

    auto lyInput = new QHBoxLayout();
    lyInput->addWidget(search);
    lyInput->addWidget(btnCopy);

    selected = nullptr;
    result = new QLabel();
    result->setFixedSize(30, 30);
    result->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    result->setFrameShape(QFrame::Box);

    timer = new QTimer();
    timer->setInterval(1000);
    note = new QLabel();
    note->setFixedHeight(30);
    note->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    auto lyLabel = new QHBoxLayout();
    lyLabel->addStretch();
    lyLabel->addWidget(result);
    lyLabel->addWidget(note);
    lyLabel->addStretch();

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(table);
    layout->addLayout(lyInput);
    layout->addLayout(lyLabel);

    QWidget* centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    this->setCentralWidget(centralWidget);
    this->resize(400, 300);
}

void MainWindow::loadChars()
{
    QList<QString> uppers = {
        "Α", "Β", "Γ", "Δ", "Ε", "Ζ", "Η", "Θ",
        "Ι", "Κ", "Λ", "Μ", "Ν", "Ξ", "Ο", "Π",
        "Ρ", "Σ", "Τ", "Υ", "Φ", "Χ", "Ψ", "Ω"};

    QList<QString> lowers = {
        "α", "β", "γ", "δ", "ε", "ζ", "η", "θ",
        "ι", "κ", "λ", "μ", "ν", "ξ", "ο", "π",
        "ρ", "σ", "τ", "υ", "φ", "χ", "ψ", "ω"};

    QList<QString> descs = {
        "alpha", "beta", "gamma", "delta",
        "epsilon", "zeta", "eta", "theta",
        "iota", "kappa", "lambda", "mu",
        "nu", "xi", "omicron", "pi",
        "rho", "sigma", "tau", "upsilon",
        "phi", "chi", "psi", "omega"};

    table->setRowCount(descs.size());

    for (int i = 0; i < descs.size(); ++i)
    {
        table->setItem(i, 0, new QTableWidgetItem(uppers[i]));
        table->setItem(i, 1, new QTableWidgetItem(lowers[i]));
        table->setItem(i, 2, new QTableWidgetItem(descs[i]));
    }
}

void MainWindow::connectSignals()
{
    connect(actUsage, &QAction::triggered, this, &MainWindow::showUsage);
    connect(actAbout, &QAction::triggered, this, &MainWindow::showAbout);

    connect(table, &QTableWidget::itemClicked, this, &MainWindow::select);
    connect(search, &QLineEdit::textChanged, this, &MainWindow::updateTable);
    connect(btnCopy, &QPushButton::clicked, this, &MainWindow::copyToClipboard);

    connect(timer, &QTimer::timeout, note, &QLabel::clear);
}

void MainWindow::select(QTableWidgetItem* item)
{
    note->clear();

    selected = item;
    if (selected)
    {
        result->setText("<h3>" + selected->text() + "</h3>");
        result->setFixedSize(item->column() == 2 ? 100 : 30, 30);
        result->setFrameShape(QFrame::Box);
        table->setCurrentItem(selected);
    }
    else
    {
        result->clear();
        result->setFixedSize(0, 30);
        result->setFrameShape(QFrame::NoFrame);
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

    timer->start();
}

void MainWindow::showUsage()
{
    QString msg = R"(
- **Using Keyboard (Efficient):**

  1. Type in the search bar to filter characters.
  2. Press Enter to copy the selected character to the clipboard.
  3. Use arrow keys to navigate.
  4. Use Tab to switch between upper and lower case if character case exists.

- **Using Mouse (Easy):**

  1. Click a character in the table to select it.
  2. Click the "Copy" button to copy the selected character to the clipboard.
)";

    QMessageBox::about(this, "Usage", msg);
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, "About",
                       QString("<h2>%1</h2><i>%2</i><p>Author: %3</p><p>Version: %4</p><a href='%5'>%5</a>")
                           .arg(TITLE, DESC, AUTHOR, VERSION, URL));
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key())
        {
            case Qt::Key_Return: // main enter
            case Qt::Key_Enter:  // num enter
                copyToClipboard();
                return true;

            case Qt::Key_Tab:
                defaultUpper = !defaultUpper;
                if (selected)
                {
                    select(table->item(selected->row(), defaultUpper ? 0 : 1));
                }
                note->setText(QString("default %1 case").arg(defaultUpper ? "upper" : "lower"));
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
