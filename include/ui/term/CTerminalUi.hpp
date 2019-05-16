//
// Created by ddl_blue on 31.7.18.
//

#ifndef NOI_SOFTWARE_CTERMINALUI_HPP
#define NOI_SOFTWARE_CTERMINALUI_HPP


#include "../CUi.hpp"

namespace NUi {
    namespace NTerm {
        /**
         * NCurses based terminal UI for debug purpose.
         */
        class CTerminalUi : public CUi {
        public:
            CTerminalUi(NLgc::ANoiApp app);

        private:
            virtual void UiThreadWorker() override;

            /// Used for toggling notes
            std::vector<bool> m_toneState;
        };
    }
}


#endif //NOI_SOFTWARE_CTERMINALUI_HPP
