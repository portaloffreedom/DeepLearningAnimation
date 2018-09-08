#ifndef ANIMATION_ENGINE_TERRAIN_H
#define ANIMATION_ENGINE_TERRAIN_H

#include <Eigen/Dense>

namespace animation {
namespace engine {

/**
 * @todo write docs
 */
class Terrain
{
public:
    virtual float calculate_height(Eigen::Vector3f point) const = 0;
    virtual float calculate_slope(Eigen::Vector3f point) const = 0;
};

}
}

#endif // ANIMATION_ENGINE_TERRAIN_H
