//
// Created by ddl_blue on 26.8.18.
//

#include "../../include/snd/CTrackManager.hpp"
#include "../../include/msc/CMaintainer.hpp"

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
void CTrackManager::ProcessBuffer(SND_DATA_TYPE *input, SND_DATA_TYPE *buffer, int len) {
    if (m_isPlaying && m_trackLock.TryLockBlue()) {

        for (const auto &track : m_tracks) {
            track->ProcessBuffer(input, buffer, len);
        }

        m_playbackPosition += len;
        m_trackLock.Unlock();
    } else {
        // TODO log warning
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
    StartPlayback();
    m_isRecording = true;

    if (m_trackLock.TryLockBlue()) {
        for (const auto &track : m_tracks)
            track->SetPosition(m_playbackPosition);

        m_trackLock.Unlock();
    }

    return true; // todo change return type to void?
}

/*----------------------------------------------------------------------*/
bool CTrackManager::StopRecording() {
    m_isRecording = false;
    if (m_selectedTrack)
        m_selectedTrack->StopRecording();

}

/*----------------------------------------------------------------------*/
bool CTrackManager::StartPlayback() {
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
    StopRecording();
    m_isPlaying = false;
}

/*----------------------------------------------------------------------*/
uint32_t CTrack::PositionToSampleNumber(uint32_t position) {

}
