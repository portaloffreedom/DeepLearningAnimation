#ifndef FAKETERRAIN_H
#define FAKETERRAIN_H

#include <animation/engine/terrain.h>

/**
 * @todo write docs
 */
class FakeTerrain : public animation::engine::Terrain
{
public:
    /**
     * Default constructor
     */
    FakeTerrain() {}

    /**
     * @todo write docs
     *
     * @param point TODO
     * @return TODO
     */
    virtual float calculate_slope(Eigen::Vector3f point) const override;

    /**
     * @todo write docs
     *
     * @param point TODO
     * @return TODO
     */
    virtual float calculate_height(Eigen::Vector3f point) const override;

};

#endif // FAKETERRAIN_H
