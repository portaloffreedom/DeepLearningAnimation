#pragma once

#include <Eigen/Dense>
#include <Vector3.hpp>

// @param in 3d position vector in Right handed Coordinate System
// @param out 3d position vector in Unity Coordinate System
Eigen::Vector3f RHSToUnity(const godot::Vector3 &in)
{
    return Eigen::Vector3f(
            in[1],
            in[2],
           -in[0]
                );
//  out[0] = in[1];
//  out[1] = in[2];
//  out[2] = -in[0];
}

// @param in 3d position vector in Unity Coordinate System
// @param out 3d position vector in Right handed Coordinate System
godot::Vector3 UnityToRHS(const Eigen::Vector3f &in)
{
    return godot::Vector3(
           -in.z(),
            in.x(),
            in.y());
//  out[0] = -in[2];
//  out[1] = in[0];
//  out[2] = in[1];
}
