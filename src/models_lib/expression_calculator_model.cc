#include "expression_calculator_model.h"

namespace s21 {
long double ExpressionCalculatorModel::CalculateExpression(
    const std::string& expression, long double x = 0) {
  x_value_ = x;
  expression_ = expression;
  token_infix_vector_.clear();
  token_postfix_vector_.clear();
  while (!double_stack_.empty()) double_stack_.pop();
  buffer_.clear();
  last_operator_ = kNull;
  equal_operator_met_ = false;
  left_parentheses_count = 0;
  right_parantheses_count = 0;

  ProcessExpression();
  ConvertToPostfix();
  long double result = CalculateRpn();
  return result;
}

std::pair<std::vector<double>, std::vector<double>>
ExpressionCalculatorModel::CalculateAxesValues(const std::string& expression,
                                               long double x_min,
                                               long double x_max) {
  long double h = (x_max - x_min) / 500;
  std::vector<double> x_axis;
  std::vector<double> y_axis;
  for (long double x = x_min; x <= x_max; x += h) {
    if (fabs(x) <= 1e-7) x = 0;
    x_axis.push_back(x);
    y_axis.push_back(CalculateExpression(expression, x));
  }

  std::pair<std::vector<double>, std::vector<double>> axes_pair(
      std::move(x_axis), std::move(y_axis));
  return axes_pair;
}

void ExpressionCalculatorModel::Token::SetToken(Operator oper, int precendence,
                                                Associativity associativity,
                                                long double value) {
  this->oper = oper;
  this->precendence = precendence;
  this->associativity = associativity;
  this->value = value;
}

void ExpressionCalculatorModel::Token::CharToToken(char c,
                                                   long double x_value) {
  Operator oper = static_cast<Operator>(c);
  switch (oper) {
    case kPlus:
    case kMinus:
      SetToken(oper, 1);
      break;
    case kMult:
    case kDiv:
      SetToken(oper, 2);
      break;
    case kPower:
      SetToken(oper, 3, Associativity::kRight);
      break;
    case kLeftParenthesis:
    case kRightParenthesis:
    case kEqual:
      SetToken(oper, 0, Associativity::kNone);
      break;
    case kX:
      SetToken(Operator::kNum, 0, Associativity::kNone, x_value);
      break;
    case kSpace:
      SetToken(oper, 0, Associativity::kNone);
      break;
    default:
      SetToken(Operator::kNull, 0, Associativity::kNone);
      break;
  }
}

void ExpressionCalculatorModel::Token::StringToToken(
    const std::string& buffer) {
  if (buffer.back() == '.')
    throw std::runtime_error("Invalid function operator");
  try {
    size_t last_converted_char;
    long double value = std::stold(buffer, &last_converted_char);
    if (last_converted_char != buffer.size()) throw std::runtime_error("");
    SetToken(kNum, 0, Associativity::kNone, value);
  } catch (...) {
    if (buffer == "cos") {
      SetToken(Operator::kCos, 4);
    } else if (buffer == "sin") {
      SetToken(Operator::kSin, 4);
    } else if (buffer == "tan") {
      SetToken(Operator::kTan, 4);
    } else if (buffer == "acos") {
      SetToken(Operator::kAcos, 4);
    } else if (buffer == "asin") {
      SetToken(Operator::kAsin, 4);
    } else if (buffer == "atan") {
      SetToken(Operator::kAtan, 4);
    } else if (buffer == "sqrt") {
      SetToken(Operator::kSqrt, 4);
    } else if (buffer == "ln") {
      SetToken(Operator::kLn, 4);
    } else if (buffer == "log") {
      SetToken(Operator::kLog, 4);
    } else if (buffer == "mod") {
      SetToken(Operator::kMod, 2);
    } else {
      throw std::runtime_error("Invalid function operator");
    }
  }
}

bool ExpressionCalculatorModel::CheckOperatorCorrectness(const Operator& oper) {
  switch (oper) {
    case kPlus:
    case kMinus:
      if (last_operator_ != kLeftParenthesis &&
          last_operator_ != kRightParenthesis && last_operator_ != kNum &&
          last_operator_ != kNull)
        return false;
      break;
    case kMult:
    case kDiv:
    case kPower:
    case kRightParenthesis:
    case kEqual:
    case kMod:
      if (oper == kEqual) {
        if (equal_operator_met_ == true)
          return false;
        else
          equal_operator_met_ = true;
      }
      if (last_operator_ != kRightParenthesis && last_operator_ != kNum)
        return false;
      break;
    case kLeftParenthesis:
      if (last_operator_ == kRightParenthesis || last_operator_ == kNum)
        return false;
      break;
    case kCos:
    case kSin:
    case kTan:
    case kAcos:
    case kAsin:
    case kAtan:
    case kSqrt:
    case kLn:
    case kLog:
    case kNum:
      if ((last_operator_ >= kCos && last_operator_ <= kLog) ||
          last_operator_ == kRightParenthesis || last_operator_ == kNum)
        return false;
      break;
    default:
      return false;
      break;
  }
  return true;
}

void ExpressionCalculatorModel::AddToken(Token&& token) {
  if (CheckOperatorCorrectness(token.oper)) {
    if (token.oper != kEqual) {
      last_operator_ = token.oper;
      if (last_operator_ == kRightParenthesis)
        ++right_parantheses_count;
      else if (last_operator_ == kLeftParenthesis)
        ++left_parentheses_count;
      token_infix_vector_.push_back(token);
      buffer_.clear();
    }
  } else {
    throw std::runtime_error("Invalid order of operators");
  }
}

void ExpressionCalculatorModel::ProcessExpression() {
  std::transform(expression_.begin(), expression_.end(), expression_.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  if (expression_.back() != kEqual)
    throw std::runtime_error("No equal operator at the end");
  for (auto c : expression_) {
    Token new_token;
    new_token.CharToToken(c, x_value_);

    if (new_token.oper != kNull &&
        !((new_token.oper == kPlus || new_token.oper == kMinus) &&
          buffer_.back() == kEps)) {
      if (!buffer_.empty()) {
        Token new_string_token;
        new_string_token.StringToToken(buffer_);
        AddToken(std::move(new_string_token));
      }
      if (new_token.oper == kPlus || new_token.oper == kMinus) {
        if (token_infix_vector_.empty() || last_operator_ == kLeftParenthesis) {
          Token new_zero_token;
          new_zero_token.StringToToken("0");
          AddToken(std::move(new_zero_token));
        }
      }
      if (new_token.oper != kSpace) AddToken(std::move(new_token));
    } else {
      buffer_ += c;
    }
  }

  if (left_parentheses_count != right_parantheses_count)
    throw std::runtime_error(
        "The number of left parentheses does not match the number of right "
        "parentheses");
}

void ExpressionCalculatorModel::ConvertToPostfix() {
  std::stack<Token> token_stack;
  for (auto token : token_infix_vector_) {
    switch (token.oper) {
      case kNum:
        token_postfix_vector_.push_back(std::move(token));
        break;
      case kPlus:
      case kMinus:
      case kMult:
      case kDiv:
      case kPower:
      case kMod:
        while (!token_stack.empty() &&
               ((token_stack.top().precendence > token.precendence) ||
                (token_stack.top().precendence == token.precendence &&
                 token.associativity == Associativity::kLeft))) {
          token_postfix_vector_.push_back(token_stack.top());
          token_stack.pop();
        }
        token_stack.push(token);
        break;
      case kRightParenthesis:
        while (!token_stack.empty() &&
               token_stack.top().oper != kLeftParenthesis) {
          token_postfix_vector_.push_back(token_stack.top());
          token_stack.pop();
        }
        if (token_stack.empty()) {
          throw std::runtime_error("Invalid order of operators");
        } else {
          token_stack.pop();
          if (!token_stack.empty() && token_stack.top().oper >= kCos &&
              token_stack.top().oper <= kLog) {
            token_postfix_vector_.push_back(token_stack.top());
            token_stack.pop();
          }
        }
        break;
      default:
        token_stack.push(token);
        break;
    }
  }
  while (!token_stack.empty()) {
    if (token_stack.top().oper == kLeftParenthesis ||
        token_stack.top().oper == kRightParenthesis) {
      throw std::runtime_error("Invalid order of operators");
    } else {
      token_postfix_vector_.push_back(token_stack.top());
      token_stack.pop();
    }
  }
}

void ExpressionCalculatorModel::ExecuteFunctionForCalculation(
    std::function<long double(long double, long double)> function) {
  if (!double_stack_.empty()) {
    long double value_2 = double_stack_.top();
    double_stack_.pop();
    if (!double_stack_.empty()) {
      long double& value_1 = double_stack_.top();
      value_1 = function(value_1, value_2);
    } else {
      throw std::runtime_error("Invalid order of operators");
    }
  } else {
    throw std::runtime_error("Invalid order of operators");
  }
}

void ExpressionCalculatorModel::ExecuteFunctionForCalculation(
    std::function<long double(long double)> function) {
  if (!double_stack_.empty()) {
    long double& value = double_stack_.top();
    value = function(value);
  } else {
    throw std::runtime_error("Invalid order of operators");
  }
}

long double ExpressionCalculatorModel::CalculateRpn() {
  for (auto token : token_postfix_vector_) {
    switch (token.oper) {
      case kNum:
        double_stack_.push(token.value);
        break;
      case kPlus:
        ExecuteFunctionForCalculation(std::plus<long double>());
        break;
      case kMinus:
        ExecuteFunctionForCalculation(std::minus<long double>());
        break;
      case kMult:
        ExecuteFunctionForCalculation(std::multiplies<long double>());
        break;
      case kDiv:
        ExecuteFunctionForCalculation(std::divides<long double>());
        break;
      case kPower:
        ExecuteFunctionForCalculation(powl);
        break;
      case kCos:
        ExecuteFunctionForCalculation(cosl);
        break;
      case kSin:
        ExecuteFunctionForCalculation(sinl);
        break;
      case kTan:
        ExecuteFunctionForCalculation(tanl);
        break;
      case kAcos:
        ExecuteFunctionForCalculation(acosl);
        break;
      case kAsin:
        ExecuteFunctionForCalculation(asinl);
        break;
      case kAtan:
        ExecuteFunctionForCalculation(atanl);
        break;
      case kSqrt:
        ExecuteFunctionForCalculation(sqrtl);
        break;
      case kLn:
        ExecuteFunctionForCalculation(logl);
        break;
      case kLog:
        ExecuteFunctionForCalculation(log10l);
        break;
      case kMod:
        ExecuteFunctionForCalculation(fmodl);
        break;
      default:
        throw std::runtime_error("Invalid order of operators");
        break;
    }
  }
  if (!double_stack_.empty())
    return double_stack_.top();
  else
    throw std::runtime_error("Invalid order of operators");
}
}  // namespace s21
