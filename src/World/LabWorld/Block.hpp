#pragma once
#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include "Entity/Entity.hpp"
#include "AABB/AABB.hpp"

class Block : public Entity {
    public:
        Block(Mesh *, ModelTexture, const glm::vec3, const glm::vec3, const glm::vec3, float);
        
        AABB boundingBox;
        bool isHit;
        float velocity;
        
        /* Define model textures for dead and alive game objects*/

        void update(Entity *, AABB);
};

#endif