//noexcept
// Created by ddl_blue on 7.4.18.
//

#ifndef NOI_SOFTWARE_CLOCKLESSQUE_HPP
#define NOI_SOFTWARE_CLOCKLESSQUE_HPP

#include <atomic>
#include <memory>
#include "CLogger.hpp"

namespace NMsc {

// TODO do more in depth test of this, research atomic int under ARM
// TODO investigate destructor behaviour in the cirrcle buffer. pop, push, rewriting, etc 
/*############################################################################################################################*/
    /**
     * Generic lockless que. For two threads. One thread can push, the other one can pop.
     * @tparam T Data type in the que
     */
    template<class T>
    class CLocklessQue {
    public:

        CLocklessQue();

        ~CLocklessQue();

        /**
         * Push new item into the que
         * @param item Item pushed into the que
         * @return True on success
         */
        bool Push(const T &item);

        /**
         * Pop last element in the que
         * @return Removed element
         */
        T Pop();

        /**
         * Is que empty?
         * @return True if empty
         */
        bool Empty() const;

        /**
         * Clears the que
         */
        void Clear();


    private:
        const static int BUFFER_SIZE = 1024;
        std::atomic_int m_head, m_tail;
        T *m_buffer;
    };

    template<class T>
    using ALocklessQue = std::shared_ptr<CLocklessQue<T>>;

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
        // Todo check this in a code.
        NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CLocklsessQue: Push failed!");
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
    template<class T>
    void CLocklessQue<T>::Clear() {
        while (!Empty())
            Pop();
    }
}

#endif //NOI_SOFTWARE_CLOCKLESSQUE_HPP
