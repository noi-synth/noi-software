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
        class WinPickFromList : public CWindow {
        public:
            WinPickFromList(WWindowManager manager, std::string title,
                            std::vector<std::pair<std::string, std::function<void()>>> &items, bool canExit);

            virtual CInptutEventInfo ProcessInput(CInptutEventInfo input) override;

            void Draw() override;

            typedef std::vector<std::pair<std::string, std::function<void()>>> items;

        private:
            typedef std::vector<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::function<void()>>, std::allocator<std::pair<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>, std::function<void()>>>>::iterator position_iterator;
            std::string m_title;
            std::vector<std::pair<std::string, std::function<void()>>> m_items;
            bool m_canExit;

            position_iterator m_position;
        };
    }
}


#endif //NOI_SOFTWARE_WINPICKFROMLIST_HPP
