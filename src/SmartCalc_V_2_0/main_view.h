#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  QString GetExpression() const;
  void SetExpression(const QString &expression);
  void BuildGraph(QVector<double> &&x_axis, QVector<double> &&y_axis);
  double GetXMinValue() const;
  double GetXMaxValue() const;
  double GetXValue() const;

 signals:
  void CreditCalcOpenSignal();
  void DepositCalcOpenSignal();
  void CalculateSignal();
  void ClearSingal();
  void NewInputSignal(QString input);
  void BuildSignal();

 private slots:
  void CreditCalcClicked();
  void DepositCalcClicked();
  void CalculateButtonClicked();
  void InputButtonClicked();
  void ClearButtonClicked();
  void BuildButtonClicked();

 private:
  Ui::MainWindow *ui;
};
#endif  // MAINWINDOW_H
