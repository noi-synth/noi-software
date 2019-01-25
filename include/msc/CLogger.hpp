//
// Created by ddl_blue on 7.4.18.
//

#ifndef NOI_SOFTWARE_CLOGGER_HPP
#define NOI_SOFTWARE_CLOGGER_HPP

#define LOG_LINE_PREFIX "#NOI>"

#include <iostream>
#include <string>
#include "Functions.hpp"

static const bool COLOR_OUTPUT = true;

// Prefixes and colors for log messages. Indexes depends on NMSc::ELogType.
static const std::string LINE_PREFIXES[] = {"#TMP> ", "#NOT> ", "#WRN> ", "#ERR> ", "#RTE> ", "#RTW> "};
static const std::string TYPE_COLORS[] = {"\u001b[32m", "\u001b[36m", "\u001b[33m", "\u001b[31m", "\u001b[35m",
                                          "\u001b[33;1m"};
static const std::string TYPE_COLOR_RESET = "\u001b[0m";

namespace NMsc {
    enum ELogType {
        // Numbers are important for log message prefix and color tables.
                TMP_DEBUG = 0,
        NOTE = 1,
        WARNING = 2,
        ERROR = 3,
        RT_ERROR = 4,
        RT_WARNING = 5
    };


    class CLogger {
    public:
        //TODO make logger more flexible
        //TODO make logger thread safe
        static void Log(ELogType type, const char *format) {
            if (COLOR_OUTPUT)
                std::cerr << TYPE_COLORS[type] << LINE_PREFIXES[type] << format << TYPE_COLOR_RESET << std::endl;
            else
                std::cerr << LINE_PREFIXES << format << std::endl;
        }

        /*----------------------------------------------------------------------*/
        template<typename T, typename... Targs>
        static void Log(ELogType type, const char *format, T value, Targs... Fargs) {
            if (COLOR_OUTPUT)
                std::cerr << TYPE_COLORS[type] << LINE_PREFIXES[type];
            else
                std::cerr << LINE_PREFIXES[type];

            NMsc::Functions::Print(std::cerr, format, value, Fargs...);

            if (COLOR_OUTPUT)
                std::cerr << TYPE_COLORS[type] << std::endl;
            else
                std::cerr << std::endl;
        }



    private:
        /*template<typename T, typename... Targs>
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
        }*/
    };


}
#endif //NOI_SOFTWARE_CLOGGER_HPP
