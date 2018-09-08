#ifndef NN_DEBUG_VIEW_H
#define NN_DEBUG_VIEW_H

#include <QMainWindow>

namespace Ui {
class nn_debug_view;
}

class QHBoxLayout;
class QSpinBox;
class Chart;

class nn_debug_view : public QMainWindow
{
    Q_OBJECT

public:
    explicit nn_debug_view(QWidget *parent = 0);
    ~nn_debug_view();

private:
    QHBoxLayout * init_control_bar();

private:
    Ui::nn_debug_view *ui;
    Chart *m_chart;

    QSpinBox * timerDelaySpinBox_;

};

#endif // NN_DEBUG_VIEW_H
