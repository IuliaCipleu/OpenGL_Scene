// SplineUtils.hpp
#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace gps {

    class SplineUtils {
    public:
        // Interpolate position along spline curve
        static glm::vec3 interpolateSpline(const std::vector<glm::vec3>& controlPoints, float t);
        //glm::vec3 interpolateAlongRoute(const std::vector<gps::Model3D>& route, float t);
    };

} // namespace gps
