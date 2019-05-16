//
// Created by ddl_blue on 26.8.18.
//

#ifndef NOI_SOFTWARE_CTRACK_HPP
#define NOI_SOFTWARE_CTRACK_HPP

#include <functional>
#include <memory>
#include <vector>
#include "../msc/CTeamLock.hpp"
#include "../msc/CLocklessQue.hpp"
#include "CTrackSlice.hpp"

namespace NSnd {
    /**
     * Represents a single track.
     */
    class CTrack {
    public:
        CTrack();

        ~CTrack();

        /**
         * Generate a (and record) audio data.
         * @param input Buffer with audio input to the track. This will be recorded.
         * @param buffer Buffer with audio output from track playback
         * @param len Length of buffers in frames.
         * @param position Position on the track in samples. Used only on first call after start of playback.
         */
        void ProcessBuffer(SND_DATA_TYPE *input, SND_DATA_TYPE *buffer, unsigned long int len, uint32_t position);

        /***
         * Clears undo and sets new position on next ProcessBuffer(...) call. Deferred call.
         */
        void SetPosition();

        /**
         * Clears undo on next ProcessBuffer(...) call. Deferred call.
         */
        void ClearUndo(); // clears undo :)

        /**
         * Starts recording on next ProcessBuffer(...) call. Deferred call. Clears Undo.
         */
        void StartRecording(); // clears undo

        /**
         * Stops recording on next on next ProcessBuffer(...) call. Deferred call.
         */
        void StopRecording();

        /**
         * Undo last recording and return track to it's state before last recording on next ProcessBuffer(...) call. Deferred call.
         */
        void UndoRecording(); // clears undo

        /**
         * Can this track do an undo?
         * @return True if it can do undo.
         */
        bool CanUndo();

        /**
         * Volume of this track
         */
        float m_volume;

        /**
         * Convert track position to CTrackSlice number
         * @param position Position in samples
         * @return Position in CTrackSlice number
         */
        static inline uint32_t PositionToSampleNumber(uint32_t position);

        /**
         * Convert track position to offset in CTrackSlice
         * @param position Track position in samples
         * @return Offset in CTrackSlice
         */
        static inline uint32_t PositionToSampleOffset(uint32_t position);

        /**
         * Converts CTrackSlice number and offset into positions in samples
         * @param sampleNumber CTrackSlice number
         * @param sampleOffset Offset within CTrackSlice
         * @return Track position in samples
         */
        static inline uint32_t SampleNumberOffsetToPosition(uint32_t sampleNumber, uint32_t sampleOffset);

    private:

        /**
         * Clears undo. Intended for use in RT thread.
         */
        void ClearUndoUnsafe();

        /**
         * Track data is stored in CTrackSlice objects in short chunks.
         */
        std::vector<CTrackSlice *> m_trackData;
        /**
         * Current position in playback in samples
         */
        std::atomic_int m_playbackPosition;
        /**
         * How far in track the playback ever was
         */
        std::atomic_int m_farthestPlaybackSample;
        /**
         * True if position change was requested.
         */
        std::atomic_bool m_shouldUpdatePosition;

        /**
         * True if this track is recording sound input.
         */
        bool m_isRecording;

        /**
         * Que containing all deffered calls. Will be done in RT thread.
         */
        NMsc::CLocklessQue<std::function<void()>> m_quedTrackManipulations;
        // Blue team = Maintainer thread, Red team = RT thread
        /**
         * Lock that protects track data vector. Blue team = Maintainer thread, Red team = RT thread
         */
        NMsc::CTeamLock m_recordingManipulationLock;

        /**
         * Contains backup of CTrackSlices modified on last recording. Used for undo.
         */
        std::vector<CTrackSlice *> m_undoRecording;
        /**
         * Position of first CTrackSlice where last recording started.
         */
        std::atomic_int m_undoRecordingStartSamplePosition;
        /**
         * CTrackSlice position 1 after the last CTrackSlice afected by last recording.
         */
        std::atomic_int m_undoRecordingEndSamplePosition;

        /**
         * Recording from situation, when RT thread could not lock the lock protecting the track data
         */
        SND_DATA_TYPE m_recordingEmergencyBuffer[INTERNAL_BUFFERS_LEN * 3];
        /**
         * Length of record in emergency buffer
         */
        uint32_t m_recordingEmergencyLen;

        /**
         * ID of worker task in CMaintainer
         */
        int m_workerId;

    };

    typedef std::shared_ptr<CTrack> ATrack;
}

#endif //NOI_SOFTWARE_CTRACK_HPP
