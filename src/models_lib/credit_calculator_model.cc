#include "credit_calculator_model.h"

namespace s21 {
typename CreditCalculatorModel::CreditOutput
CreditCalculatorModel::CalculateCredit(long double credit_ammount,
                                       uint16_t term, long double interest_rate,
                                       CreditType credit_type) const {
  CreditOutput result;
  if (credit_type == CreditType::kAnnual) {
    long double monthly_rate = interest_rate / (100 * 12);
    result.monthly_payment =
        credit_ammount *
        (monthly_rate + monthly_rate / (powl(1 + monthly_rate, term) - 1));
    result.monthly_payment_last = result.monthly_payment;
    result.total_payment = result.monthly_payment * term;
    result.overpayment = result.total_payment - credit_ammount;
  } else {
    result.total_payment = 0;
    result.overpayment = 0;
    long double SBody = credit_ammount / term;
    long double credit_ammount_left = credit_ammount;
    for (int i = 0; i < term; i++) {
      long double rate_payment =
          credit_ammount_left * interest_rate / (12 * 100);
      credit_ammount_left = credit_ammount_left - SBody;
      result.overpayment += rate_payment;
      long double full_payment = rate_payment + SBody;
      if (i == 0)
        result.monthly_payment = full_payment;
      else if (i == term - 1)
        result.monthly_payment_last = full_payment;
    }
    result.total_payment = credit_ammount + result.overpayment;
  }
  return result;
}
}  // namespace s21