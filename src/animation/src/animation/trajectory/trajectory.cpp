#include "trajectory.h"

using namespace animation;

const float Trajectory::WIDTH = 0.5f;

const int Trajectory::FRAMERATE = 60;
const int Trajectory::POINTS = 111;
const int Trajectory::POINT_SAMPLES = 12;
const int Trajectory::PAST_POINTS = 60;
const int Trajectory::FUTURE_POINTS = 50;
const int Trajectory::ROOT_POINT_INDEX = 60;
const int Trajectory::POINT_DENSITY = 10;

Trajectory::Trajectory(unsigned int dim, unsigned int n_styles)
 : _points()
 , root_point_index(static_cast<unsigned int>((dim / 1.85)+0.5))
 , n_styles(n_styles)
{
    _points.reserve(dim);
    for (int i=0; i<dim; i++) {
        _points.emplace_back(i, n_styles);
    }
}


const float Trajectory::get_lenght(int start, int end, int step) const
{
    float length = 0;
    for (int i=0; i<end-step; i+= step) {
        Eigen::Vector3f a = _points[i].get_position();
        Eigen::Vector3f b = _points[i+step].get_position();
        length += (b - a).norm();
    }
    return length;
}

void Trajectory::post_process(const engine::Terrain &terrain)
{
    for (Point &p: _points) {
        p.post_process(terrain);
    }
}

void Trajectory::update_trajectory(const Eigen::Vector3f &root_motion, float rest, float framerate) {
    Point &root_point = _points[root_point_index];

    //TODO initialize root_point_index correctly
    //TODO recycle value from previous function
    Matrix4f current_root = root_point.get_transformation(); //copy matrix
    current_root(1,3) = 0.0; //TODO this is flat terrain hardcoded shit

    //Update Past Trajectory
    //TODO simply shift points in a list, why all of these copies???
    for(unsigned int i=0; i<root_point_index; i++) {
        Point& point = _points[i];
        Point& next_point = _points[i+1];
        point.set_position(next_point.get_position());
        point.set_direction(next_point.get_direction());
        point.set_velocity(next_point.get_velocity());
        point.set_speed(next_point.get_speed());
        for(unsigned int j=0; j<this->n_styles; j++) {
            point.style(j) = next_point.get_style(j);
        }
    }

    // Update Root
    float rotational_offset = 0;
    Vector3f translational_offset(rest * Vector3f(root_motion.x(), 0, root_motion.z()));
    rotational_offset = rest * root_motion.y();

    root_point.set_position(translational_offset.get_relative_position_from(current_root));
    root_point.set_direction(Quaternionf::AngleAxis(rotational_offset, Vector3f::Up()) * root_point.get_direction());
    root_point.set_velocity(translational_offset.get_relative_direction_from(current_root) * framerate);
    Eigen::Matrix4f next_root = root_point.get_transformation();
    next_root(1,3) = 0.0; //TODO this is flat terrain hardcoded shit

    // Update Future Trajectory
    const auto points_n = _points.size();
    for(int i=root_point_index+1; i<points_n; i++) {
        assert(i < _points.size());
        Point& point = _points[i];
        point.set_position(
                point.get_position()
                + rest*translational_offset.get_relative_direction_from(next_root)
        );
        point.set_direction(
                Quaternionf::AngleAxis(rotational_offset, Vector3f::Up())
                * point.get_direction()
        );
        point.set_velocity(
                point.get_velocity()
                + translational_offset.get_relative_direction_from(next_root) * framerate
        );
    }
}

Trajectory::Samples Trajectory::samples(unsigned int samples) const {
    return Trajectory::Samples(this, samples);
}

Trajectory::Samples::Samples(const animation::Trajectory *trajectory, unsigned int sample_n)
        : trajectory(trajectory)
        , _size(sample_n)
{
    //TODO improve once we have our trained data (sample_jump should be float)

    // 9.25 = 111 / 12
    sample_jump = trajectory->_points.size() / _size;
    // 108 = 12 * 9
    auto covered = _size * sample_jump;
    if (covered < unfiltered_size()) {
        // 9++ -> 10
        sample_jump++;
    }
}

Trajectory::Samples::const_iterator Trajectory::Samples::cbegin() {
    return Trajectory::Samples::const_iterator(this);
}

Trajectory::Samples::const_iterator Trajectory::Samples::cend() {
    return Trajectory::Samples::const_iterator(this, _size);
}

Trajectory::Samples::const_iterator::const_iterator(const Samples *samples,
                                                    unsigned long index)
    : samples(samples)
    , index(index)
{}

Trajectory::Samples::const_iterator::const_iterator(const Trajectory::Samples::const_iterator &other)
    : Trajectory::Samples::const_iterator::const_iterator(other.samples, other.index)
{}

Trajectory::Samples::const_iterator::operator bool() const
{
    return index < samples->size();
}

bool Trajectory::Samples::const_iterator::operator!=(const Trajectory::Samples::const_iterator &other) const
{
    return index != other.index;
}

Trajectory::Samples::const_iterator &Trajectory::Samples::const_iterator::operator++()
{
    index++;
    return *this;
}

const TrajectoryPoint &Trajectory::Samples::const_iterator::operator*() const
{
    auto i = std::clamp(index * samples->sample_jump, 0lu, samples->unfiltered_size()-1);
    return samples->trajectory->_points[i];
}

const TrajectoryPoint *Trajectory::Samples::const_iterator::operator->() const
{
    return &operator*();
}
