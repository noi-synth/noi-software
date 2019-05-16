//
// Created by ddl_blue on 31.7.18.
//

#ifndef NOI_SOFTWARE_CINPUTRESOLVER_HPP
#define NOI_SOFTWARE_CINPUTRESOLVER_HPP

#include "../UiEnums.hpp"

namespace NUi {
    namespace NTerm {
        /**
         * Translates computer key press into internal input event
         */
        class CInputResolver {
        public:
            /**
             * Translates computer key press into internal input event
             * @param input Pressed character
             * @return Internal input event
             */
            static EControlInput ResolveInput(int input);
        };
    }
}


#endif //NOI_SOFTWARE_CINPUTRESOLVER_HPP
