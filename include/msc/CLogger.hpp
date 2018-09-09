//
// Created by ddl_blue on 7.4.18.
//

#ifndef NOI_SOFTWARE_CLOGGER_HPP
#define NOI_SOFTWARE_CLOGGER_HPP

#define LOG_LINE_PREFIX "#NOI>"

#include <iostream>
#include <string>


namespace NMsc {
    class CLogger {
    public:
        //TODO make logger more flexible
        //TODO make logger thread safe
        static void Log(const char *format) {
            std::cerr << "#NOI>" << format << std::endl;
        }

        /*----------------------------------------------------------------------*/
        template<typename T, typename... Targs>
        static void Log(const char *format, T value, Targs... Fargs) {
            std::cerr << LOG_LINE_PREFIX;
            Print(format + 1, value, Fargs...);
            std::cerr << std::endl;
        }



    private:
        template<typename T, typename... Targs>
        static void Print(const char *format, T value, Targs... Fargs) {
            for (; *format != '\0'; format++) {
                if (*format == '%') {
                    std::cerr << value;
                    Print(format + 1, Fargs...); // recursive call
                    return;
                }
                std::cerr << *format;
            }
        }

        static void Print(const char *format) {
            std::cerr << format;
        }
    };

    enum LogType {
        TMP_DEBUG,
        NOTE,
        WARNING,
        ERROR,
        RT_ERROR
    };

}
#endif //NOI_SOFTWARE_CLOGGER_HPP
