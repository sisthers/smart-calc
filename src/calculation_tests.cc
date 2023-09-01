#include <gtest/gtest.h>

#include "models_lib/credit_calculator_model.h"
#include "models_lib/deposit_calculator_model.h"
#include "models_lib/expression_calculator_model.h"

TEST(ExpressionCalculator, Success) {
  std::string test_str;
  long double x, expected_result, result;
  s21::ExpressionCalculatorModel model;

  test_str = "123.123=";
  x = 0;
  expected_result = 123.123;
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "sqrt(25)*acos(tan(10))=";
  x = 0;
  expected_result = sqrt(25) * acos(tan(10));
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "(123-x)/(123+x)=";
  x = 15;
  expected_result = (123 - x) / (123 + x);
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str =
      "121-8*log(16*ln(10)/((13)mod(5)))+"
      "sin(6^9)/cos(3.14)+atan(asin(x))=";
  x = 0.192;
  expected_result = 121 - 8 * log10(16 * log(10) / (fmod(13, 5))) +
                    sin(pow(6, 9)) / cos(3.14) + atan(asin(x));
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "10-(-10)=";
  x = 0;
  expected_result = 10 - (-10);
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "-sin(120)=";
  x = 0;
  expected_result = -sin(120);
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "sin(-120)=";
  x = 0;
  expected_result = sin(-120);
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "124-(-1)mod(4)=";
  x = 0;
  expected_result = 124 - fmod(-1, 4);
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "124- (-12) mod 5=";
  x = 0;
  expected_result = 124 - fmod(-12, 5);
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "-124-(12)mod(-4)=";
  x = 0;
  expected_result = -124 - fmod(+12, -4);
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "ln(25-(-x))=";
  x = 4;
  expected_result = log(25 - (-x));
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "-x+10=";
  x = 4;
  expected_result = -x + 10;
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);
  test_str = "ln(-(2 - 5))=";
  x = 0;
  expected_result = log(-(2 - 5));
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "sqrt(-(-5)+9)=";
  x = 0;
  expected_result = sqrt(-(-5) + 9);
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "sqrt(-(-1251 + 600) + 9 mod 4 - (11 * 5) * (-1))=";
  x = 0;
  expected_result = sqrt(-(-1251 + 600) + 9 % 4 - (11 * 5) * (-1));
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "12+(+5+(+6))=";
  x = 0;
  expected_result = 12 + (+5 + (+6));
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "2^5=";
  x = 0;
  expected_result = 32;
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "sin(x)mod 2=";
  x = 0;
  expected_result = fmod(sin(x), 2);
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "10 modx=";
  x = 1;
  expected_result = fmod(10, x);
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);

  test_str = "xmod 2=";
  x = 1;
  expected_result = fmod(x, 2);
  result = model.CalculateExpression(test_str, x);
  ASSERT_DOUBLE_EQ(expected_result, result);
}

