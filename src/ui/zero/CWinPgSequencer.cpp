//
// Created by ddl_blue on 29.5.19.
//

#include "../../../include/ui/zero/CWinPgSequencer.hpp"
#include "../../../include/ui/zero/CNoiZeroCommunicator.hpp"
#include "../../../plg/midiProcessors/CSequencer.hpp"

using namespace NUi::NZero;

/*----------------------------------------------------------------------*/
CWinPgSequencer::CWinPgSequencer(NUi::WWindowManager windowManager) : CWindow(windowManager),
                                                                      m_baseTone(NUi::EControlInput::NOTE_0),
                                                                      m_pressedTonesCnt(0), m_shiftPressed(false) {
    AWindowManager manager = windowManager.lock();
    m_app = manager->GetApp();
}

/*----------------------------------------------------------------------*/
NUi::CInptutEventInfo CWinPgSequencer::ProcessInput(NUi::CInptutEventInfo input) {
    m_shiftPressed = input.m_shift;
    std::vector<NSnd::AMidiProcessor> processors = m_app->MidiProcessorsGet();
    if (processors.size() < 8) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinPgEffects: There is just % < 8 processors", processors.size());
        return CInptutEventInfo();
    }


    if (input.IsScroll()) {
        if (!m_editedSequencer)
            return CInptutEventInfo();

        // Sequence speed
        if (input.m_input == EControlInput::ROT_0) {
            bool dirUp = (input.m_type == EControlInputType::SCROLL_UP);
            int32_t speed = m_editedSequencer->m_speed;

            if (dirUp) {
                if (speed < 9)
                    ++speed;
                else if (speed < 32)
                    speed *= 2;
            } else {
                if (speed > 8)
                    speed /= 2;
                else if (speed > 1)
                    --speed;
            }

            m_editedSequencer->m_speed = speed;
        }


        return CInptutEventInfo();
    }

    if (input.m_shift) {
        // sequence selection
        if (input.IsFnKey()) {
            if (!input.JustPressed())
                return CInptutEventInfo();

            uint32_t fnKeyId = NMsc::Functions::EnumSub(input.m_input, NUi::EControlInput::BTN_FN_0);

            // Save sequence to previously edited sequence
            if (m_editedSequencer) {
                m_editedSequencer->ChangeSequence(m_sequence);
            }

            if (processors[fnKeyId] == m_editedSequencer) {
                // Deselect sequence
                m_editedSequencer = nullptr;
                m_app->MidiProcessorSelect(nullptr);
            } else {
                m_editedSequencer = std::dynamic_pointer_cast<NPlg::NSequencer::CSequencer>(processors[fnKeyId]);
                m_app->MidiProcessorSelect(m_editedSequencer);
            }

            // Load new sequence
            if (m_editedSequencer)
                m_sequence = m_editedSequencer->GetCurrentSequence();
            else
                m_sequence.clear();

            return CInptutEventInfo();
        }
        return input;
    } else {

        // Clear sequence
        if (input.m_input == EControlInput::BTN_LEFT && m_isActive) {
            if (input.JustPressed()) {
                m_sequence.clear();
                m_thisPositionTones.clear();
                m_pressedTonesCnt = 0;
            }
            return CInptutEventInfo();
        }

        // Insert silent spot
        if (input.m_input == EControlInput::BTN_RIGHT && m_isActive) {
            if (input.JustPressed()) {
                m_sequence.push_back(std::vector<int32_t>());
                m_thisPositionTones.clear();
                m_pressedTonesCnt = 0;
            }
            return CInptutEventInfo();
        }

        // Record note
        if (input.IsNoteKey()) {
            // Dont record on background
            if (!m_isActive)
                return CInptutEventInfo();

            int32_t noteValue = NMsc::Functions::EnumSub(input.m_input, m_baseTone);

            if (input.JustPressed()) {
                // First sequence position
                if (m_sequence.empty()) {
                    m_baseTone = input.m_input;
                    noteValue = 0;
                }
                // Record note
                if (m_thisPositionTones.find(noteValue) == m_thisPositionTones.end()) {

                    // First tone
                    if (!m_pressedTonesCnt || m_sequence.empty()) {
                        m_sequence.push_back(std::vector<int32_t>());
                    }

                    m_thisPositionTones.emplace(noteValue);
                    m_sequence[m_sequence.size() - 1].push_back(noteValue);
                    ++m_pressedTonesCnt;
                }

                return CInptutEventInfo();
            }

            // Release tone
            if (input.m_type == EControlInputType::RELEASE) {
                if (m_thisPositionTones.find(noteValue) != m_thisPositionTones.end()) {
                    --m_pressedTonesCnt;
                    if (!m_pressedTonesCnt) {
                        m_thisPositionTones.clear();
                    }

                }
            }
            return CInptutEventInfo();
        }
    }

    return input;

}

