//
// Created by ddl_blue on 1.5.19.
//

#ifndef NOI_SOFTWARE_CZEROUI_HPP
#define NOI_SOFTWARE_CZEROUI_HPP

#include "../CUi.hpp"
#include "../CInputEventInfo.hpp"

namespace NUi::NZero {
    /**
     * Represent NoiZero UI
     */
    class CZeroUi : public CUi {
    public:
        /**
         * Constructor
         * @param app Reference to main App class
         */
        CZeroUi(NLgc::ANoiApp app);

        /**
         * Send user input to widnows
         * @param input User input
         */
        void ProcessInput(CInptutEventInfo input);

    private:
        virtual void UiThreadWorker() override;
    };

    typedef std::shared_ptr<CZeroUi> AZeroUi;
}


#endif //NOI_SOFTWARE_CZEROUI_HPP
