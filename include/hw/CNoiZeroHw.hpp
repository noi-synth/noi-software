//
// Created by ddl_blue on 10.9.18.
//

#ifndef NOI_SOFTWARE_CNOIZEROHW_HPP
#define NOI_SOFTWARE_CNOIZEROHW_HPP

#include <cstdint>
#include <vector>
#include <functional>
#include "../config.hpp"
#include "CI2CGpioExtender.hpp"
#include "../snd/CMidiMsg.hpp"
#include "../msc/CLocklessQue.hpp"
#include "../ui/CInputEventInfo.hpp"
#include "HwEnums.hpp"

// Thread unsafe! (adding outputs and pushing buttons might make it blow up)
namespace NHw {
    class CNoiZeroHw {
    public:
        CNoiZeroHw();

        ~CNoiZeroHw();

        void AttachMidiOutput(const std::function<void(NSnd::CMidiMsg)> &outputFunction);

        void AttachControlOutput(NMsc::ALocklessQue<NUi::CInptutEventInfo> outputQue);

        void ClearMidiOuptuts();

        void ClearControlOutputs();

        void SetLedOutput(ELedId ledId, ELedColor color);

    private:

        struct CPinPosition {
            CPinPosition(EExtenderId extender, uint32_t pinId) : m_extenderId(extender), m_pinId(pinId),
                                                                 m_empty(false) {}

            CPinPosition() : m_extenderId(EExtenderId::A), m_pinId(999), m_empty(true) {}

            explicit CPinPosition(int32_t globalPortNumber) : m_empty(false) {
                m_extenderId = static_cast<EExtenderId> (globalPortNumber / 16);
                m_pinId = (unsigned) globalPortNumber % 16;
            }

            EExtenderId m_extenderId;
            uint32_t m_pinId;
            bool m_empty;

            uint16_t GetMask() {
                return (uint16_t) 1 << m_pinId;
            }

            int32_t GetGlobalPortNumber() {
                if (m_empty)
                    return -1;
                return m_extenderId * 16 + m_pinId;
            }
        };

        void WorkerMethod();

        void
        ProcessReadTimeValues(EExtenderId extenderId, uint16_t value, uint16_t diff, bool interruptTimeValues = false);

        void SendMidiMsg(NSnd::CMidiMsg message);

        void SendControl(NUi::CInptutEventInfo control);

        std::vector<std::function<void(NSnd::CMidiMsg)>> m_midiOutputs;

        std::vector<NMsc::ALocklessQue<NUi::CInptutEventInfo>> m_controlOutputs;

        bool m_stopWorker;
        std::thread m_worker;

        uint32_t m_msWaiting;

        const static uint32_t MIN_WAITING = 4;
        const static uint32_t MAX_WAITING = 100;
        const static uint32_t STEP_WAITING = 10;

        CPinPosition LED_POSITIONS[_LED_NUMBER];
        CPinPosition CONTROL_POSITIONS[NUi::EControlInput::_CONTROL_LAST];

        const static uint32_t EXTENDER_CNT = 5;

        NUi::EControlInput CONTROLS[EXTENDER_CNT * 16];

        // I2C
#ifndef NO_RPI_HW
        std::vector<CI2cGpioExtender> m_extenders;
#endif
    };
}

#endif //NOI_SOFTWARE_CNOIZEROHW_HPP
