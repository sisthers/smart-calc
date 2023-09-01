#ifndef DEPCALCWINDOW_H
#define DEPCALCWINDOW_H

#include <QListWidgetItem>
#include <QMainWindow>

namespace Ui {
class DepCalcWindow;
}

class DepCalcWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit DepCalcWindow(QWidget *parent = nullptr);
  ~DepCalcWindow();

  double GetDepositAmmount() const;
  double GetTerm() const;
  double GetInterestRate() const;
  double GetTaxRate() const;
  double GetTaxRateForRateAbove() const;
  double GetPeriodicy() const;
  int GetSelectedCapitalization() const;
  double GetMonth() const;
  int GetSelectedActionType() const;
  double GetAmmount() const;
  void SetData(double accurred_interest, double deposit_ammount,
               double tax_ammount);
  void SetError(const QString &error);
  void AddToActionList(QListWidgetItem *item);
  void ClearActionList();

 signals:
  void CalculateSignal();
  void CloseSignal();
  void ClearSignal();
  void AddActionSignal();

 private slots:
  void CalculateClicked();
  void CloseClicked();
  void ClearClicked();
  void AddActionClicked();

 private:
  Ui::DepCalcWindow *ui;
};

#endif  // DEPCALCWINDOW_H
