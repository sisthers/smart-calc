#ifndef CPP3_SMARTCALC_V2_0_MODEL_H_
#define CPP3_SMARTCALC_V2_0_MODEL_H_

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

namespace s21 {
class ExpressionCalculatorModel {
 public:
  long double CalculateExpression(const std::string& expression, long double x);
  std::pair<std::vector<double>, std::vector<double>> CalculateAxesValues(
      const std::string& expression, long double x_min, long double x_max);

 private:
  enum Operator {
    kPlus = '+',
    kMinus = '-',
    kMult = '*',
    kDiv = '/',
    kPower = '^',
    kLeftParenthesis = '(',
    kRightParenthesis = ')',
    kEqual = '=',
    kX = 'x',
    kSpace = ' ',
    kEps = 'e',
    kCos = 0,
    kSin,
    kTan,
    kAcos,
    kAsin,
    kAtan,
    kSqrt,
    kLn,
    kLog,
    kMod,
    kNum,
    kNull
  };

  enum class Associativity { kLeft, kRight, kNone };

  struct Token {
   public:
    void SetToken(Operator oper, int precendence,
                  Associativity associativity = Associativity::kLeft,
                  long double value = 0);
    void CharToToken(char c, long double x_value);
    void StringToToken(const std::string& buffer);

    Operator oper = kNull;
    uint8_t precendence = 0;
    Associativity associativity = associativity;
    long double value = 0;
  };

  bool CheckOperatorCorrectness(const Operator& oper);
  void AddToken(Token&& token);
  void ProcessExpression();
  void ConvertToPostfix();
  void ExecuteFunctionForCalculation(
      std::function<long double(long double, long double)> function);
  void ExecuteFunctionForCalculation(
      std::function<long double(long double)> function);
  long double CalculateRpn();

  std::string expression_;
  std::vector<Token> token_infix_vector_;
  std::vector<Token> token_postfix_vector_;
  std::stack<long double> double_stack_;
  std::string buffer_;
  Operator last_operator_ = Operator::kNull;
  bool equal_operator_met_ = false;
  size_t left_parentheses_count = 0;
  size_t right_parantheses_count = 0;
  long double x_value_ = 0;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_MODEL_H_
