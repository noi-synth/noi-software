//
// Created by ddl_blue on 26.10.18.
//

#include "../../../include/ui/term/WinPickFromList.hpp"
#include "../../../include/ui/UiEnums.hpp"
#include "../../../include/gfx/CNcurses.hpp"

using namespace NUi;
using namespace NUi::NTerm;
using Color = NGfx::CNcurses::ColorPair;

/*----------------------------------------------------------------------*/
WinPickFromList::WinPickFromList(NUi::WWindowManager manager, std::string title,
                                 std::vector<std::pair<std::string, std::function<void()>>> &items, bool canExit)
        : CWindow(manager), m_title(title), m_items(items), m_canExit(canExit) {
    m_position = m_items.begin();


}

/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
ControlInput WinPickFromList::ProcessInput(ControlInput control, ControlInputType type) {

    AWindowManager wm = m_manager.lock();

    if (m_isActive)
        switch (control) {
            case ControlInput::BTN_SHUTDOWN:
                if (m_canExit)
                    wm->CloseTopWindowCallback();
                return ControlInput::NONE;

            case ControlInput::BTN_UP:
                if (m_position == m_items.begin())
                    m_position = --m_items.end();
                else
                    --m_position;
                RequestRedraw();
                return ControlInput::NONE;

            case ControlInput::BTN_DOWN:
                if (++m_position == m_items.end())
                    m_position = m_items.begin();
                RequestRedraw();
                return ControlInput::NONE;

            case BTN_OK:
                m_position->second();
                return ControlInput::NONE;

            default:
                return ControlInput::NONE;
        }
}

/*----------------------------------------------------------------------*/
void WinPickFromList::Draw() {
    NGfx::CNcurses *gfx = NGfx::CNcurses::GetInstance();

    // Todo scrolling

    gfx->SetFrame(80, 30);

    gfx->DrawEmptyWindow(Color::WHITE_BLUE);
    gfx->DrawTextCentered(2, m_title, Color::WHITE_BLACK);

    unsigned int devDispY = 4;

    for (auto item = m_items.begin(); item != m_items.end(); ++item) {
        gfx->DrawText(2, devDispY++, item->first, item == m_position ? Color::WHITE_BLUE : Color::BLACK_BLUE);
    }
}


