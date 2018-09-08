#ifndef TIMEDGENERATOR_H
#define TIMEDGENERATOR_H

#include <QObject>
#include <QTimer>
#include <chrono>
#include <memory>
#include "animation/keyboard_controller.h"
#include "animation/controller.h"
#include "animation/trajectory/trajectory.h"

/**
 * @todo write docs
 */
class TimedGenerator : public QObject
{
    Q_OBJECT
public:
    TimedGenerator(unsigned int timer_wait,
                   animation::Controller* anim_controller,
                   std::unique_ptr<animation::KeyboardController> &controller,
                   QObject *parent = nullptr);
    virtual ~TimedGenerator();

    void start();
    void setUpdateTimerDelay(int delay);

signals:
    void neuron_output(const float* output, float ms_for_nn);

private:
    void run_cycle();
    void set_inputs();
    float update_network();
    const float* get_outputs() const;
    const animation::TrajectoryPoint& get_sample(unsigned int index) const;

// Class Variables
private:
    static unsigned const TRAJECTORY_DIM_IN = 6;
    static unsigned const TRAJECTORY_DIM_OUT = 6;
    static unsigned const JOINT_DIM_IN = 12;
    static unsigned const JOINT_DIM_OUT = 12;

    //Trajectory for 60 Hz framerate
    const int framerate = 60;
    const int points = 111;
    const int point_samples = 12;
    const int past_points = 60;
    const int future_points = 50;
    const int root_point_index = 60;
    const int point_density = 10;

    QTimer *timer;
    unsigned int interval;

    animation::Controller* anim_controller;
    std::unique_ptr<animation::Trajectory> trajectory;
    std::unique_ptr<animation::KeyboardController> key_controller;

    // Delta time calculation required bits
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;

    // input/output dimensions
    unsigned const trajectory_dim_in;
    unsigned const trajectory_dim_out;
    unsigned const joint_dim_in;
    unsigned const joint_dim_out;
};

#endif // TIMEDGENERATOR_H
