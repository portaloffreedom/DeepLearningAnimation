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

#include "chart.h"
#include <algorithm>
#include <iomanip>
#include <memory>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDoubleSpinBox>
#include <QtCharts/QValueAxis>

#include "animation/keyboard_controller.h"
#include "fakeskeleton.h"
#include "faketerrain.h"
#include "animation/nn/models/pfnnmodel.h"

class FakeTerrain;
class FakeSkeleton;
QT_CHARTS_USE_NAMESPACE

const unsigned int Chart::MAX_OUTPUT_SIZE = 8;

bool isFixedPitch(const QFont & font) {
    const QFontInfo fi(font);
    //qDebug() << fi.family() << fi.fixedPitch();
    return fi.fixedPitch();
}

QFont getMonospaceFont(){
    QFont font("monospace");
    if (isFixedPitch(font)) return font;
    font.setStyleHint(QFont::Monospace);
    if (isFixedPitch(font)) return font;
    font.setStyleHint(QFont::TypeWriter);
    if (isFixedPitch(font)) return font;
    font.setFamily("courier");
    if (isFixedPitch(font)) return font;
    return font;
}

QValueAxis *createXAxis(const char* name, unsigned int history = 100)
{
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, history);
    axisX->setLabelFormat("%g");
    axisX->setTitleText(name);

    return axisX;
}

QValueAxis *createYAxis(const char* name, qreal min = -1, qreal max = 1)
{
    QValueAxis *axisY = new QValueAxis;

    axisY = new QValueAxis;
    axisY->setRange(min, max);
    axisY->setTitleText(name);

    return axisY;
}

Chart::Chart(QWidget *parent)
    : QWidget(parent)
    , controller(nullptr)
    , output_size(0)
    , chart(nullptr)
    , fps_text(nullptr)
    , timer(nullptr)
    , history_size(100)
{
    const unsigned int timer_wait = 30; //ms
    const unsigned int style_count = 4; //TODO This is the value for Adam

    std::unique_ptr<animation::engine::Skeleton> skeleton(
        new FakeSkeleton());
    std::shared_ptr<animation::engine::Terrain> terrain(
        new FakeTerrain());

    auto pfnn_model =
        new animation::nn::PFNNModel(468, 512, 376, 4, 10, 375);
    pfnn_model->load_parameters( ASSETS_FOLDER "/adam/nn/" );

    std::shared_ptr<animation::nn::Model> model(pfnn_model);


    this->controller = new animation::Controller(
        skeleton,
        terrain,
        model
    );
    std::unique_ptr<animation::KeyboardController> key_controller(new animation::KeyboardController(style_count));
    this->timer = new TimedGenerator(timer_wait,
                                     controller,
                                     key_controller,
                                     this);

    this->chart = new QChart;
    QChartView *chartView = new QChartView(this->chart, this);
    this->chart->setMinimumSize(400,400);

    output_size = std::min(this->controller->get_output_dim(), MAX_OUTPUT_SIZE);
    this->outputs = new QLineSeries*[output_size];
    for (int i=0; i<output_size; i++) {
        auto line_series = new QLineSeries;
        chart->addSeries(line_series);
        this->outputs[i] = line_series;
    }

    QValueAxis *axisX;
    QValueAxis *axisY;
//     QValueAxis *axisY_phi;

    axisX = createXAxis("Samples", history_size);
    axisY = createYAxis("Value level", 0.0, 1.0);
//     axisY_phi = createYAxis("Phi",0, 100000);

    chart->legend()->hide();
    chart->setTitle("Data from the neural network");
//     axisY->setLinePenColor(this->outputs[0]->pen().color());
//     axisY_phi->setLinePenColor(this->outputs[0]->pen().color());

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
//     chart->addAxis(axisY_phi, Qt::AlignRight);

    for (int i=0; i<output_size; i++) {
        this->outputs[i]->attachAxis(axisX);
        this->outputs[i]->attachAxis(axisY);
    }

    connect(timer, &TimedGenerator::neuron_output, this, &Chart::add_output_to_chart);

    // Add fps text
    fps_text = new QLabel("fps", this);
    fps_text->setFont(getMonospaceFont());

    // Layout
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(chartView, 0, 0);
    mainLayout->addWidget(fps_text, 1, 0);

    this->setLayout(mainLayout);

    timer->start();
}

Chart::~Chart()
{
    for (int i=0; i<output_size; i++) {
        delete outputs[i];
    }
    delete[] outputs;
    delete controller;
}

void Chart::setUpdateTimerDelay(int delay)
{
    timer->setUpdateTimerDelay(delay);
}

void Chart::add_output_to_chart(const float *const output, float ms_for_nn)
{
    for (int i=0; i<output_size; i++) {
        auto point = output[i];
        auto series = this->outputs[i];
        QVector<QPointF> oldPoints = series->pointsVector();
        QVector<QPointF> points;

        if (oldPoints.count() < this->history_size) {
            points = series->pointsVector();
        } else {
            for (int i = 1; i < oldPoints.count(); i++)
                points.append(QPointF(i - 1, oldPoints.at(i).y()));
        }

        qint64 size = points.count();
        points.append(QPointF(size, point));

        series->replace(points);
    }

    std::stringstream text;
    text << "NN: " << std::fixed << std::setprecision(2) << std::setw(6) << ms_for_nn << "ms";
    fps_text->setText(text.str().c_str());
}
