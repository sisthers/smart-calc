#include "controller.h"

#include <QString>
#include <cstring>

// TODO: 1. x entering

namespace s21 {
Controller::Controller(MainWindow* main_window,
                       CreditCalcWindow* credit_calc_window,
                       DepCalcWindow* deposit_calc_window)
    : main_window_(main_window),
      credit_calc_window_(credit_calc_window),
      deposit_calc_window_(deposit_calc_window) {
  connect(main_window_, SIGNAL(NewInputSignal(QString)), this,
          SLOT(NewInputFromButtons(QString)));
  connect(main_window_, SIGNAL(ClearSingal()), this, SLOT(Clear()));
  connect(main_window_, SIGNAL(CalculateSignal()), this,
          SLOT(ExecuteExpressionCalculation()));
  connect(main_window_, SIGNAL(CreditCalcOpenSignal()), this,
          SLOT(OpenCreditCalc()));
  connect(main_window_, SIGNAL(DepositCalcOpenSignal()), this,
          SLOT(OpenDepositCalc()));
  connect(main_window_, SIGNAL(BuildSignal()), this, SLOT(BuildGraph()));

  connect(credit_calc_window_, SIGNAL(CalculateSignal()), this,
          SLOT(ExecuteCreditCalculation()));
  connect(credit_calc_window_, SIGNAL(CloseSignal()), this,
          SLOT(CloseCreditCalc()));

  connect(deposit_calc_window_, SIGNAL(CloseSignal()), this,
          SLOT(CloseDepositCalc()));
  connect(deposit_calc_window_, SIGNAL(ClearSignal()), this,
          SLOT(ClearDepositActions()));
  connect(deposit_calc_window_, SIGNAL(AddActionSignal()), this,
          SLOT(AddDepositAction()));
  connect(deposit_calc_window_, SIGNAL(CalculateSignal()), this,
          SLOT(ExecuteDepositCalculation()));
}

void Controller::Start() { main_window_->show(); }

void Controller::OpenCreditCalc() {
  main_window_->close();
  credit_calc_window_->show();
}

void Controller::CloseCreditCalc() {
  credit_calc_window_->close();
  main_window_->show();
}

void Controller::OpenDepositCalc() {
  main_window_->close();
  deposit_calc_window_->show();
}
void Controller::CloseDepositCalc() {
  deposit_calc_window_->close();
  main_window_->show();
}

void Controller::NewInputFromButtons(QString input) {
  main_window_->SetExpression(main_window_->GetExpression() + input);
}

void Controller::Clear() { main_window_->SetExpression(""); }

void Controller::AddDepositAction() {
  uint16_t month = deposit_calc_window_->GetMonth();
  double ammount = deposit_calc_window_->GetAmmount();
  int action_index = deposit_calc_window_->GetSelectedActionType();
  auto action_type = action_index == 0
                         ? DepositCalculatorModel::ActionType::kReplenishment
                         : DepositCalculatorModel::ActionType::kWidthDrawal;
  deposit_model_.AddAction(month, ammount, action_type);

  QListWidgetItem* item = new QListWidgetItem();
  item->setText(
      QString::number(month) + " " +
      (action_type == DepositCalculatorModel::ActionType::kReplenishment
           ? " Replenishment "
           : " Withdrawal ") +
      QString::number(ammount));
  item->setTextAlignment(Qt::AlignCenter);
  deposit_calc_window_->AddToActionList(item);
}

void Controller::ClearDepositActions() {
  deposit_model_.ClearActions();
  deposit_calc_window_->ClearActionList();
}

void Controller::ExecuteExpressionCalculation() {
  std::string expression =
      main_window_->GetExpression().toLocal8Bit().constData();
  expression += "=";
  std::replace(expression.begin(), expression.end(), '.', ',');

  try {
    double result = expression_model_.CalculateExpression(
        expression, main_window_->GetXValue());
    main_window_->SetExpression(QString::number(result));
  } catch (const std::runtime_error& ex) {
    std::string error = ex.what();
    std::string error_message = "Error: " + error;
    main_window_->SetExpression(QString::fromStdString(error_message));
  }
}

void Controller::ExecuteCreditCalculation() {
  double credit_ammount = credit_calc_window_->GetCreditAmmount();
  uint16_t term = credit_calc_window_->GetTerm();
  double rate = credit_calc_window_->GetInterestRate();
  int type_index = credit_calc_window_->GetSelectedCreditType();
  auto credit_type = type_index == 0
                         ? CreditCalculatorModel::CreditType::kAnnual
                         : CreditCalculatorModel::CreditType::kDiffer;
  auto result =
      credit_model_.CalculateCredit(credit_ammount, term, rate, credit_type);
  credit_calc_window_->SetData(result.monthly_payment,
                               result.monthly_payment_last,
                               result.total_payment, result.overpayment);
}

void Controller::ExecuteDepositCalculation() {
  double deposit_ammount = deposit_calc_window_->GetDepositAmmount();
  uint16_t term = deposit_calc_window_->GetTerm();
  double interest_rate = deposit_calc_window_->GetInterestRate();
  double tax_rate = deposit_calc_window_->GetTaxRate();
  double tax_for_rate_above = deposit_calc_window_->GetTaxRateForRateAbove();
  uint16_t periodicy = deposit_calc_window_->GetPeriodicy();
  int capitalization_index = deposit_calc_window_->GetSelectedCapitalization();
  auto capitalization_type =
      capitalization_index == 0
          ? DepositCalculatorModel::DepositCapitalisationType::kCapDisabled
          : DepositCalculatorModel::DepositCapitalisationType::kCapEnabled;

  try {
    auto result = deposit_model_.CalculateDeposit(
        deposit_ammount, term, interest_rate, tax_rate, tax_for_rate_above,
        periodicy, capitalization_type);

    deposit_calc_window_->SetData(result.accrued_interest,
                                  result.deposit_ammount, result.tax_ammount);
  } catch (const std::runtime_error& ex) {
    std::string error = ex.what();
    std::string error_message = "Error: " + error;
    deposit_calc_window_->SetError(QString::fromStdString(error_message));
  }
}

void Controller::BuildGraph() {
  std::string expression =
      main_window_->GetExpression().toLocal8Bit().constData();
  expression += "=";
  std::replace(expression.begin(), expression.end(), '.', ',');

  try {
    auto axes = expression_model_.CalculateAxesValues(
        expression, main_window_->GetXMinValue(), main_window_->GetXMaxValue());
    main_window_->BuildGraph(
        QVector<double>(axes.first.begin(), axes.first.end()),
        QVector<double>(axes.second.begin(), axes.second.end()));
  } catch (const std::runtime_error& ex) {
    std::string error = ex.what();
    std::string error_message = "Error: " + error;
    main_window_->SetExpression(QString::fromStdString(error_message));
  }
}
}  // namespace s21
