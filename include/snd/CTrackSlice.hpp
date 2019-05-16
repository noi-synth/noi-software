//
// Created by ddl_blue on 25.8.18.
//

#ifndef NOI_SOFTWARE_CTRACKSLICE_HPP
#define NOI_SOFTWARE_CTRACKSLICE_HPP

#include <map>
#include <atomic>
#include "NSndConfig.hpp"
#include "../msc/CLocklessQue.hpp"

namespace NSnd {
    /**
     * Represents short chunk of track audio data. Tracks are made of those slices.
     * Also holds database of those slices and does forward allocation of them.
     */
    class CTrackSlice {
    public:

        ~CTrackSlice();

        /**
         * Get unique ID of this slice
         * @return unique ID
         */
        uint32_t GetId();

        /**
         * Get raw audio buffer of this slice
         * @return audio buffer
         */
        SND_DATA_TYPE *GetBuffer();

        /*bool IsLoaded(); TODO
        void RequestLoad();
        void Unload();*/

        /**
         * Delete this slice.
         */
        void Delete();

        /**
         * Wipe the slice with zeros
         */
        void ClearSample();

        /**
         * Duplicate this slice and copy content
         * @return Clone of this slice
         */
        CTrackSlice *Clone();

        /**
         * Get slice by it's ID
         * @param id slice's ID
         * @return found CTrackSlice, or nullptr if not found.
         */
        static CTrackSlice *GetSlice(int32_t id);

        /**
         * Get new unused slice that was allocated before.
         * @return New slice
         */
        static CTrackSlice *GetNewSlice();

        /**
         * Clear database of slices.
         */
        static void DeleteAllSlices();

        /**
         * Start an automatic allocation of free unused slices.
         */
        static void StartAutomaticAllocation();

        /**
         * Stop an automatic allocation of free unused slices.
         */
        static void StopAutomaticAllocation();

    private:

        CTrackSlice();

        /// Raw audio buffer
        SND_DATA_TYPE *m_buffer;

        /// ID of this slice
        uint64_t m_id;


        /// Preallocated slices that were never used
        static NMsc::CLocklessQue<CTrackSlice *> m_newUnusedSlices;

        /// Number of never used slices
        static std::atomic_int m_newUnusedSliceCnt;

        /// Deleted slices for reuse
        static NMsc::CLocklessQue<CTrackSlice *> m_unusedSlices;


        /// Trash buffer for slices with no buffer
        static SND_DATA_TYPE m_trashBuffer[NSnd::TRACK_SLICE_BUFFER_LEN];

        /// Counts IDs, so each new slice get unique ID
        static uint64_t m_uniqueIdCounter;

        /// Global database of slices by their ID
        static std::map<uint64_t, CTrackSlice *> m_sliceDatabase;

        /// CMaintainer worker, that pre-allocates samples.
        static int m_workerId;

    };
}

#endif //NOI_SOFTWARE_CTRACKSLICE_HPP
