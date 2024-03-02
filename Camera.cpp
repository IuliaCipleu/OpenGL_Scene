#include "Camera.hpp"
#include <GLFW/glfw3.h>

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        //TODO
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        this->cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUp));
        this->cameraUpDirection = glm::cross(cameraRightDirection, cameraFrontDirection);
    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        //TODO
        return glm::lookAt(cameraPosition, cameraPosition + cameraFrontDirection, cameraUpDirection);
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        //TODO
        if (direction == MOVE_FORWARD) {
            cameraPosition += cameraFrontDirection * speed;
        }
        else if (direction == MOVE_BACKWARD) {
            cameraPosition -= cameraFrontDirection * speed;
        }
        else if (direction == MOVE_RIGHT) {
            cameraPosition += cameraRightDirection * speed;
        }
        else if (direction == MOVE_LEFT) {
            cameraPosition -= cameraRightDirection * speed;
        }
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        //TODO
        cameraFrontDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFrontDirection.y = sin(glm::radians(pitch));
        cameraFrontDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFrontDirection = glm::normalize(cameraFrontDirection);

        // Update right and up vectors
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection));
        //cameraUpDirection = glm::normalize(glm::cross(cameraRightDirection, cameraFrontDirection));
    }

    //void Camera::animateToPosition(glm::vec3 targetPosition, float duration) {
    //    glm::vec3 startPosition = cameraPosition;
    //    double startTime = glfwGetTime();  // Use double for time

    //    while (true) {
    //        double currentTime = glfwGetTime();
    //        float t = static_cast<float>((currentTime - startTime) / duration);

    //        if (t >= 1.0f) {
    //            // Animation finished
    //            cameraPosition = targetPosition;
    //            break;
    //        }

    //        // Interpolate the camera position
    //        cameraPosition = glm::mix(startPosition, targetPosition, t);
    //    }
    //}
}