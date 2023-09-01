#include <iostream>
#include <vector>

namespace s21 {
class DepositCalculatorModel {
 private:
  struct DepositOutput {
    long double accrued_interest = 0;
    long double tax_ammount = 0;
    long double deposit_ammount = 0;
  };

 public:
  enum class DepositCapitalisationType { kCapEnabled, kCapDisabled };

  enum class ActionType { kWidthDrawal, kReplenishment };

  struct Action {
    Action(uint16_t month, long double ammount, ActionType action_type)
        : month(month), ammount(ammount), action_type(action_type) {}
    uint16_t month;
    long double ammount;
    ActionType action_type;
  };

  void AddAction(uint16_t month, long double ammount, ActionType action_type);
  void ClearActions();
  DepositOutput CalculateDeposit(long double deposit_ammount, uint16_t term,
                                 long double interest_rate,
                                 long double tax_rate,
                                 long double tax_for_rate_above,
                                 uint16_t payment_periodicity,
                                 DepositCapitalisationType dep_cap_type) const;

 private:
  std::vector<Action> actions_;
};
}  // namespace s21