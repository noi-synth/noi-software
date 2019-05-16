//
// Created by ddl_blue on 10.9.18.
//

#ifndef NOI_SOFTWARE_CNOIZEROHW_HPP
#define NOI_SOFTWARE_CNOIZEROHW_HPP

#include <cstdint>
#include <vector>
#include <functional>
#include "../config.hpp"

#ifndef NO_RPI_HW


#include "CI2CGpioExtender.hpp"
#include "../snd/CMidiMsg.hpp"
#include "../msc/CLocklessQue.hpp"
#include "../ui/CInputEventInfo.hpp"
#include "HwEnums.hpp"

// Thread unsafe! (adding outputs and pushing buttons might make it blow up)
namespace NHw {

    /**
     * Device hardware's abstraction. Takes care of all communication with Noi Zero device controls.
     */
    class CNoiZeroHw {
    public:
        CNoiZeroHw();

        ~CNoiZeroHw();

        /**
         * Attach a function, that will be called each time a note key is pressed, or released
         * @param outputFunction Function to be called
         */
        void AttachMidiOutput(const std::function<void(NSnd::CMidiMsg)> &outputFunction);

        /**
         * Attach a que, that will receive control input messages
         * @param outputQue Que that will recive messages
         */
        void AttachControlOutput(NMsc::ALocklessQue<NUi::CInptutEventInfo> outputQue);

        /**
         * Clear all attached outputs
         */
        void ClearMidiOuptuts();

        /**
         * Clear all attached cotnrol outptus
         */
        void ClearControlOutputs();

        /**
         * Set a color of a RGB LED.
         * @param ledId LED that will be affected
         * @param color New color of this LED
         */
        void SetLedOutput(ELedId ledId, ELedColor color);

    private:

        /**
         * Represents a GPIO Extender's pin
         */
        struct CPinPosition {
            /**
             * Constructor
             * @param extender Extender that this pin belongs to
             * @param pinId Extender's pin number
             */
            CPinPosition(EExtenderId extender, uint32_t pinId) : m_extenderId(extender), m_pinId(pinId),
                                                                 m_empty(false) {}

            CPinPosition() : m_extenderId(EExtenderId::A), m_pinId(999), m_empty(true) {}

            /**
             * Constructs a PinPosition from a globally unique pin ID (that can be obtained using GetGlobalPortNumber method)
             * @param globalPortNumber Gobally unique pin ID
             */
            explicit CPinPosition(int32_t globalPortNumber) : m_empty(false) {
                m_extenderId = static_cast<EExtenderId> (globalPortNumber / 16);
                m_pinId = (unsigned) globalPortNumber % 16;
            }

            /**
             * Extender that this pin belongs to
             */
            EExtenderId m_extenderId;

            /**
             * Pin's number on an extender
             */
            uint32_t m_pinId;

            /**
             * True if "no pin"
             */
            bool m_empty;

            /**
             * Get an globally unique pin ID
             * @return globally unique pin ID
             */
            int32_t GetGlobalPortNumber() {
                if (m_empty)
                    return -1;
                return m_extenderId * 16 + m_pinId;
            }
        };

        /**
         * Method that runs in this class's worker thread
         */
        void WorkerMethod();


        /**
         * Takes an input from GPIO extender and generate corresponding input messages
         * @param extenderId ID of scanned extender
         * @param value Value read from the extender
         * @param diff Difference from last value
         * @param interruptTimeValues Are those interrupt-time values?
         */
        void
        ProcessReadTimeValues(EExtenderId extenderId, uint16_t value, uint16_t diff, bool interruptTimeValues = false);

        /**
         * Send a midi message to all attached outputs
         * @param message Message to send
         */
        void SendMidiMsg(NSnd::CMidiMsg message);

        /**
         * Send a control event info to all attached outputs
         * @param control Event to send
         */
        void SendControl(NUi::CInptutEventInfo control);

        /**
         * Midi messages will be sent to those outputs
         */
        std::vector<std::function<void(NSnd::CMidiMsg)>> m_midiOutputs;

        /**
         * Control input events will be sent to those outputs
         */
        std::vector<NMsc::ALocklessQue<NUi::CInptutEventInfo>> m_controlOutputs;

        /**
         * If true, the worker thread will stop
         */
        bool m_stopWorker;
        /**
         * Worker thread
         */
        std::thread m_worker;

        /**
         * Pin positions of red channels of all LEDs
         */
        CPinPosition LED_POSITIONS[_LED_NUMBER];
        /**
         * Pin positions of all buttons and knobs
         */
        CPinPosition CONTROL_POSITIONS[NUi::EControlInput::_CONTROL_LAST];

        /**
         * Number of GPIO extenders
         */
        const static uint32_t EXTENDER_CNT = 5;

        /**
         * Inverted table of CONTROL_POSITIONS
         */
        NUi::EControlInput CONTROLS[EXTENDER_CNT * 16];

        /**
         * Connected GPIO extenders
         */
        std::vector<CI2cGpioExtender> m_extenders;

    };

    typedef std::shared_ptr<CNoiZeroHw> ANoiZeroHw;
}

#endif /* NO_RPI_HW */

#endif //NOI_SOFTWARE_CNOIZEROHW_HPP
