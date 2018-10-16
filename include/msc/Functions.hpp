//
// Created by ddl_blue on 7.10.18.
//

#ifndef NOI_SOFTWARE_FUNCTIONS_HPP
#define NOI_SOFTWARE_FUNCTIONS_HPP

#include <string>
#include <sstream>

namespace NMsc {

/***
 * This class contains various functions for general use.
 */
    class Functions {
    public:

        template<typename T, typename... Targs>
        static void Print(std::ostream &stream, const char *format, T value, Targs... Fargs) {
            for (; *format; format++) {
                if (*format == '%') {
                    stream << value;
                    Print(stream, format + 1, Fargs...); // recursive call
                    return;
                }
                stream << *format;
            }
        }

        static void Print(std::ostream &stream, const char *format) {
            stream << format;
        }


        /*std::string FormatToString(const char * format){
            return format;
        }

        template<typename T, typename... Targs>
        std::string FormatToString(const char * format, T value, Targs... Fargs){
            std::ostringstream stream;
            Print(stream, format, value, Fargs...);
            return stream.str();
        }*/

    private:

    };
}

#endif //NOI_SOFTWARE_FUNCTIONS_HPP