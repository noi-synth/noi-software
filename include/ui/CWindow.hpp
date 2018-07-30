//
// Created by ddl_blue on 7.7.18.
//

#ifndef NOI_SOFTWARE_CWINDOW_HPP
#define NOI_SOFTWARE_CWINDOW_HPP

#include <memory>

namespace NUi {
    class CWindow;

    typedef std::shared_ptr<CWindow> AWindow;
    typedef std::weak_ptr<CWindow> WWindow;
}

#include "UiEnums.hpp"
#include "CWindowManager.hpp"

namespace NUi {

    class CWindow {
    public:
        CWindow(WWindowManager windowManager);

        virtual void Activate();

        virtual void Deactivate();

        virtual ControlInput ProcessInput(ControlInput control, ControlInputType type);

        virtual void Draw();

    protected:
        WWindowManager m_manager;
        bool m_isActive;

    };

}

#endif //NOI_SOFTWARE_CWINDOW_HPP
