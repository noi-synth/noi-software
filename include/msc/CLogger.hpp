//
// Created by ddl_blue on 7.4.18.
//

#ifndef NOI_SOFTWARE_CLOGGER_HPP
#define NOI_SOFTWARE_CLOGGER_HPP

#define LOG_LINE_PREFIX "#NOI>"

#include <iostream>
#include <string>
#include "Functions.hpp"

/**
 * Enables color highlight in log output
 */
static const bool COLOR_OUTPUT = true;

// Prefixes and colors for log messages. Indexes depends on NMSc::ELogType.
/**
 * Prefixes for each message type
 */
static const std::string LINE_PREFIXES[] = {"#TMP> ", "#NOT> ", "#WRN> ", "#ERR> ", "#RTE> ", "#RTW> "};
/**
 * Colors for each message type
 */
static const std::string TYPE_COLORS[] = {"\u001b[32m", "\u001b[36m", "\u001b[33m", "\u001b[31m", "\u001b[35m",
                                          "\u001b[33;1m"};
/**
 * ANSI sequence that resets colors to normal
 */
static const std::string TYPE_COLOR_RESET = "\u001b[0m";

namespace NMsc {
    /**
     * Log message type.
     */
    enum ELogType {
        // Numbers are important for log message prefix and color tables.
                TMP_DEBUG = 0,
        NOTE = 1,
        WARNING = 2,
        ERROR = 3,
        RT_ERROR = 4,
        RT_WARNING = 5
    };

    /**
     * Logger. Allows you to log messages into a log file. Static class.
     */
    class CLogger {
    public:
        //TODO make logger more flexible
        //TODO make logger thread safe
        /**
         * Writes a simple message into the log
         * @param type Type of the message
         * @param format The message
         */
        static void Log(ELogType type, const char *format) {
            if (COLOR_OUTPUT)
                std::cerr << TYPE_COLORS[type] << LINE_PREFIXES[type] << format << TYPE_COLOR_RESET << std::endl;
            else
                std::cerr << LINE_PREFIXES << format << std::endl;
        }

        /*----------------------------------------------------------------------*/
        /**
         * Writes a message with some values into the log. Each '%' character will be replaced with one value from arguments. Supports newlines and tabs.
         * @tparam T type of first value
         * @tparam Targs Types of other values
         * @param type Message type
         * @param format Formatting string
         * @param value First value
         * @param Fargs Other values
         */
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

    };


}
#endif //NOI_SOFTWARE_CLOGGER_HPP
