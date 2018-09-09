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
    class CTrackSlice {
    public:

        ~CTrackSlice();

        uint32_t GetId();

        SND_DATA_TYPE *GetBuffer();

        /*bool IsLoaded(); TODO
        void RequestLoad();
        void Unload();*/
        void Delete();


        static CTrackSlice *GetSlice(int32_t id);


        static CTrackSlice *GetNewSlice();

        static void DeleteAllSlices();

        static void StartAutomaticAllocation();

        static void StopAutomaticAllocation();

    private:

        CTrackSlice();

        SND_DATA_TYPE *m_buffer;
        uint32_t m_id;

        static NMsc::CLocklessQue<CTrackSlice *> m_newUnusedSlices;
        static std::atomic_int m_newUnusedSliceCnt;

        static NMsc::CLocklessQue<CTrackSlice *> m_unusedSlices;

        static SND_DATA_TYPE m_trashBuffer[NSnd::TRACK_SLICE_BUFFER_LEN];
        static uint32_t m_uniqueIdCounter;

        static std::map<uint32_t, CTrackSlice *> m_sliceDatabase;

        static int m_workerId;

    };
}

#endif //NOI_SOFTWARE_CTRACKSLICE_HPP
