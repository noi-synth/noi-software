//
// Created by ddl_blue on 4.8.19.
//

#ifndef NOI_SOFTWARE_CPLUGININFO_HPP
#define NOI_SOFTWARE_CPLUGININFO_HPP

#include <string>
#include "../include/msc/CSerializationNode.hpp"

namespace NPlg {
    struct CPluginInfo {
        enum EPluginType {
            TYPE_RESERVED = 0,
            TYPE_INSTRUMENT = 1,
            TYPE_EFFECT = 2
        };

        CPluginInfo(const std::string &uniqueName, const std::string &name, const std::string &version,
                    EPluginType pluginType,
                    int32_t serialisationVersion, const std::string &author, const std::string &description,
                    const std::string &implementationNamespace, const std::string &implementationCalssName,
                    const std::string &targetNoiModel);

        CPluginInfo(NMsc::ASerializationNode serializationSource);

        void Serialize(NMsc::ASerializationNode serializationTarget);

        std::string m_uniqueName;
        std::string m_name;
        std::string m_version;
        EPluginType m_pluginType;
        int32_t m_serialisationVersion;
        std::string m_author;
        std::string m_description;

        std::string m_implementationNamespace;
        std::string m_implementationClassName;

        std::string m_targetNoiModel;

    };
}

#endif //NOI_SOFTWARE_CPLUGININFO_HPP
