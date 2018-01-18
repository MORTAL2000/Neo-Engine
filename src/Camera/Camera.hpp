/* Parent Camera class
 * This camera can be used as a floating camera with no bounds */
#pragma once
#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "glm/glm.hpp"
#include "Toolbox/Toolbox.hpp"

#define LOOK_SPEED 0.005f
#define MOVE_SPEED 30.f

class Camera {
    public:
        /* Position in 3-D world */
        glm::vec3 position;
        /* Position looking at in 3-D world */
        glm::vec3 lookAt;

        /* Constructors */
        Camera(const glm::vec3);
        Camera() : Camera(glm::vec3(0.f)) { }

        /* Update */
        virtual void update();
        virtual void takeMouseInput(const double, const double);

        /* Move according to UVW */
        virtual void moveForward(const float);
        virtual void moveBackward(const float);
        virtual void moveLeft(const float);
        virtual void moveRight(const float);
        virtual void moveUp(const float);
        virtual void moveDown(const float);

    protected:
        /* Used for look at calculation */
        double phi;
        double theta;

        /* UVW basis vectors */
        glm::vec3 u, v, w;

        void updateUVW();
        virtual void updateLookAt();
};

#endif
