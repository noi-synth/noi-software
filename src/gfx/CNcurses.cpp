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
    if (width >= COLS || height >= LINES)
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
    mvaddstr(m_frameY + y, m_frameX + x, text.c_str());
    attroff(COLOR_PAIR(colors));
}

void CNcurses::DrawTextCentered(unsigned int y, std::string text, NGfx::CNcurses::ColorPair colors) {
    int x = (int) (m_frameW / 2 - text.length() / 2);
    attron(COLOR_PAIR(colors));
    mvaddstr(m_frameY + y, m_frameX + x, text.c_str());
    attroff(COLOR_PAIR(colors));
}

/*----------------------------------------------------------------------*/
void CNcurses::ClearScreen() {
    clear();
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