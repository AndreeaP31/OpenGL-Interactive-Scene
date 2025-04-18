#include "Camera.hpp"

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpPrim = -cameraUp;
        cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        cameraRightDirection = -glm::normalize(glm::cross(cameraFrontDirection, cameraUpPrim));
        cameraUpDirection = glm::cross(cameraRightDirection, cameraFrontDirection);
    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        //TODO
       // return glm::lookAt(cameraPosition, cameraTarget, cameraUp);
        return glm::lookAt(this->cameraPosition, this->cameraFrontDirection + this->cameraPosition, this->cameraUpDirection);
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        if (direction == MOVE_FORWARD) {
            cameraPosition += cameraFrontDirection * speed;
            cameraTarget = cameraPosition + cameraFrontDirection;
        }
        if (direction == MOVE_BACKWARD) {
            cameraPosition -= cameraFrontDirection * speed;
            cameraTarget = cameraPosition + cameraFrontDirection;
        }
        if (direction == MOVE_RIGHT) {
            cameraPosition += cameraRightDirection * speed;
            cameraTarget = cameraPosition + cameraRightDirection;
        }
        if (direction == MOVE_LEFT) {
            cameraPosition -= cameraRightDirection * speed;
            cameraTarget = cameraPosition + cameraRightDirection;
        }
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        cameraFrontDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFrontDirection.y = sin(glm::radians(pitch));
        cameraFrontDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFrontDirection = glm::normalize(cameraFrontDirection);
        cameraRightDirection = -glm::normalize(glm::cross(cameraFrontDirection, cameraUpPrim));
        cameraUpDirection = glm::cross(cameraRightDirection, cameraFrontDirection);
    }

    // Set camera target
    void Camera::setTarget(glm::vec3 newTarget) {
        cameraTarget = newTarget;
        cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        cameraRightDirection = -glm::normalize(glm::cross(cameraFrontDirection, cameraUpPrim));
        cameraUpDirection = glm::cross(cameraRightDirection, cameraFrontDirection);
    }

    // Set camera position
    void Camera::setPosition(glm::vec3 newPosition) {
        cameraPosition = newPosition;
        cameraTarget = cameraPosition + cameraFrontDirection;
    }

}