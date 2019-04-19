//
// Created by ddl_blue on 19.4.19.
//

#ifndef NOI_SOFTWARE_CWIRINGPIHANDLER_HPP
#define NOI_SOFTWARE_CWIRINGPIHANDLER_HPP

namespace NHw {
    /**
     * Makes sure that WiringPi is initialized.
     */
    class CWiringPiHandler {
    public:
        static void Init();

    private:
        static bool s_isInitialized;
    };
}

#endif //NOI_SOFTWARE_CWIRINGPIHANDLER_HPP
