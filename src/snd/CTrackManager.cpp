//
// Created by ddl_blue on 26.8.18.
//

#include <cstring>
#include "../../include/snd/CTrackManager.hpp"
#include "../../include/msc/CMaintainer.hpp"
#include "../../include/msc/CLogger.hpp"
#include "../../include/msc/CDebugInfo.hpp"

using namespace NSnd;

/*----------------------------------------------------------------------*/
CTrackManager::CTrackManager() : m_playbackPosition(0), m_isRecording(false), m_isPlaying(false) {
    m_workerId = NMsc::CMaintainer::GetInstance().RegisterTask([&]() {
        // Do cool stuff (todo)
    });
}

/*----------------------------------------------------------------------*/
CTrackManager::~CTrackManager() {
    NMsc::CMaintainer::GetInstance().UnregisterTask(m_workerId);
}

/*----------------------------------------------------------------------*/
void CTrackManager::ProcessBuffer(SND_DATA_TYPE *input, SND_DATA_TYPE *buffer, unsigned long int len) {

#ifdef DEBUG
    NMsc::CDebugInfo::m_sndPositionDisplacement = m_playbackPosition;
#endif

    std::memcpy(buffer, input, (len << 1) * sizeof(SND_DATA_TYPE));
    if (m_isPlaying) {
        if (m_trackLock.TryLockBlue()) {
//            memset(buffer, 0, (len<<1) * sizeof(SND_DATA_TYPE));
            for (const auto &track : m_tracks) {
                track->ProcessBuffer(input, buffer, len);
            }

            m_playbackPosition += len;
            m_trackLock.Unlock();
        } else {
            NMsc::CLogger::Log(NMsc::ELogType::RT_ERROR, "CTrackManager: playback failed to lock the lock.");
            // TODO log warning
        }
    }
}

/*----------------------------------------------------------------------*/
bool CTrackManager::SelectTrack(const NSnd::ATrack &track) {

    if (m_trackLock.TryLockBlue()) {
        if (m_selectedTrack)
            m_selectedTrack->StopRecording();

        track->SetPosition(m_playbackPosition);
        if (m_isRecording)
            track->StartRecording();

        // m_tracks.insert(track);
        m_selectedTrack = track;

        m_trackLock.Unlock();
        return true;
    }
    return false;
}

/*----------------------------------------------------------------------*/
ATrack CTrackManager::CreateTrack() {
    if (m_trackLock.TryLockBlue()) {
        ATrack rtrn = std::make_shared<CTrack>();

        rtrn->SetPosition(m_playbackPosition);

        m_tracks.insert(rtrn);
        m_trackLock.Unlock();
        return rtrn;
    }

    return ATrack();
}

/*----------------------------------------------------------------------*/
bool CTrackManager::RemoveTrack(const NSnd::ATrack &track) {
    if (m_trackLock.TryLockBlue()) {
        m_tracks.erase(track);
        if (m_selectedTrack == track) {
            if (!m_tracks.empty())
                m_selectedTrack = *m_tracks.begin();
            else
                m_selectedTrack.reset();

            m_isRecording = false;
        }
        m_trackLock.Unlock();
        return true;
    }
    return false;
}

/*----------------------------------------------------------------------*/
bool CTrackManager::RemoveAllTracks() {
    if (m_trackLock.TryLockBlue()) {

        if (m_selectedTrack) {
            m_selectedTrack->StopRecording();
        }

        m_selectedTrack.reset();
        m_tracks.clear();
        m_trackLock.Unlock();
        return true;
    }
    return false;
}

/*----------------------------------------------------------------------*/
bool CTrackManager::UndoRecording() {
    if (m_trackLock.TryLockBlue()) {
        if (m_selectedTrack)
            m_selectedTrack->UndoRecording();
        m_trackLock.Unlock();
        return true;
    }
    return false;
}

/*----------------------------------------------------------------------*/
bool CTrackManager::StartRecording() {
    if (m_isRecording)
        return true;

    StartPlayback();
    m_isRecording = true;

    if (m_trackLock.TryLockBlue()) {
        for (const auto &track : m_tracks)
            track->SetPosition(m_playbackPosition);

        m_trackLock.Unlock();
    }

    if (m_selectedTrack)
        m_selectedTrack->StartRecording();

//    NMsc::CLogger::Log(NMsc::ELogType::TMP_DEBUG, "CTrackManager: Recording started. Selected track = %", m_selectedTrack.get());

    return true; // todo change return type to void?
}

/*----------------------------------------------------------------------*/
bool CTrackManager::StopRecording() {
    if (!m_isRecording)
        return true;

    m_isRecording = false;
    if (m_selectedTrack)
        m_selectedTrack->StopRecording();

}

/*----------------------------------------------------------------------*/
bool CTrackManager::StartPlayback() {
    if (m_isPlaying)
        return true;

    m_isPlaying = true;

    if (m_trackLock.TryLockBlue()) {
        for (const auto &track : m_tracks)
            track->SetPosition(m_playbackPosition);

        m_trackLock.Unlock();
    }

    return true; // todo change return type to void?
}

/*----------------------------------------------------------------------*/
bool CTrackManager::StopPlayback() {
    if (!m_isPlaying)
        return true;

    StopRecording();
    m_isPlaying = false;
}

/*----------------------------------------------------------------------*/
uint32_t CTrack::PositionToSampleNumber(uint32_t position) {

}

/*----------------------------------------------------------------------*/

bool CTrackManager::IsRecording() {
    return m_isRecording;
}

/*----------------------------------------------------------------------*/

bool CTrackManager::IsPlaying() {
    return m_isPlaying;
}

/*----------------------------------------------------------------------*/
bool CTrackManager::SetPlaybackPosition(uint32_t position) {
    if (m_trackLock.TryLockBlue()) {

        m_playbackPosition = position;
        for (auto &&track : m_tracks) {
            track->SetPosition(position);
        }

        m_trackLock.Unlock();
        return true;
    }

    return false;
}

/*----------------------------------------------------------------------*/
uint32_t CTrackManager::GetPlaybackPosition() {
    return m_playbackPosition;
}