TEST(ExpressionCalculator, Failure) {
  std::string test_str;
  s21::ExpressionCalculatorModel model;
  long double x = 0;

  test_str = "x123=";
  bool catched = false;
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "123.123.23=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "123x=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "123/=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "123*3+=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "123*3-=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "x^=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "148/23mod=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "12-sin(123))=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "2modmod10=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "2***3=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "^2-13=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "mod2-13=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "12*pli(123)=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "12*sin(123.)=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "12*sin(123))*cos(10.123)=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "152sin10=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "sin)10(=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = ")10*10(=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "12*pli123=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "12^123*((pli123)=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "atan4cos=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "100-+-1=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "2*+ -1=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "2+xx=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "log(23*sqrt())=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "log(23*sqrt()=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "log(23*sqrt()))=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = ")(=";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);

  catched = false;
  test_str = "23*.1";
  try {
    model.CalculateExpression(test_str, x);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);
}

TEST(CreditCalculator, Annual) {
  s21::CreditCalculatorModel model;

  auto result = model.CalculateCredit(
      100000, 12, 9.5, s21::CreditCalculatorModel::CreditType::kAnnual);
  ASSERT_NEAR(result.monthly_payment, 8768.35, 1);
  ASSERT_NEAR(result.total_payment, 105220.2, 1);
  ASSERT_NEAR(result.overpayment, 5220.2, 1);

  result = model.CalculateCredit(
      250000, 14, 21, s21::CreditCalculatorModel::CreditType::kAnnual);
  ASSERT_NEAR(result.monthly_payment, 20288.9, 1);
  ASSERT_NEAR(result.total_payment, 284044.6, 1);
  ASSERT_NEAR(result.overpayment, 34044.6, 1);
}

TEST(CreditCalculator, Differ) {
  s21::CreditCalculatorModel model;

  auto result = model.CalculateCredit(
      100000, 12, 9.5, s21::CreditCalculatorModel::CreditType::kDiffer);
  ASSERT_NEAR(result.monthly_payment, 9125, 1);
  ASSERT_NEAR(result.monthly_payment_last, 8399.31, 1);
  ASSERT_NEAR(result.total_payment, 105145.83, 1);
  ASSERT_NEAR(result.overpayment, 5145.83, 1);

  result = model.CalculateCredit(
      250000, 14, 21, s21::CreditCalculatorModel::CreditType::kDiffer);
  ASSERT_NEAR(result.monthly_payment, 22232.14, 1);
  ASSERT_NEAR(result.monthly_payment_last, 18169.64, 1);
  ASSERT_NEAR(result.total_payment, 282812.5, 1);
  ASSERT_NEAR(result.overpayment, 32812.5, 1);
}

TEST(DepositCalculatorModel, CapDisabled) {
  s21::DepositCalculatorModel model;

  auto result = model.CalculateDeposit(
      100000, 12, 10, 10, 15, 1,
      s21::DepositCalculatorModel::DepositCapitalisationType::kCapDisabled);
  ASSERT_NEAR(result.accrued_interest, 10000, 1);
  ASSERT_NEAR(result.deposit_ammount, 100000, 1);
  ASSERT_NEAR(result.tax_ammount, 0, 1);

  result = model.CalculateDeposit(
      100000, 12, 10, 25, 5, 1,
      s21::DepositCalculatorModel::DepositCapitalisationType::kCapDisabled);
  ASSERT_NEAR(result.accrued_interest, 8750, 1);
  ASSERT_NEAR(result.deposit_ammount, 100000, 1);
  ASSERT_NEAR(result.tax_ammount, 1250, 1);
}

TEST(DepositCalculatorm, CapEnabled) {
  s21::DepositCalculatorModel model;

  auto result = model.CalculateDeposit(
      100000, 12, 10, 0, 0, 1,
      s21::DepositCalculatorModel::DepositCapitalisationType::kCapEnabled);
  ASSERT_NEAR(result.accrued_interest, 10471, 1);
  ASSERT_NEAR(result.deposit_ammount, 110471, 1);
  ASSERT_NEAR(result.tax_ammount, 0, 1);

  result = model.CalculateDeposit(
      100000, 12, 10, 25, 5, 1,
      s21::DepositCalculatorModel::DepositCapitalisationType::kCapEnabled);
  ASSERT_NEAR(result.accrued_interest, 9109, 1);
  ASSERT_NEAR(result.deposit_ammount, 109109, 1);
  ASSERT_NEAR(result.tax_ammount, 1302, 1);
}

TEST(DepositCalculatorModel, Actions) {
  s21::DepositCalculatorModel model;
  for (int i = 0; i < 4; ++i) {
    if (i != 3) {
      model.AddAction(i + 1, 10000,
                      s21::DepositCalculatorModel::ActionType::kReplenishment);
    } else {
      model.AddAction(i + 1, 10000,
                      s21::DepositCalculatorModel::ActionType::kWidthDrawal);
    }
  }

  auto result = model.CalculateDeposit(
      100000, 4, 10, 0, 0, 1,
      s21::DepositCalculatorModel::DepositCapitalisationType::kCapDisabled);

  ASSERT_NEAR(result.accrued_interest, 3833, 1);
  ASSERT_NEAR(result.deposit_ammount, 120000, 1);
  ASSERT_NEAR(result.tax_ammount, 0, 1);

  model.ClearActions();

  for (int i = 0; i < 4; ++i) {
    if (i != 3) {
      model.AddAction(i + 1, 10000,
                      s21::DepositCalculatorModel::ActionType::kReplenishment);
    } else {
      model.AddAction(i + 1, 1000000,
                      s21::DepositCalculatorModel::ActionType::kWidthDrawal);
    }
  }

  bool catched = false;
  try {
    result = model.CalculateDeposit(
        100000, 4, 10, 0, 0, 1,
        s21::DepositCalculatorModel::DepositCapitalisationType::kCapDisabled);
  } catch (...) {
    catched = true;
  }
  ASSERT_EQ(catched, true);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
