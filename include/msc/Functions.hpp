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

        /**
         * Prints a text with embedded values into output stream. Can be used in similar way as printf. Each occurrence of '%' character is replaced with one value.
         * @tparam T Type of first value.
         * @tparam Targs Type of other values
         * @param stream Output stream that the result will be written to
         * @param format The text with '%' characters
         * @param value First value
         * @param Fargs Other values
         */
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

        /**
         * Print text into stream. Used as end of recursion for Print with values.
         * @param stream Output stream that recives the text
         * @param format Text that will be written into the stream
         */
        static void Print(std::ostream &stream, const char *format) {
            stream << format;
        }

        /**
         * Add a number to an Enum value
         * @tparam T Enum type
         * @param first Enum value
         * @param second Number to add
         * @return Addition result
         */
        template<typename T>
        static T EnumAdd(T first, int32_t second) {
            return (T) (((int32_t) (first) + second));
        }

        /**
         * Subtract two Enum values.
         * @tparam T Enum type
         * @param first First value
         * @param second Second value
         * @return Result
         */
        template<typename T>
        static int32_t EnumSub(T first, T second) {
            return (int32_t) (first - second);
        }

        /**
         * Does logical OR of two Enum values
         * @tparam T Enum types
         * @param first First value
         * @param second Second value
         * @return Result
         */
        template<typename T>
        static T EnumOr(T first, T second) {
            return (T) (((uint32_t) first) | ((uint32_t) second));
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
