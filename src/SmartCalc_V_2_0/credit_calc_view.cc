#include "credit_calc_view.h"

#include "./ui_credit_calc_view.h"

CreditCalcWindow::CreditCalcWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CreditCalcWindow) {
  ui->setupUi(this);
  setFixedSize(560, 230);
  setWindowTitle("Smart Calc V2.0");

  connect(ui->calculate_button, SIGNAL(clicked()), this,
          SLOT(CalculateClicked()));
  connect(ui->close_button, SIGNAL(clicked()), this, SLOT(CloseClicked()));
}

CreditCalcWindow::~CreditCalcWindow() { delete ui; }

double CreditCalcWindow::GetCreditAmmount() const {
  return ui->doubleSpinBox_ammount->value();
}

double CreditCalcWindow::GetTerm() const {
  return ui->doubleSpinBox_term->value();
}

double CreditCalcWindow::GetInterestRate() const {
  return ui->doubleSpinBox_rate->value();
}

int CreditCalcWindow::GetSelectedCreditType() const {
  return ui->comboBox_type->currentIndex();
}

void CreditCalcWindow::SetData(double first_payment, double last_payment,
                               double total_payment, double overpayment) {
  ui->lineEdit_fpayment->setText(QString::number(first_payment));
  ui->lineEdit_spayment->setText(QString::number(last_payment));
  ui->lineEdit_tpayment->setText(QString::number(total_payment));
  ui->lineEdit_overpayment->setText(QString::number(overpayment));
}

void CreditCalcWindow::CalculateClicked() { emit CalculateSignal(); }

void CreditCalcWindow::CloseClicked() { emit CloseSignal(); }
