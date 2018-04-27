//
// Created by ddl_blue on 7.4.18.
//

#ifndef NOI_SOFTWARE_CLOGGER_HPP
#define NOI_SOFTWARE_CLOGGER_HPP

#include <iostream>
#include <string>


namespace NMsc {
    class CLogger {
    public:
        //TODO make logger more flexible
        //TODO make logger thread safe
        static void Log(const std::string str);

    };
}

#endif //NOI_SOFTWARE_CLOGGER_HPP
