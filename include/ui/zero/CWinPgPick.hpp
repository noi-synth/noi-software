//
// Created by ddl_blue on 9.5.19.
//

#ifndef NOI_SOFTWARE_CWINPGPICK_HPP
#define NOI_SOFTWARE_CWINPGPICK_HPP


/*class CWinPgPick {

};*/

#include "../CWindow.hpp"

namespace NUi {
    namespace NZero {
        /**
         * Window that allows page change.
         */
        class CWinPgPick : public CWindow {
        public:
            /**
             * Constructor
             * @param windowManager Parent window manager
             */
            explicit CWinPgPick(WWindowManager windowManager);

            virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

            virtual void Draw() override;

            //void Update() override;

            //void Init() override;

        };
    }
}


#endif //NOI_SOFTWARE_CWINPGPICK_HPP
