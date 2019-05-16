//
// Created by ddl_blue on 26.10.18.
//

#ifndef NOI_SOFTWARE_WINPICKFROMLIST_HPP
#define NOI_SOFTWARE_WINPICKFROMLIST_HPP

#include <string>
#include <vector>
#include <functional>

#include "../CWindow.hpp"


namespace NUi {
    namespace NTerm {
        /**
         * Generic window for picking from list of choices.
         */
        class WinPickFromList : public CWindow {
        public:
            /**
             * Constructor
             * @param manager Window manager this window belongs to
             * @param title Title of the window
             * @param items List of items
             * @param canExit If true, can be exited without actually picking something
             */
            WinPickFromList(WWindowManager manager, std::string title,
                            std::vector<std::pair<std::string, std::function<void()>>> &items, bool canExit);

            virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

            void Draw() override;

            /// One item in list
            typedef std::vector<std::pair<std::string, std::function<void()>>> items;

        private:
            /// Iterator to iterate list of choices
            typedef std::vector<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::function<void()>>, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::function<void()>>>>::iterator position_iterator;

            /// Title of the window
            std::string m_title;

            /// List of items
            std::vector<std::pair<std::string, std::function<void()>>> m_items;

            /// If true, can be exited without actually picking something
            bool m_canExit;

            /// Current position of choice
            position_iterator m_position;
        };
    }
}


#endif //NOI_SOFTWARE_WINPICKFROMLIST_HPP
