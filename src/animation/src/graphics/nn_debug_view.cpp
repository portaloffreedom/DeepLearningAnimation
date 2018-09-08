#include "nn_debug_view.h"
#include "ui_nn_debug_view.h"
#include "chart.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>

nn_debug_view::nn_debug_view(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::nn_debug_view)
{
    ui->setupUi(this);
    m_chart = new Chart(this);
    QWidget *centralWidget = new QWidget(this);


    QVBoxLayout *mainLayout = new QVBoxLayout;
    centralWidget->setLayout(mainLayout);

    QHBoxLayout * controlsLayout = init_control_bar();
    mainLayout->addLayout(controlsLayout);

    mainLayout->addWidget(m_chart);

    this->setCentralWidget(centralWidget);
}

nn_debug_view::~nn_debug_view()
{
    delete ui;
}

QHBoxLayout * nn_debug_view::init_control_bar()
{
    QHBoxLayout *controlsLayout = new QHBoxLayout;

    timerDelaySpinBox_ = new QSpinBox(this);
    controlsLayout->addWidget(timerDelaySpinBox_);
    QObject::connect(timerDelaySpinBox_, SIGNAL(valueChanged(int)), m_chart, SLOT(setUpdateTimerDelay(int)));
    timerDelaySpinBox_->setValue(30);

    return controlsLayout;
}
