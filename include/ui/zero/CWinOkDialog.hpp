//
// Created by ddl_blue on 23.5.19.
//

#ifndef NOI_SOFTWARE_CWINOKDIALOG_HPP
#define NOI_SOFTWARE_CWINOKDIALOG_HPP

#include "../CWindow.hpp"

namespace NUi::NZero {
    class CWinOkDialog : public CWindow {
    public:
        /**
         * Constructor
         * @param windowManager Parent window manager
         */
        explicit CWinOkDialog(WWindowManager windowManager, uint64_t requestId);

        virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

        virtual void Draw() override;

        void Update() override;

    private:
        /// Reference to main App class
        NLgc::ANoiApp m_app;

        /// Binary mask that defines speed of animations
        static const uint64_t ANIMATION_SLOWDOWN_MASK = 4;

        /// Counter that slows down animations
        uint64_t m_animationSlowdown;

        /// ID of request for return value of this dialog
        uint64_t m_requestId;

        /// Progress of welcome animation. 0 means end of animation
        uint32_t m_welcomeAnimation;

        /// Progress of confirmation dialog. Dialog is confirmed when certain value is reached.
        uint32_t m_progress;

        /// Progress of ending animation. 0 means it did not even started.
        uint32_t m_endingAnimation;

        /// Result of the dialog. Is returned at the end of ending animation.
        bool m_result;

    };
}

#endif //NOI_SOFTWARE_CWINOKDIALOG_HPP
