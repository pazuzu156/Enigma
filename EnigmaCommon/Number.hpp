#ifndef NUMBER_HPP_INCLUDED
#define NUMBER_HPP_INCLUDED

#include "string.hpp"

#include "boost_lexical_cast.hpp"

#include "TypeDefs.hpp"

namespace Enigma
{
    template <class NumberType>
    class DllExport Number
    {
        private:

        NumberType mValue;

        protected:

        public:

        Number()
        {
            this->mValue=0;
        }

        Number(NumberType value)
        {
            this->mValue=value;
        }

        ~Number()
        {

        }

        NumberType GetValue()
        {
            return this->mValue;
        }

        void SetValue(NumberType value)
        {
            this->mValue=value;
        }

        std::string ToString()
        {
            return boost::lexical_cast<std::string>(this->mValue);
        }

        void Parse(const std::string& value)
        {
            this->mValue = boost::lexical_cast<NumberType>(value)
        }

        void Parse(const char* value)
        {
            this->mValue = boost::lexical_cast<NumberType>(value)
        }

        const char& operator +=(const char& value)
        {
            this->mValue+=value;
            return this->mValue;
        }

        const unsigned char& operator +=(const unsigned char& value)
        {
            this->mValue+=value;
            return this->mValue;
        }

        const short& operator +=(const short& value)
        {
            this->mValue+=value;
            return this->mValue;
        }

        const unsigned short& operator +=(const unsigned short& value)
        {
            this->mValue+=value;
            return this->mValue;
        }

        const int& operator +=(const int& value)
        {
            this->mValue+=value;
            return this->mValue;
        }

        const unsigned int& operator +=(const unsigned int& value)
        {
            this->mValue+=value;
            return this->mValue;
        }

        const long& operator +=(const long& value)
        {
            this->mValue+=value;
            return this->mValue;
        }

        const unsigned long& operator +=(const unsigned long& value)
        {
            this->mValue+=value;
            return this->mValue;
        }

        void operator +=(const Number<NumberType>& value)
        {
            this->mValue+=value.GetValue();
            return &this;
        }

    };
};

#endif // NUMBER_HPP_INCLUDED
