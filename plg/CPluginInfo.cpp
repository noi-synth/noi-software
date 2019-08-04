//
// Created by ddl_blue on 4.8.19.
//

#include "CPluginInfo.hpp"

using namespace NPlg;

/*----------------------------------------------------------------------*/
CPluginInfo::CPluginInfo(const std::string &uniqueName, const std::string &name, const std::string &version,
                         NPlg::CPluginInfo::EPluginType pluginType, int32_t serialisationVersion,
                         const std::string &author,
                         const std::string &description, const std::string &implementationNamespace,
                         const std::string &implementationCalssName, const std::string &targetNoiModel)
        : m_uniqueName(uniqueName), m_name(name), m_version(version), m_pluginType(pluginType),
          m_serialisationVersion(serialisationVersion), m_author(author), m_description(description),
          m_implementationNamespace(implementationNamespace), m_implementationClassName(m_implementationClassName),
          m_targetNoiModel(targetNoiModel) {

}

/*----------------------------------------------------------------------*/
CPluginInfo::CPluginInfo(NMsc::ASerializationNode serializationSource) {
    m_uniqueName = serializationTarget->DeserializeString("UniqueName");
    m_name = serializationTarget->DeserializeString("Name");
    m_version = serializationTarget->DeserializeString("Version");

    m_author = serializationTarget->DeserializeString("Author");
    m_description = serializationTarget->DeserializeString("Description");
    m_implementationNamespace = serializationTarget->DeserializeString("ImplNamespace");
    m_implementationClassName = serializationTarget->DeserializeString("ImplClassName");
    m_targetNoiModel = serializationTarget->DeserializeString("TargetModel");


}

/*----------------------------------------------------------------------*/
void CPluginInfo::Serialize(NMsc::ASerializationNode serializationTarget) {
    serializationTarget->SerializeString("UniqueName", m_uniqueName);
    serializationTarget->SerializeString("Name", m_name);
    serializationTarget->SerializeString("Version", m_version);
    serializationTarget->SerializeInt("PluginType", (int64_t) m_pluginType);
    serializationTarget->SerializeInt("SerialisationVersion", m_serialisationVersion);
    serializationTarget->SerializeString("Author", m_author);
    serializationTarget->SerializeString("Description", m_description);
    serializationTarget->SerializeString("ImplNamespace", m_implementationNamespace);
    serializationTarget->SerializeString("ImplClassName", m_implementationClassName);
    serializationTarget->SerializeString("TargetModel", m_targetNoiModel);
}