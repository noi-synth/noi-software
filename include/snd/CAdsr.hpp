//
// Created by ddl_blue on 24.6.19.
//

#ifndef NOI_SOFTWARE_CADSR_HPP
#define NOI_SOFTWARE_CADSR_HPP

#include <memory>

#include "CAdsrSettings.hpp"

namespace NSnd {
    class CAdsr {
    public:
        CAdsr(AAdsrSettings &settings);

        CAdsr();

        void StartNote();

        void StopNote();

        double GetVolume();

        void Tick();

        bool GetNoteOn();


    private:
        enum AsdrState {
            ADSR_OFF,
            ADSR_ATTACK,
            ADSR_DECAY,
            ADSR_SUSTAIN,
            ADSR_RELEASE
        };

        void SetupState(AsdrState state);

        AsdrState m_state;
        double m_volume;
        double m_deltaVolume;
        AAdsrSettings m_settings;

    };

    typedef std::shared_ptr<CAdsr> AAdsr;

}

#endif //NOI_SOFTWARE_CADSR_HPP
