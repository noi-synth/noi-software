//
// Created by ddl_blue on 30.7.18.
//

#include "../../include/gfx/CNcurses.hpp"

#ifdef NCURSES_ENABLED

#include <ncurses.h>
#include <iostream>

using namespace NGfx;

CNcurses *CNcurses::m_instance;

/*----------------------------------------------------------------------*/
CNcurses::CNcurses() {
    initscr();
    if (!has_colors()) {
        endwin();
        std::cout << "Your terminal does not support color\n" << std::endl;
        return;
    }
    start_color();


    // Colors
    init_pair(ColorPair::WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
    init_pair(ColorPair::RED_BLACK, COLOR_RED, COLOR_BLACK);
    init_pair(ColorPair::WHITE_BLUE, COLOR_WHITE, COLOR_BLUE);
    init_pair(ColorPair::BLACK_BLUE, COLOR_BLACK, COLOR_BLUE);

    init_pair(ColorPair::BLACK_WHITE, COLOR_BLACK, COLOR_WHITE);
    init_pair(ColorPair::BLACK_RED, COLOR_BLACK, COLOR_RED);
    init_pair(ColorPair::BLACK_GREEN, COLOR_BLACK, COLOR_GREEN);
    init_pair(ColorPair::BLACK_YELLOW, COLOR_BLACK, COLOR_YELLOW);
    init_pair(ColorPair::BLACK_MAGENTA, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(ColorPair::BLACK_CYAN, COLOR_BLACK, COLOR_CYAN);



    timeout(17);
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE);
}

/*----------------------------------------------------------------------*/
CNcurses::~CNcurses() {
    endwin();
}

/*----------------------------------------------------------------------*/
CNcurses *CNcurses::GetInstance() {
    if (!m_instance)
        m_instance = new CNcurses();
    return m_instance;
}

/*----------------------------------------------------------------------*/
void CNcurses::FreeInstance() {
    if (m_instance)
        delete m_instance;
    m_instance = nullptr;
}

/*----------------------------------------------------------------------*/
bool CNcurses::SetFrame(unsigned int width, unsigned int height) {
    if (width > COLS || height > LINES)
        return false;

    m_frameW = width;
    m_frameH = height;
    m_frameX = (COLS - width) / 2;
    m_frameY = (LINES - height) / 2;
    return true;
}

/*----------------------------------------------------------------------*/
void CNcurses::DrawEmptyWindow(NGfx::CNcurses::ColorPair colors) {
    attron(COLOR_PAIR(colors));

    for (unsigned int i = 0; i < m_frameH; ++i) {
        move(m_frameY + i, m_frameX);
        for (unsigned int j = 0; j < m_frameW; ++j) {
            addch(' ');
        }
    }

    for (int i = 1; i < m_frameH; ++i) {
        mvaddch(m_frameY + i, m_frameX, ACS_VLINE);
        mvaddch(m_frameY + i, m_frameX + m_frameW, ACS_VLINE);
    }

    for (int i = 1; i < m_frameW; ++i) {
        mvaddch(m_frameY, m_frameX + i, ACS_HLINE);
        mvaddch(m_frameY + m_frameH, m_frameX + i, ACS_HLINE);
    }

    mvaddch(m_frameY, m_frameX, ACS_ULCORNER);
    mvaddch(m_frameY, m_frameX + m_frameW, ACS_URCORNER);
    mvaddch(m_frameY + m_frameH, m_frameX + m_frameW, ACS_LRCORNER);
    mvaddch(m_frameY + m_frameH, m_frameX, ACS_LLCORNER);

    attroff(COLOR_PAIR(colors));
}

/*----------------------------------------------------------------------*/
void CNcurses::DrawText(unsigned int x, unsigned int y, std::string text, NGfx::CNcurses::ColorPair colors) {
    attron(COLOR_PAIR(colors));

    // Move to local window coordinates
    x += m_frameX;
    y += m_frameY;

    int lineX = x;

    const char *t = text.c_str();
    move(y, x);
    for (; *t; ++t) {
        // newline
        if (*t == '\n') {
            move(++y, x);
            lineX = x;
        }
            // tab
        else if (*t == '\t') {
            lineX = ((lineX & (~0x7)) + 8);
            move(y, lineX);
        } else {
            addch((chtype) *t);
            ++lineX;
        }
    }


    //mvaddstr(m_frameY + y, m_frameX + x, text.c_str());
    attroff(COLOR_PAIR(colors));
}

void CNcurses::DrawTextCentered(unsigned int y, std::string text, NGfx::CNcurses::ColorPair colors) {
    int x = (int) (m_frameW / 2 - text.length() / 2);
    attron(COLOR_PAIR(colors));
    mvaddstr(m_frameY + y, m_frameX + x, text.c_str());
    attroff(COLOR_PAIR(colors));
}

/*----------------------------------------------------------------------*/
void CNcurses::DrawPoint(unsigned int x, unsigned int y, char ch, NGfx::CNcurses::ColorPair colors) {
    attron(COLOR_PAIR(colors));
    mvaddch(m_frameY + y, m_frameX + x, ch);
    attroff(COLOR_PAIR(colors));
}

/*----------------------------------------------------------------------*/
void CNcurses::ClearScreen() {
    erase();
}

/*----------------------------------------------------------------------*/
void CNcurses::Update() {
    refresh();
}

/*----------------------------------------------------------------------*/
unsigned int CNcurses::GetScreenWidth() {
    return (unsigned int) COLS;
}

/*----------------------------------------------------------------------*/
unsigned int CNcurses::GetScreenHeight() {
    return (unsigned int) LINES;
}

/*----------------------------------------------------------------------*/
int CNcurses::GetInput() {
    return getch();
}


#endif /* NCURSES_ENABLED */