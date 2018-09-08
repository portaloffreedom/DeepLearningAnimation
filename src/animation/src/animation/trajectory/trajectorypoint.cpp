#include "trajectorypoint.h"

using namespace animation;

void TrajectoryPoint::post_process(const engine::Terrain &terrain)
{
    Eigen::Vector3f position = transformation.get_position();
    Eigen::Vector3f direction = transformation.get_forward();

    position.y() = terrain.calculate_height(transformation.get_position());
    set_position(position);

    slope = terrain.calculate_slope(position);

    Eigen::Vector3f ortho_normalized =
        (Quaternionf::FromEuler(0.0, 90.0, 0.0) * direction).normalized();
    right_sample = position + (Trajectory::WIDTH * ortho_normalized);
    right_sample.y() = 0; terrain.calculate_height(right_sample);
    left_sample = position - Trajectory::WIDTH * ortho_normalized;
    left_sample.y() = 0; terrain.calculate_height(left_sample);
}