/*----------------------------------------------------------------------*/
void CWinPgSequencer::Draw() {
    CNoiZeroCommunicator *g = CNoiZeroCommunicator::GetInstance();
    if (!g) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CWinPgEffects: No CNoiZeroCommunicator found.");
        return;
    }

    g->SetStatusLed(EStatusLed::PAGE, ELedState::ON, NHw::ELedColor::YELLOW);

    g->ClearFnLeds();

    // NPlg::NSequencer::ASequencer sequencer =  //std::dynamic_pointer_cast<NPlg::NSequencer::CSequencer>(m_app->MidiProcessorSelectedGet());

    if (m_shiftPressed) {
        // List of all sequences
        std::vector<NSnd::AMidiProcessor> allProcessors = m_app->MidiProcessorsGet();

        int i = 0;
        for (const auto &processor : allProcessors)
            g->SetFnLed(i++, processor == m_editedSequencer ? ELedState::BLINKING : ELedState::ON,
                        NHw::ELedColor::YELLOW);

    } else {
        // Sequencer editor
        if (m_editedSequencer) {
            int32_t pos1, pos8, pos64;
            pos1 = m_sequence.size() % 8;
            pos8 = (m_sequence.size() / 8) % 8;
            pos64 = m_sequence.size() / 64;

            // position in sequence
            for (uint32_t i = 0; i < 8; ++i) {
                NHw::ELedColor col = NHw::ELedColor::BLACK;
                col = (i == pos1) ? NMsc::Functions::EnumOr(col, NHw::ELedColor::RED) : col;
                col = (i == pos8) ? NMsc::Functions::EnumOr(col, NHw::ELedColor::GREEN) : col;
                col = (i == pos64) ? NMsc::Functions::EnumOr(col, NHw::ELedColor::BLUE) : col;

                g->SetFnLed(i, (m_pressedTonesCnt && pos1 == i) ? ELedState::BLINKING : ELedState::ON, col);

            }

        } else {
            for (uint32_t i = 0; i < 8; ++i) {
                g->SetFnLed(i, ELedState::ON, NHw::ELedColor::RED);
            }
        }
    }
}

/*----------------------------------------------------------------------*/
void CWinPgSequencer::Update() {

}

/*----------------------------------------------------------------------*/
void CWinPgSequencer::Init() {

    // Make sure there is enough of sequencers
    std::vector<NSnd::AMidiProcessor> processors = m_app->MidiProcessorsGet();
    int32_t processorCnt = processors.size();

    while (processorCnt++ < 8)
        m_app->MidiProcessorAdd(std::make_shared<NPlg::NSequencer::CSequencer>());

}

/*----------------------------------------------------------------------*/
void CWinPgSequencer::Activate() {
    m_isActive = true;

    if (m_editedSequencer)
        m_editedSequencer->ChangeSequence(m_sequence);

    m_editedSequencer = std::dynamic_pointer_cast<NPlg::NSequencer::CSequencer>(m_app->MidiProcessorSelectedGet());

    if (m_editedSequencer) {
        // Get sequencer's sequence
        m_sequence = m_editedSequencer->GetCurrentSequence();

        // Disable sequence when editing
        m_editedSequencer->ChangeSequence(NPlg::NSequencer::CSequencer::CToneSequence());
    }

}

/*----------------------------------------------------------------------*/
void CWinPgSequencer::Deactivate() {
    m_isActive = false;

    if (m_editedSequencer)
        m_editedSequencer->ChangeSequence(m_sequence);
}