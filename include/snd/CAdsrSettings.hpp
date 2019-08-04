//
// Created by ddl_blue on 24.6.19.
//

#ifndef NOI_SOFTWARE_CADSRSETTINGS_HPP
#define NOI_SOFTWARE_CADSRSETTINGS_HPP

#include <atomic>
#include <memory>

namespace NSnd {
    class CAdsrSettings;

    typedef std::shared_ptr<CAdsrSettings> AAdsrSettings;

    class CAdsrSettings {
    public:
        explicit CAdsrSettings(double attack = 0.1, double decay = 0.1, double sustain = 1.0, double release = 0.1);

        void SetAttack(double attack);

        void SetSustain(double sustain);

        void SetDecay(double decay);

        void SetRelease(double release);

        double GetAttack();

        double GetSustain();

        double GetDecay();

        double GetRelease();

        uint32_t GetAttackSampleLen();

        uint32_t GetDecaySampleLen();

        uint32_t GetReleaseSampleLen();

    private:
        static const uint32_t CONVERSION_MULTIPLE = 1024;
        std::atomic_int m_attack;
        std::atomic_int m_sustain;
        std::atomic_int m_decay;
        std::atomic_int m_release;
    };
}

#endif //NOI_SOFTWARE_CADSRSETTINGS_HPP
