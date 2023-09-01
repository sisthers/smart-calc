#ifndef CREDITCALCWINDOW_H
#define CREDITCALCWINDOW_H

#include <QMainWindow>

namespace Ui {
class CreditCalcWindow;
}

class CreditCalcWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit CreditCalcWindow(QWidget *parent = nullptr);
  ~CreditCalcWindow();

  double GetCreditAmmount() const;
  double GetTerm() const;
  double GetInterestRate() const;
  int GetSelectedCreditType() const;
  void SetData(double first_payment, double last_payment, double total_payment,
               double overpayment);

 signals:
  void CalculateSignal();
  void CloseSignal();

 private slots:
  void CalculateClicked();
  void CloseClicked();

 private:
  Ui::CreditCalcWindow *ui;
};

#endif  // CREDITCALCWINDOW_H
