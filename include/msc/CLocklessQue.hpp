//
// Created by ddl_blue on 7.4.18.
//

#ifndef NOI_SOFTWARE_CLOCKLESSQUE_HPP
#define NOI_SOFTWARE_CLOCKLESSQUE_HPP

#include <atomic>

namespace NMsc {


/*############################################################################################################################*/
    template<class T>
    class CLocklessQue {
    public:

        CLocklessQue();

        ~CLocklessQue();

        bool Push(const T &item);

        T Pop();

        bool Empty() const;


    private:
        const static int BUFFER_SIZE = 16;
        std::atomic_int m_head, m_tail;
        T *m_buffer;
    };

/*----------------------------------------------------------------------*/
    template<class T>
    CLocklessQue<T>::CLocklessQue() : m_head(0), m_tail(0) {
        m_buffer = new T[BUFFER_SIZE];
    }

/*----------------------------------------------------------------------*/
    template<class T>
    CLocklessQue<T>::~CLocklessQue() {
        delete[] m_buffer;
    }

/*----------------------------------------------------------------------*/
    template<class T>
    bool CLocklessQue<T>::Push(const T &item) {
        if ((m_tail - m_head + BUFFER_SIZE + 1) % BUFFER_SIZE) {
            m_buffer[m_tail] = item;
            m_tail = m_tail < BUFFER_SIZE - 1 ? m_tail + 1 : 0;
            return true;
        }
        return false;
    }

/*----------------------------------------------------------------------*/
    template<class T>
    T CLocklessQue<T>::Pop() {
        T rtrn = m_buffer[m_head];
        m_head = m_head < BUFFER_SIZE - 1 ? m_head + 1 : 0;
        return rtrn;
    }

/*----------------------------------------------------------------------*/
    template<class T>
    bool CLocklessQue<T>::Empty() const {
        return (m_head == m_tail);
    }

/*----------------------------------------------------------------------*/




}

#endif //NOI_SOFTWARE_CLOCKLESSQUE_HPP
