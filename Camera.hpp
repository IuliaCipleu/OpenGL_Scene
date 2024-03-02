#ifndef Camera_hpp
#define Camera_hpp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace gps {
    
    enum MOVE_DIRECTION {MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT};
    
    class Camera {

    public:
        //Camera constructor
        Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp);
        //return the view matrix, using the glm::lookAt() function
        glm::mat4 getViewMatrix();
        //update the camera internal parameters following a camera move event
        void move(MOVE_DIRECTION direction, float speed);
        //update the camera internal parameters following a camera rotate event
        //yaw - camera rotation around the y axis
        //pitch - camera rotation around the x axis
        void rotate(float pitch, float yaw);
       // void animateToPosition(glm::vec3 targetPosition, float duration);
       /* glm::vec3 getCameraPosition() const {
            return cameraPosition;
        }*/

        //// set the camera position
        //void setCameraPosition(const glm::vec3& newPosition) {
        //    cameraPosition = newPosition;
        //    updateCameraVectors(); // Update other camera vectors after changing position
        //}
        void setPosition(const glm::vec3& newPosition) {
            cameraPosition = newPosition;
            cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
            cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection));
            cameraUpDirection = glm::cross(cameraRightDirection, cameraFrontDirection);
        }

        glm::vec3 getPosition() {
            return cameraPosition;
        }

        glm::vec3 getFrontDirection() {
            return cameraFrontDirection;
        }
        
    private:
        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget;
        glm::vec3 cameraFrontDirection;
        glm::vec3 cameraRightDirection;
        glm::vec3 cameraUpDirection;
        //void updateCameraVectors() {
        //    //... existing code ...
        //}
    };    
}

#endif /* Camera_hpp */
