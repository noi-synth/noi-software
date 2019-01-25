//
// Created by ddl_blue on 28.10.18.
//

#ifndef NOI_SOFTWARE_CPARAMETER_HPP
#define NOI_SOFTWARE_CPARAMETER_HPP

#include <cstdint>
#include <string>

namespace NLgc {
    class CParameter {
    public:
        enum EType {
            EMPTY, INT, FLOAT, BOOL, STRING
        };

        CParameter() : m_type(EType::EMPTY) {};

        CParameter(uint32_t value);

        CParameter(float value);

        CParameter(bool value);

        CParameter(std::string value);

        ~CParameter() {};

        EType GetType();

        void SetValue(uint32_t value);

        void SetValue(float value);

        void SetValue(bool value);

        void SetValue(std::string value);

        int32_t GetIntValue();

        float GetFloatValue();

        bool GetBoolValue();

        std::string GetStringValue();


    private:
        union ValueUnion {
            int32_t m_int;
            float m_float;
            bool m_bool;
            std::string m_string;

            ValueUnion() : m_string() {};

            ~ValueUnion() {};
        };

        ValueUnion m_value;
        EType m_type;
    };
}


#endif //NOI_SOFTWARE_CPARAMETER_HPP
