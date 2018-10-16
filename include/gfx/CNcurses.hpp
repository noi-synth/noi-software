//
// Created by ddl_blue on 30.7.18.
//
#include "../config.hpp"
#include "../msc/Functions.hpp"

#ifdef NCURSES_ENABLED

#ifndef NOI_SOFTWARE_CNCURSES_HPP
#define NOI_SOFTWARE_CNCURSES_HPP

#include <string>
#include <sstream>

namespace NGfx {
    class CNcurses {
    public:
        enum ColorPair {
            WHITE_BLACK = 1,
            RED_BLACK,
            WHITE_BLUE,
            BLACK_BLUE
        };

        static CNcurses *GetInstance();

        static void FreeInstance();

        bool SetFrame(unsigned int width, unsigned int height);

        //void DrawBorder(ColorPair colors);

        void DrawEmptyWindow(ColorPair colors);

        void DrawText(unsigned int x, unsigned int y, std::string text, ColorPair colors);

        void DrawTextCentered(unsigned int y, std::string text, ColorPair colors);

        void DrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, ColorPair colors);

        void ClearScreen();

        int GetInput();

        void Update();

        unsigned int GetScreenWidth();

        unsigned int GetScreenHeight();

        template<typename T, typename... Targs>
        void
        DrawTextFormated(unsigned int x, unsigned int y, ColorPair colors, const char *format, T value, Targs... Fargs);

    private:
        CNcurses();

        ~CNcurses();

        static CNcurses *m_instance;
        unsigned int m_frameX, m_frameY, m_frameW, m_frameH;

    };

    /*----------------------------------------------------------------------*/
    template<typename T, typename... Targs>
    void CNcurses::DrawTextFormated(unsigned int x, unsigned int y, NGfx::CNcurses::ColorPair colors,
                                    const char *format, T value, Targs... Fargs) {
        std::ostringstream stream;
        NMsc::Functions::Print(stream, format, value, Fargs...);
        DrawText(x, y, stream.str(), colors);
    }
}

#endif //NOI_SOFTWARE_CNCURSES_HPP

#endif /* NCURSES_ENABLED */