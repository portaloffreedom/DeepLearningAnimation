/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2016  Matteo De Carlo <matteo.dek@covolunablu.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CHART_H
#define CHART_H

#include <vector>
#include <QWidget>
#include <QTimer>
#include <QtCharts/QChartGlobal>

#include "animation/controller.h"
#include "graphics/timedgenerator.h"

class QLayout;
class QLabel;

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class Chart : public QWidget
{
    Q_OBJECT
public:
    Chart(QWidget *parent = nullptr);
    virtual ~Chart();

public slots:
    void setUpdateTimerDelay(int delay);
    void add_output_to_chart(const float *const output, float ms_for_nn);

private:
    animation::Controller *controller;

    static const unsigned int MAX_OUTPUT_SIZE;// = 8;
    unsigned int output_size;
    QLineSeries** outputs;
    QChart *chart;
    QLabel *fps_text;
    TimedGenerator *timer;
    const unsigned int history_size;
};

#endif // CHART_H
