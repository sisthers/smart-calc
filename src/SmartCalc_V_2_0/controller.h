#include <QObject>
#include <QString>
#include <QVector>
#include <cstring>

#include "../models_lib/credit_calculator_model.h"
#include "../models_lib/deposit_calculator_model.h"
#include "../models_lib/expression_calculator_model.h"
#include "credit_calc_view.h"
#include "deposit_calc_view.h"
#include "main_view.h"

namespace s21 {
class Controller : public QObject {
  Q_OBJECT
 public:
  explicit Controller(MainWindow* main_window,
                      CreditCalcWindow* credit_calc_window,
                      DepCalcWindow* deposit_calc_window);
  void Start();

 private slots:
  void OpenCreditCalc();
  void CloseCreditCalc();
  void OpenDepositCalc();
  void CloseDepositCalc();
  void NewInputFromButtons(QString input);
  void Clear();
  void AddDepositAction();
  void ClearDepositActions();
  void ExecuteExpressionCalculation();
  void ExecuteCreditCalculation();
  void ExecuteDepositCalculation();
  void BuildGraph();

 private:
  s21::CreditCalculatorModel credit_model_;
  s21::DepositCalculatorModel deposit_model_;
  s21::ExpressionCalculatorModel expression_model_;
  MainWindow* main_window_;
  CreditCalcWindow* credit_calc_window_;
  DepCalcWindow* deposit_calc_window_;
};
}  // namespace s21
