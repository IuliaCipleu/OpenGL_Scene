// SplineUtils.cpp
#include "SplineUtils.hpp"
#include "Model3D.hpp"

namespace gps {

    glm::vec3 SplineUtils::interpolateSpline(const std::vector<glm::vec3>& controlPoints, float t) {
        // Implement your spline interpolation logic here
        // For simplicity, you can use linear interpolation as an example:
        if (controlPoints.size() < 2) {
            return glm::vec3(0.0f); // Handle insufficient control points
        }

        size_t lastIndex = controlPoints.size() - 1;
        float clampedT = glm::clamp(t, 0.0f, 1.0f);
        float indexFloat = clampedT * lastIndex;
        size_t lowerIndex = static_cast<size_t>(glm::floor(indexFloat));
        size_t upperIndex = glm::min(lowerIndex + 1, lastIndex);

        float alpha = indexFloat - lowerIndex;

        return glm::mix(controlPoints[lowerIndex], controlPoints[upperIndex], alpha);
    }

    //glm::vec3 interpolateAlongRoute(const std::vector<gps::Model3D>& route, float t) {
    //    // Calculate the index of the current segment
    //    float segmentSize = 1.0f / (route.size() - 1);
    //    int segmentIndex = static_cast<int>(t / segmentSize);
    //    if (segmentIndex >= route.size() - 1) {
    //        return route.back().getPosition();  // Return the position of the last model
    //    }

    //    // Calculate the local parameter for the current segment
    //    float localT = (t - segmentIndex * segmentSize) / segmentSize;

    //    // Interpolate between the positions of the current and next models
    //    glm::vec3 position1 = route[segmentIndex].getPosition();
    //    glm::vec3 position2 = route[segmentIndex + 1].getPosition();
    //    glm::vec3 interpolatedPosition = glm::mix(position1, position2, localT);

    //    return interpolatedPosition;
    //}


} // namespace gps
