//
// Created by ddl_blue on 24.5.19.
//

#ifndef NOI_SOFTWARE_CWINFILEDIALOG_HPP
#define NOI_SOFTWARE_CWINFILEDIALOG_HPP

#include "../CWindow.hpp"

namespace NUi::NZero {
    /**
     * Dialog that allows you to save or load a project.
     */
    class CWinFileDialog : public CWindow {
    public:
        /**
         * Constructor
         * @param windowManager Parent window manager
         */
        explicit CWinFileDialog(WWindowManager windowManager, bool saveDialog);

        virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

        virtual void Draw() override;

        void Update() override;

    private:
        /// Reference to main App class
        NLgc::ANoiApp m_app;

        /// Path to saved projects
        const char *FILE_FOLDER = "./usr/saves/";

        /// Is this a save dialog, or a load dialog?
        bool m_saveDialog;

        /// ID of confirmation dialog's return value request
        uint64_t m_confirmationDialogID;

        /// ID of selected file
        uint32_t m_fileId;
    };
}

#endif //NOI_SOFTWARE_CWINFILEDIALOG_HPP
