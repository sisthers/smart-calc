#include <cmath>
#include <iostream>

namespace s21 {
class CreditCalculatorModel {
 private:
  struct CreditOutput {
    double monthly_payment = 0;
    double monthly_payment_last = 0;
    double overpayment = 0;
    double total_payment = 0;
  };

 public:
  enum class CreditType { kAnnual, kDiffer };

  CreditOutput CalculateCredit(long double credit_ammount, uint16_t term,
                               long double interest_rate,
                               CreditType credit_type) const;
};
}  // namespace s21