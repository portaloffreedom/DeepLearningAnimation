#pragma once

#include <animation/engine/terrain.h>

/**
 * @todo write docs
 */
class GodotTerrain : public animation::engine::Terrain
{
public:
    /**
     * Default constructor
     */
    GodotTerrain() {}

    /**
     * @todo write docs
     *
     * @param point TODO
     * @return TODO
     */
    virtual float calculate_slope(Eigen::Vector3f point) const override
    { return 0; }

    /**
     * @todo write docs
     *
     * @param point TODO
     * @return TODO
     */
    virtual float calculate_height(Eigen::Vector3f point) const override
    { return 0; }

};
