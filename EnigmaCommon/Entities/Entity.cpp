/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "Entity.hpp"

namespace Enigma
{
    Entity::Entity()
    {
        this->mX=0;
        this->mY=0;
        this->mZ=0;

        this->mTargetX=0;
        this->mTargetY=0;
        this->mTargetZ=0;

        this->mVelocityX=0;
        this->mVelocityY=0;
        this->mVelocityZ=0;

        this->mModelId=0;
    }

    Entity::~Entity()
    {

    }

    Enigma::f32 Entity::GetX() const
    {
        return this->mX;
    }

    void Entity::SetX(Enigma::f32 value)
    {
        this->mX=value;
    }

    Enigma::f32 Entity::GetY() const
    {
        return this->mY;
    }

    void Entity::SetY(Enigma::f32 value)
    {
        this->mY=value;
    }

    Enigma::f32 Entity::GetZ() const
    {
        return this->mZ;
    }

    void Entity::SetZ(Enigma::f32 value)
    {
        this->mZ=value;
    }

    Enigma::f32 Entity::GetTargetX() const
    {
        return this->mTargetX;
    }

    void Entity::SetTargetX(Enigma::f32 value)
    {
        this->mTargetX=value;
    }

    Enigma::f32 Entity::GetTargetY() const
    {
        return this->mTargetY;
    }

    void Entity::SetTargetY(Enigma::f32 value)
    {
        this->mTargetY=value;
    }

    Enigma::f32 Entity::GetTargetZ() const
    {
        return this->mTargetZ;
    }

    void Entity::SetTargetZ(Enigma::f32 value)
    {
        this->mTargetZ=value;
    }

    Enigma::f32 Entity::GetVelocityX() const
    {
        return this->mVelocityX;
    }

    void Entity::SetVelocityX(Enigma::f32 value)
    {
        this->mVelocityX=value;
    }

    Enigma::f32 Entity::GetVelocityY() const
    {
        return this->mVelocityY;
    }

    void Entity::SetVelocityY(Enigma::f32 value)
    {
        this->mVelocityY=value;
    }

    Enigma::f32 Entity::GetVelocityZ() const
    {
        return this->mVelocityZ;
    }

    void Entity::SetVelocityZ(Enigma::f32 value)
    {
        this->mVelocityZ=value;
    }

    size_t Entity::GetModelId() const
    {
        return this->mModelId;
    }

    void Entity::SetModelId(size_t value)
    {
        this->mModelId=value;
    }
};
