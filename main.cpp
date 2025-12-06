#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>

class MatrixMultiplierWindow : public QWidget {
public:
    MatrixMultiplierWindow(QWidget *parent = nullptr)
        : QWidget(parent) {
        setWindowTitle("Matrix Multiplier");
        setMinimumSize(900, 600);

        auto *rootLayout = new QVBoxLayout(this);
        auto *description = new QLabel(
            "Enter values for each matrix cell. Adjust row and column counts up to 10. "
            "Press \"Multiply\" to see the product.");
        description->setWordWrap(true);
        rootLayout->addWidget(description);

        auto *matrixLayout = new QGridLayout();
        rootLayout->addLayout(matrixLayout);

        matrixLayout->addWidget(createMatrixGroup("Matrix A", rowsA_, colsA_, tableA_), 0, 0);
        matrixLayout->addWidget(createMatrixGroup("Matrix B", rowsB_, colsB_, tableB_), 0, 1);

        multiplyButton_ = new QPushButton("Multiply", this);
        multiplyButton_->setFixedHeight(40);
        matrixLayout->addWidget(multiplyButton_, 1, 0, 1, 2);

        auto *resultGroup = new QGroupBox("Result", this);
        auto *resultLayout = new QVBoxLayout(resultGroup);
        tableResult_ = new QTableWidget(2, 2, resultGroup);
        configureTable(tableResult_);
        resultLayout->addWidget(tableResult_);
        statusLabel_ = new QLabel("Ready to multiply.", resultGroup);
        resultLayout->addWidget(statusLabel_);
        rootLayout->addWidget(resultGroup);

        connect(rowsA_, QOverload<int>::of(&QSpinBox::valueChanged), this, &MatrixMultiplierWindow::syncTableSizes);
        connect(colsA_, QOverload<int>::of(&QSpinBox::valueChanged), this, &MatrixMultiplierWindow::syncTableSizes);
        connect(rowsB_, QOverload<int>::of(&QSpinBox::valueChanged), this, &MatrixMultiplierWindow::syncTableSizes);
        connect(colsB_, QOverload<int>::of(&QSpinBox::valueChanged), this, &MatrixMultiplierWindow::syncTableSizes);
        connect(multiplyButton_, &QPushButton::clicked, this, &MatrixMultiplierWindow::multiply);

        syncTableSizes();
    }

private:
    QGroupBox *createMatrixGroup(const QString &title, QSpinBox *&rowsSpin, QSpinBox *&colsSpin, QTableWidget *&table) {
        auto *group = new QGroupBox(title, this);
        auto *layout = new QVBoxLayout(group);

        auto *controls = new QHBoxLayout();
        controls->addWidget(new QLabel("Rows:"));
        rowsSpin = new QSpinBox(group);
        setupSpin(rowsSpin);
        controls->addWidget(rowsSpin);

        controls->addSpacing(10);
        controls->addWidget(new QLabel("Columns:"));
        colsSpin = new QSpinBox(group);
        setupSpin(colsSpin);
        controls->addWidget(colsSpin);
        controls->addStretch();

        layout->addLayout(controls);

        table = new QTableWidget(2, 2, group);
        configureTable(table);
        layout->addWidget(table);

        return group;
    }

    void setupSpin(QSpinBox *spin) {
        spin->setRange(1, 10);
        spin->setValue(2);
    }

    void configureTable(QTableWidget *table) {
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->setMinimumHeight(200);
        table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    }

    void syncTableSizes() {
        updateTable(tableA_, rowsA_->value(), colsA_->value());
        updateTable(tableB_, rowsB_->value(), colsB_->value());
        tableResult_->setRowCount(rowsA_->value());
        tableResult_->setColumnCount(colsB_->value());
        fillMissingCells(tableResult_);
    }

    void updateTable(QTableWidget *table, int rows, int cols) {
        table->setRowCount(rows);
        table->setColumnCount(cols);
        fillMissingCells(table);
    }

    void fillMissingCells(QTableWidget *table) {
        for (int r = 0; r < table->rowCount(); ++r) {
            for (int c = 0; c < table->columnCount(); ++c) {
                if (!table->item(r, c)) {
                    table->setItem(r, c, new QTableWidgetItem("0"));
                }
            }
        }
    }

    double readValue(const QTableWidget *table, int row, int column) const {
        const auto *item = table->item(row, column);
        bool ok = false;
        double value = item ? item->text().toDouble(&ok) : 0.0;
        return ok ? value : 0.0;
    }

    void multiply() {
        const int rowsA = rowsA_->value();
        const int colsA = colsA_->value();
        const int rowsB = rowsB_->value();
        const int colsB = colsB_->value();

        if (colsA != rowsB) {
            statusLabel_->setText("Matrix multiplication requires columns of A to equal rows of B.");
            tableResult_->clearContents();
            return;
        }

        tableResult_->setRowCount(rowsA);
        tableResult_->setColumnCount(colsB);
        fillMissingCells(tableResult_);

        for (int i = 0; i < rowsA; ++i) {
            for (int j = 0; j < colsB; ++j) {
                double sum = 0.0;
                for (int k = 0; k < colsA; ++k) {
                    sum += readValue(tableA_, i, k) * readValue(tableB_, k, j);
                }
                tableResult_->item(i, j)->setText(QString::number(sum));
            }
        }

        statusLabel_->setText("Multiplication successful.");
    }

    QSpinBox *rowsA_ = nullptr;
    QSpinBox *colsA_ = nullptr;
    QSpinBox *rowsB_ = nullptr;
    QSpinBox *colsB_ = nullptr;

    QTableWidget *tableA_ = nullptr;
    QTableWidget *tableB_ = nullptr;
    QTableWidget *tableResult_ = nullptr;

    QPushButton *multiplyButton_ = nullptr;
    QLabel *statusLabel_ = nullptr;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MatrixMultiplierWindow window;
    window.show();
    return app.exec();
}
