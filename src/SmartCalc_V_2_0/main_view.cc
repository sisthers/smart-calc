#include "main_view.h"

#include "./ui_main_view.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setFixedSize(1466, 425);
  setWindowTitle("Smart Calc V2.0");

  ui->widget->setBackground(QBrush(QColor(233, 223, 233)));
  ui->widget->xAxis->setRange(-10, 10);
  ui->widget->yAxis->setRange(-10, 10);

  connect(ui->eq_button_o, SIGNAL(clicked()), this,
          SLOT(CalculateButtonClicked()));
  connect(ui->remove_button_o, SIGNAL(clicked()), this,
          SLOT(ClearButtonClicked()));

  connect(ui->credit_button_o, SIGNAL(clicked()), this,
          SLOT(CreditCalcClicked()));
  connect(ui->deposit_button_o, SIGNAL(clicked()), this,
          SLOT(DepositCalcClicked()));

  connect(ui->build_button, SIGNAL(clicked()), this,
          SLOT(BuildButtonClicked()));

  for (size_t i = 0; i < ui->tok_buttons_grid->count(); ++i) {
    auto button_widget = ui->tok_buttons_grid->itemAt(i)->widget();
    auto button_type = button_widget->objectName().back();
    if (button_type == 'n' || button_type == 'f' || button_type == 'p')
      connect(button_widget, SIGNAL(clicked()), this,
              SLOT(InputButtonClicked()));
  }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::CreditCalcClicked() { emit CreditCalcOpenSignal(); }

void MainWindow::DepositCalcClicked() { emit DepositCalcOpenSignal(); }

QString MainWindow::GetExpression() const {
  return ui->lineEdit_expression->text();
}

void MainWindow::SetExpression(const QString &expression) {
  ui->lineEdit_expression->setText(expression);
}

void MainWindow::BuildGraph(QVector<double> &&x_axis,
                            QVector<double> &&y_axis) {
  ui->widget->xAxis->setRange(ui->doubleSpinBox_minx->value(),
                              ui->doubleSpinBox_maxx->value());
  ui->widget->yAxis->setRange(ui->doubleSpinBox_miny->value(),
                              ui->doubleSpinBox_maxy->value());
  ui->widget->clearGraphs();
  ui->widget->addGraph();
  ui->widget->graph(0)->setPen(QColor(158, 12, 57));
  ui->widget->graph(0)->addData(x_axis, y_axis);
  ui->widget->replot();
}

double MainWindow::GetXMinValue() const {
  return ui->doubleSpinBox_minx->value();
}

double MainWindow::GetXMaxValue() const {
  return ui->doubleSpinBox_maxx->value();
}

double MainWindow::GetXValue() const {
  return ui->doubleSpinBox_xvalue->value();
}

void MainWindow::CalculateButtonClicked() { emit CalculateSignal(); }

void MainWindow::InputButtonClicked() {
  QPushButton *sender = qobject_cast<QPushButton *>(QObject::sender());
  emit NewInputSignal(sender->text());
}

void MainWindow::ClearButtonClicked() { emit ClearSingal(); }

void MainWindow::BuildButtonClicked() { emit BuildSignal(); }
