#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED

/*
Copyright © 2009  Christopher Joseph Dean Schaefer (disks86)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source distribution.
*/

#include "Object.hpp"
#include "../StorableObject.hpp"

namespace Enigma
{
	class DllExport Entity : public Object, public StorableObject
    {
        private:

        Enigma::f32 mX;
        Enigma::f32 mY;
        Enigma::f32 mZ;

        Enigma::f32 mTargetX;
        Enigma::f32 mTargetY;
        Enigma::f32 mTargetZ;

        Enigma::f32 mVelocityX;
        Enigma::f32 mVelocityY;
        Enigma::f32 mVelocityZ;

        size_t mModelId;

        protected:

        public:

        Entity();
        ~Entity();

        Enigma::f32 GetX() const;
        void SetX(Enigma::f32 value);

        Enigma::f32 GetY() const;
        void SetY(Enigma::f32 value);

        Enigma::f32 GetZ() const;
        void SetZ(Enigma::f32 value);

        Enigma::f32 GetTargetX() const;
        void SetTargetX(Enigma::f32 value);

        Enigma::f32 GetTargetY() const;
        void SetTargetY(Enigma::f32 value);

        Enigma::f32 GetTargetZ() const;
        void SetTargetZ(Enigma::f32 value);

        Enigma::f32 GetVelocityX() const;
        void SetVelocityX(Enigma::f32 value);

        Enigma::f32 GetVelocityY() const;
        void SetVelocityY(Enigma::f32 value);

        Enigma::f32 GetVelocityZ() const;
        void SetVelocityZ(Enigma::f32 value);

        size_t GetModelId() const;
        void SetModelId(size_t value);
    };
};

#endif // ENTITY_HPP_INCLUDED