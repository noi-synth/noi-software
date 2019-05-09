//
// Created by ddl_blue on 1.5.19.
//

#ifndef NOI_SOFTWARE_CZEROUI_HPP
#define NOI_SOFTWARE_CZEROUI_HPP

#include "../CUi.hpp"
#include "../CInputEventInfo.hpp"

namespace NUi::NZero {
    class CZeroUi : public CUi {
    public:
        CZeroUi(NLgc::ANoiApp app);

        void ProcessInput(CInptutEventInfo input);

    private:
        virtual void UiThreadWorker() override;
    };

    typedef std::shared_ptr<CZeroUi> AZeroUi;
}


#endif //NOI_SOFTWARE_CZEROUI_HPP
