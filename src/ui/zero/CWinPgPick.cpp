//
// Created by ddl_blue on 9.5.19.
//

#include "../../../include/ui/zero/CWinPgPick.hpp"

using namespace NUi::NZero;

/*----------------------------------------------------------------------*/
CWinPgPick::CWinPgPick(NUi::WWindowManager windowManager) : CWindow(windowManager) {
    AWindowManager manager = windowManager.lock();
    m_app = manager->GetApp();
}

/*----------------------------------------------------------------------*/
