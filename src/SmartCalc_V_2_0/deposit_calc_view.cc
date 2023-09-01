#include "deposit_calc_view.h"

#include "ui_deposit_calc_view.h"

DepCalcWindow::DepCalcWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::DepCalcWindow) {
  ui->setupUi(this);
  this->setFixedSize(810, 300);
  setWindowTitle("Smart Calc V2.0");

  connect(ui->calculate_button, SIGNAL(clicked()), this,
          SLOT(CalculateClicked()));
  connect(ui->close_button, SIGNAL(clicked()), this, SLOT(CloseClicked()));
  connect(ui->clear_button, SIGNAL(clicked()), this, SLOT(ClearClicked()));
  connect(ui->add_button, SIGNAL(clicked()), this, SLOT(AddActionClicked()));
}

DepCalcWindow::~DepCalcWindow() { delete ui; }

double DepCalcWindow::GetDepositAmmount() const {
  return ui->doubleSpinBox_depammount->value();
}
double DepCalcWindow::GetTerm() const {
  return ui->doubleSpinBox_term->value();
}

double DepCalcWindow::GetInterestRate() const {
  return ui->doubleSpinBox_interestRate->value();
}

double DepCalcWindow::GetTaxRate() const {
  return ui->doubleSpinBox_taxRate->value();
}

double DepCalcWindow::GetTaxRateForRateAbove() const {
  return ui->doubleSpinBox_TaxForIntRaAb->value();
}

double DepCalcWindow::GetPeriodicy() const {
  return ui->doubleSpinBox_Payment_periodicity->value();
}

int DepCalcWindow::GetSelectedCapitalization() const {
  return ui->comboBox_cap->currentIndex();
}

double DepCalcWindow::GetMonth() const {
  return ui->doubleSpinBox_month->value();
}

int DepCalcWindow::GetSelectedActionType() const {
  return ui->comboBox_type->currentIndex();
}

double DepCalcWindow::GetAmmount() const {
  return ui->doubleSpinBox_ammount->value();
}

void DepCalcWindow::SetData(double accurred_interest, double deposit_ammount,
                            double tax_ammount) {
  ui->lineEdit_interest->setText(QString::number(accurred_interest));
  ui->lineEdit_ammount->setText(QString::number(deposit_ammount));
  ui->lineEdit_tax->setText(QString::number(tax_ammount));
}

void DepCalcWindow::SetError(const QString& error) {
  ui->error_label->setText(error);
}

void DepCalcWindow::AddToActionList(QListWidgetItem* item) {
  ui->listWidget->addItem(item);
}

void DepCalcWindow::ClearActionList() { ui->listWidget->clear(); }

void DepCalcWindow::CalculateClicked() { emit CalculateSignal(); }

void DepCalcWindow::CloseClicked() { emit CloseSignal(); }

void DepCalcWindow::ClearClicked() { emit ClearSignal(); }

void DepCalcWindow::AddActionClicked() { emit AddActionSignal(); }
