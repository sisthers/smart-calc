#include "deposit_calculator_model.h"

namespace s21 {
void DepositCalculatorModel::AddAction(uint16_t month, long double ammount,
                                       ActionType action_type) {
  actions_.push_back(Action(month, ammount, action_type));
}

void DepositCalculatorModel::ClearActions() { actions_.clear(); }

typename DepositCalculatorModel::DepositOutput
DepositCalculatorModel::CalculateDeposit(
    long double deposit_ammount, uint16_t term, long double interest_rate,
    long double tax_rate, long double tax_for_rate_above,
    uint16_t payment_periodicity,
    DepositCapitalisationType dep_cap_type) const {
  DepositOutput deposit_output;
  deposit_output.deposit_ammount = deposit_ammount;
  long double monthly_rate = interest_rate / (12 / payment_periodicity * 100);
  for (uint16_t i = 0; i < term; ++i) {
    if (i % payment_periodicity == 0) {
      long double payment = deposit_output.deposit_ammount * monthly_rate;
      long double tax = tax_rate / 100 * payment *
                        ((interest_rate - tax_for_rate_above) / interest_rate);
      if (tax_for_rate_above < interest_rate) {
        payment -= tax;
        deposit_output.tax_ammount += tax;
      }
      deposit_output.accrued_interest += payment;
      if (dep_cap_type == DepositCapitalisationType::kCapEnabled)
        deposit_output.deposit_ammount += payment;
    }
    for (auto act : actions_) {
      if (act.month == i + 1) {
        if (act.action_type == ActionType::kWidthDrawal) {
          if (deposit_output.deposit_ammount - act.ammount >= 0)
            deposit_output.deposit_ammount -= act.ammount;
          else
            throw std::runtime_error("No enough money to withdraw");
        } else {
          deposit_output.deposit_ammount += act.ammount;
        }
      }
    }
  }
  return deposit_output;
}
}  // namespace s21