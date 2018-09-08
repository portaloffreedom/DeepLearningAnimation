#ifndef ANIMATION_TRAJECTORY_H
#define ANIMATION_TRAJECTORY_H

#include <iterator>
#include <vector>
#include <cmath>
#include <algorithm>
#include <Eigen/Dense>
#include "trajectorypoint.h"
#include "animation/engine/terrain.h"
#include "math/vector3f.h"


#include "math/quaternionf.h"
#include "animation/trajectory/trajectorypoint.h"
namespace animation {
class TrajectoryPoint;

/**
 * @todo write docs
 */
class Trajectory
{
public:
    friend class TrajectoryPoint;

    Trajectory(unsigned int dim, unsigned int n_styles);
    ~Trajectory() {}

    typedef TrajectoryPoint Point;

    const std::vector<Point>& points() const { return _points; }
//     std::vector<Point>& points() { return _points; }

    const Point & get_point(unsigned int index) const
    {
        assert(index < _points.size());

        return _points[index];
    }

    const Point& first() const
    { return _points.front(); }

    const Point& last() const
    { return _points.back(); }

    const Point& root_point(int offset = 0) const
    {
        unsigned int index = root_point_index + offset;
        assert(index < _points.size());
        return _points[index];
    }

    const unsigned int get_root_point_index() const
    {
        return root_point_index;
    }

    const float get_lenght() const
    {
        return get_lenght(0, _points.size()+1, 1);
    }

    const float get_lenght(int start, int end, int step) const;

    const float get_curvature(int start, int end, int step) const
    {
        throw std::runtime_error( "Trajectory::get_curvature() is unsupported" );
        float curvature = 0;
        for(int i=step; i<end-step; i+=step) {
//             curvature += Vector3.SignedAngle(Points[i].GetPosition() - Points[i-step].GetPosition(), Points[i+step].GetPosition() - Points[i].GetPosition(), Vector3.up);
        }
        curvature = std::abs(curvature);
        curvature = std::clamp(curvature / 180.0, 0.0, 1.0);
        return curvature;
    }

    void post_process(const engine::Terrain &terrain);

//     virtual void draw() = 0;

    void update_trajectory(const Eigen::Vector3f &root_motion,
                           float rest,
                           float framerate);

    class Samples {
    public:
        Samples(const Trajectory *trajectory, unsigned int sample_jump);

        class const_iterator {
        public:
            const_iterator(const Samples *samples, unsigned long index = 0);
            const_iterator(const const_iterator &other);
            explicit operator bool() const;
            bool operator!=(const const_iterator &other) const;
            const_iterator& operator++(); //++iter
            const TrajectoryPoint& operator*() const;
            const TrajectoryPoint* operator->() const;
        private:
            const Samples *samples;
            unsigned long index;
        };

    public:
        const_iterator cbegin();
        const_iterator cend();

        unsigned long size() const { return _size; }
        unsigned long unfiltered_size() const { return trajectory->_points.size(); }
        unsigned int sample_jump;
    private:
        const Trajectory *trajectory;
        const unsigned long _size;

    };

    Samples samples(unsigned int samples) const;

    static const float WIDTH;// = 0.5f;

public:
    //Trajectory for 60 Hz framerate
    static const int FRAMERATE;// = 60;
    static const int POINTS;// = 111;
    static const int POINT_SAMPLES;// = 12;
    static const int PAST_POINTS;// = 60;
    static const int FUTURE_POINTS;// = 50;
    static const int ROOT_POINT_INDEX;// = 60;
    static const int POINT_DENSITY;// = 10;
    
private:
    std::vector<Point> _points;
    
    unsigned int root_point_index;
    unsigned int n_styles;
};

}

#endif // ANIMATION_TRAJECTORY_H

