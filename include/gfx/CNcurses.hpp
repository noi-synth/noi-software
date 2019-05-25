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
    /**
     * Wrapper around NCurses library. Allows simple window drawing on terminal. Singleton class.
     */
    class CNcurses {

    public:

        /**
         * Available color combinations of foreground and background.
         */
        enum ColorPair {
            WHITE_BLACK = 1,
            RED_BLACK,
            WHITE_BLUE,
            BLACK_BLUE,

            BLACK_WHITE,
            BLACK_RED,
            BLACK_GREEN,
            BLACK_YELLOW,
            BLACK_MAGENTA,
            BLACK_CYAN,

        };

        /**
         * Get instance of the singleton.
         * @return Singleton instance of CNcurses.
         */
        static CNcurses *GetInstance();

        /**
         * Destroy the instance singleton of CNcurses.
         */
        static void FreeInstance();

        /**
         * Sets a frame in center of the screen for drawing. From now on, all drawing is relative to this region.
         * @param width Width of the region
         * @param height Height of the region
         * @param xOffset X-axis offset of the frame from centered position
         * @param yOffset Y-axis offset of the frame from centered position
         * @return True if frame fits on terminal screen
         */
        bool SetFrame(unsigned int width, unsigned int height, int xOffset = 0, int yOffset = 0);

        /**
         * Draws a window in the whole frame.
         * @param colors Colors of the window space.
         */
        void DrawEmptyWindow(ColorPair colors);

        /***
         * Draws a text on position x and y within the current frame. Supports \\n and \\t characters.
         * @param x X coordinate of the text within the frame
         * @param y Y coordinate of the text within the frame
         * @param text Text to be drawn
         * @param colors Foreground and background of the text
         */
        void DrawText(unsigned int x, unsigned int y, std::string text, ColorPair colors);

        /**
         * Draw text centered within the frame. Supports \\n and \\t characters.
         * @param y Y coordinate of the text within the frame
         * @param text Text to be drawn
         * @param colors Foreground and background of the text
         */
        void DrawTextCentered(unsigned int y, std::string text, ColorPair colors);

        void DrawPoint(unsigned int x, unsigned int y, char ch, ColorPair colors);

        /**
         * Clears the whole terminal screen.
         */
        void ClearScreen();

        /**
         * Checks for user input.
         * @return User's pressed input.
         */
        int GetInput();

        /**
         * Apply all changes on a terminal screen.
         */
        void Update();

        /**
         * Get width of the terminal.
         * @return terminal widrh
         */
        unsigned int GetScreenWidth();

        /**
         * Get height of the terminal.
         * @return terminal height
         */
        unsigned int GetScreenHeight();

        /**
         * Draws text into the frame. Can also print variable's values. Any occurrence of the '%' character will be replaced with next argument's value.
         * @tparam T Type of argument
         * @tparam Targs Type of other arguments
         * @param x X coordinate of the text within the frame
         * @param y Y coordinate of the text within the frame
         * @param colors Foreground and background of the text
         * @param format String to be drawn. Any '%' character will be replaced with next argument value.
         * @param value First value argument
         * @param Fargs Other value arguments
         */
        template<typename T, typename... Targs>
        void
        DrawTextFormated(unsigned int x, unsigned int y, ColorPair colors, const char *format, T value, Targs... Fargs);

    private:
        CNcurses();

        ~CNcurses();

        /**
         * Instance of this singleton.
         */
        static CNcurses *m_instance;

        /**
         * Size and position of current frame.
         */
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