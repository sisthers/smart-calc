#include <QApplication>

#include "controller.h"
#include "credit_calc_view.h"
#include "main_view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow main_window;
  CreditCalcWindow credit_calc_window;
  DepCalcWindow deposit_calc_window;
  s21::Controller controller(&main_window, &credit_calc_window,
                             &deposit_calc_window);
  controller.Start();
  return a.exec();
}
