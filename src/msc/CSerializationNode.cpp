//
// Created by ddl_blue on 25.4.19.
//

#include "../../include/msc/CSerializationNode.hpp"
#include "../../include/msc/CLogger.hpp"

#include "../../3rdParty/rapidjson/writer.h"

#include <sstream>

using namespace NMsc;

/*----------------------------------------------------------------------*/
ASerializationNode CSerializationNode::GetNewTopNode() {
    return std::make_shared<CSerializationNode>("root", "");
}

/*----------------------------------------------------------------------*/
ASerializationNode CSerializationNode::GetNewSubNode(const std::string name, bool skipNameRegistration) {
    if (!skipNameRegistration && !RegisterName(name)) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CSerializationNode: duplicate name of subnode.\n\tName=%\n\tPath=%",
                           name, GetPath());
        return nullptr;
    }

    ASerializationNode newNode = std::make_shared<CSerializationNode>(name, m_path);
    m_node.emplace(name, newNode);
    return newNode;
}

/*----------------------------------------------------------------------*/
void CSerializationNode::SerializeInt(const std::string name, int64_t value, bool skipNameRegistration) {
    if (!skipNameRegistration && !RegisterName(name)) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR,
                           "CSerializationNode: duplicate name of value int.\n\tName=%\n\tPath=%", name, GetPath());
        return;
    }
    m_int.emplace(name, value);
}

/*----------------------------------------------------------------------*/
void CSerializationNode::SerializeDouble(const std::string name, double value, bool skipNameRegistration) {
    if (!skipNameRegistration && !RegisterName(name)) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR,
                           "CSerializationNode: duplicate name of value double.\n\tName=%\n\tPath=%", name, GetPath());
        return;
    }
    m_double.emplace(name, value);
}

/*----------------------------------------------------------------------*/
void CSerializationNode::SerializeBool(const std::string name, bool value, bool skipNameRegistration) {
    if (!skipNameRegistration && !RegisterName(name)) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR,
                           "CSerializationNode: duplicate name of value bool.\n\tName=%\n\tPath=%", name, GetPath());
        return;
    }
    m_bool.emplace(name, value);
}

/*----------------------------------------------------------------------*/
void CSerializationNode::SerializeString(const std::string name, const std::string &value, bool skipNameRegistration) {
    if (!skipNameRegistration && !RegisterName(name)) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR,
                           "CSerializationNode: duplicate name of value string.\n\tName=%\n\tPath=%", name, GetPath());
        return;
    }
    m_string.emplace(name, value);
}

/*----------------------------------------------------------------------*/
void CSerializationNode::SerializeString(const std::string name, std::string &&value, bool skipNameRegistration) {
    if (!skipNameRegistration && !RegisterName(name)) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR,
                           "CSerializationNode: duplicate name of value string.\n\tName=%\n\tPath=%", name, GetPath());
        return;
    }
    m_string.emplace(name, value);
}

/*----------------------------------------------------------------------*/
void CSerializationNode::SerializeIntArray(const std::string name, const std::vector<int64_t> &value,
                                           bool skipNameRegistration) {
    if (!skipNameRegistration && !RegisterName(name)) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR,
                           "CSerializationNode: duplicate name of value int array.\n\tName=%\n\tPath=%", name,
                           GetPath());
        return;
    }
    m_intArray.emplace(name, value);
}

/*----------------------------------------------------------------------*/
void CSerializationNode::SerializeDoubleArray(const std::string name, const std::vector<double> &value,
                                              bool skipNameRegistration) {
    if (!skipNameRegistration && !RegisterName(name)) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR,
                           "CSerializationNode: duplicate name of value double array.\n\tName=%\n\tPath=%", name,
                           GetPath());
        return;
    }
    m_doubleArray.emplace(name, value);
}

/*----------------------------------------------------------------------*/
void CSerializationNode::SerializeBoolArray(const std::string name, const std::vector<bool> &value,
                                            bool skipNameRegistration) {
    if (!skipNameRegistration && !RegisterName(name)) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR,
                           "CSerializationNode: duplicate name of value bool array.\n\tName=%\n\tPath=%", name,
                           GetPath());
        return;
    }
    m_boolArray.emplace(name, value);
}

/*----------------------------------------------------------------------*/
void CSerializationNode::SerializeStringArray(const std::string name, const std::vector<std::string> &value,
                                              bool skipNameRegistration) {
    if (!skipNameRegistration && !RegisterName(name)) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR,
                           "CSerializationNode: duplicate name of value string array.\n\tName=%\n\tPath=%", name,
                           GetPath());
        return;
    }
    m_stringArray.emplace(name, value);
}

/*----------------------------------------------------------------------*/
void
CSerializationNode::SerializeIntArray(const std::string name, std::vector<int64_t> &&value, bool skipNameRegistration) {
    if (!skipNameRegistration && !RegisterName(name)) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR,
                           "CSerializationNode: duplicate name of value int array.\n\tName=%\n\tPath=%", name,
                           GetPath());
        return;
    }
    m_intArray.emplace(name, value);
}

/*----------------------------------------------------------------------*/
void CSerializationNode::SerializeDoubleArray(const std::string name, std::vector<double> &&value,
                                              bool skipNameRegistration) {
    if (!skipNameRegistration && !RegisterName(name)) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR,
                           "CSerializationNode: duplicate name of value double array.\n\tName=%\n\tPath=%", name,
                           GetPath());
        return;
    }
    m_doubleArray.emplace(name, value);
}

/*----------------------------------------------------------------------*/
void
CSerializationNode::SerializeBoolArray(const std::string name, std::vector<bool> &&value, bool skipNameRegistration) {
    if (!skipNameRegistration && !RegisterName(name)) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR,
                           "CSerializationNode: duplicate name of value bool array.\n\tName=%\n\tPath=%", name,
                           GetPath());
        return;
    }
    m_boolArray.emplace(name, value);
}

/*----------------------------------------------------------------------*/
void CSerializationNode::SerializeStringArray(const std::string name, std::vector<std::string> &&value,
                                              bool skipNameRegistration) {
    if (!skipNameRegistration && !RegisterName(name)) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR,
                           "CSerializationNode: duplicate name of value string array.\n\tName=%\n\tPath=%", name,
                           GetPath());
        return;
    }
    m_stringArray.emplace(name, value);
}

/*----------------------------------------------------------------------*/
ASerializationNode CSerializationNode::AddNodeToArray(std::string arrayName) {
    // If array does not exist
    if (m_nodeArray.find(arrayName) == m_nodeArray.end())
        m_nodeArray.emplace(arrayName, std::vector<ASerializationNode>());

    std::vector<ASerializationNode> &array = m_nodeArray[arrayName];

    std::string nodeName = arrayName + "[" + std::to_string(array.size()) + "]";
    ASerializationNode newNode = std::make_shared<CSerializationNode>(nodeName, m_path);
    array.push_back(newNode);

    return newNode;
}

/*----------------------------------------------------------------------*/
int64_t CSerializationNode::GetInt(const std::string &name) {
    auto res = m_int.find(name);
    if (res == m_int.end()) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CSerializationNode: Int value not found.\n\tName=%\n\tPath=%", name,
                           GetPath());
        return 0;
    }
    return m_int[name];
}

/*----------------------------------------------------------------------*/
double CSerializationNode::GetDouble(const std::string &name) {
    auto res = m_double.find(name);
    if (res == m_double.end()) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CSerializationNode: Double value not found.\n\tName=%\n\tPath=%",
                           name, GetPath());
        return 0;
    }
    return m_double[name];
}

/*----------------------------------------------------------------------*/
bool CSerializationNode::GetBool(const std::string &name) {
    auto res = m_bool.find(name);
    if (res == m_bool.end()) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CSerializationNode: Bool value not found.\n\tName=%\n\tPath=%", name,
                           GetPath());
        return false;
    }
    return m_bool[name];
}

/*----------------------------------------------------------------------*/
std::string CSerializationNode::GetString(const std::string &name) {
    auto res = m_string.find(name);
    if (res == m_string.end()) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CSerializationNode: String value not found.\n\tName=%\n\tPath=%",
                           name, GetPath());
        return "";
    }
    return m_string[name];
}

/*----------------------------------------------------------------------*/
ASerializationNode CSerializationNode::GetSubnode(const std::string &name) {
    auto res = m_node.find(name);
    if (res == m_node.end()) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CSerializationNode: Subnode not found.\n\tName=%\n\tPath=%", name,
                           GetPath());
        return nullptr;
    }
    return m_node[name];
}

/*----------------------------------------------------------------------*/
const std::vector<int64_t> CSerializationNode::GetIntArray(const std::string name) {
    auto res = m_intArray.find(name);
    if (res == m_intArray.end()) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CSerializationNode: Int array not found.\n\tName=%\n\tPath=%", name,
                           GetPath());
        return std::vector<int64_t>();
    }
    return m_intArray[name];
}

/*----------------------------------------------------------------------*/
const std::vector<double> CSerializationNode::GetDoubleArray(const std::string name) {
    auto res = m_doubleArray.find(name);
    if (res == m_doubleArray.end()) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CSerializationNode: Double array not found.\n\tName=%\n\tPath=%",
                           name, GetPath());
        return std::vector<double>();
    }
    return m_doubleArray[name];
}

/*----------------------------------------------------------------------*/
const std::vector<bool> CSerializationNode::GetBoolArray(const std::string name) {
    auto res = m_boolArray.find(name);
    if (res == m_boolArray.end()) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CSerializationNode: Bool array not found.\n\tName=%\n\tPath=%", name,
                           GetPath());
        return std::vector<bool>();
    }
    return m_boolArray[name];
}

/*----------------------------------------------------------------------*/
const std::vector<std::string> CSerializationNode::GetStringArray(const std::string name) {
    auto res = m_stringArray.find(name);
    if (res == m_stringArray.end()) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CSerializationNode: String array not found.\n\tName=%\n\tPath=%",
                           name, GetPath());
        return std::vector<std::string>();
    }
    return m_stringArray[name];
}

/*----------------------------------------------------------------------*/
const std::vector<ASerializationNode> CSerializationNode::GetSubnodeArray(const std::string name) {
    auto res = m_nodeArray.find(name);
    if (res == m_nodeArray.end()) {
        NMsc::CLogger::Log(NMsc::ELogType::ERROR, "CSerializationNode: Subnode array not found.\n\tName=%\n\tPath=%",
                           name, GetPath());
        return std::vector<ASerializationNode>();
    }
    return m_nodeArray[name];
}

/*----------------------------------------------------------------------*/
std::string CSerializationNode::GetPath() {
    return m_path;
}

/*----------------------------------------------------------------------*/
std::string CSerializationNode::Dump() {
    rapidjson::Document document;
    document.SetObject();

    // Create root node
    rapidjson::Value rootName;
    rootName.SetString(m_name.c_str(), m_name.length(), document.GetAllocator());
    rapidjson::Value rootNode;
    rootNode.SetObject();

    // Dump CSerializationNode tree into rapidJson tree
    Dump(rootNode, document);

    // Add root node to document
    document.AddMember(rootName, rootNode, document.GetAllocator());

    // Get string
    rapidjson::StringBuffer buffer;

    buffer.Clear();
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    return std::string(buffer.GetString());
}

/*----------------------------------------------------------------------*/
ASerializationNode CSerializationNode::Deserialize(std::istream &input) {
    // todo
    return nullptr;
}

/*----------------------------------------------------------------------*/
CSerializationNode::CSerializationNode(const std::string &name, const std::string path) : m_name(name) {
    m_path = path + "/" + name;
}

/*----------------------------------------------------------------------*/
bool CSerializationNode::RegisterName(std::string name) {
    if (m_valueNames.find(name) == m_valueNames.end()) {
        m_valueNames.emplace(name);
        return true;
    }
    return false;
}

/*----------------------------------------------------------------------*/
void CSerializationNode::Dump(rapidjson::Value &node, rapidjson::Document &document) {

    node.SetObject();

    // Int
    for (const auto &item : m_int) {
        // value
        rapidjson::Value value;
        value.SetInt64((item.second));
        // name
        rapidjson::Value name;
        name.SetString(item.first.c_str(), item.first.length(), document.GetAllocator());

        // add it
        node.AddMember(name, value, document.GetAllocator());
    }

    // Double
    for (const auto &item : m_double) {
        // value
        rapidjson::Value value;
        value.SetDouble(item.second);
        // name
        rapidjson::Value name;
        name.SetString(item.first.c_str(), item.first.length(), document.GetAllocator());

        // add it
        node.AddMember(name, value, document.GetAllocator());
    }

    // Bool
    for (const auto &item : m_bool) {
        // value
        rapidjson::Value value;
        value.SetBool(item.second);
        // name
        rapidjson::Value name;
        name.SetString(item.first.c_str(), item.first.length(), document.GetAllocator());

        // add it
        node.AddMember(name, value, document.GetAllocator());
    }

    // String
    for (const auto &item : m_string) {
        // value
        rapidjson::Value value;
        value.SetString(item.second.c_str(), item.second.length(), document.GetAllocator());
        // name
        rapidjson::Value name;
        name.SetString(item.first.c_str(), item.first.length(), document.GetAllocator());

        // add it
        node.AddMember(name, value, document.GetAllocator());
    }

    // Subnodes
    for (const auto &item : m_node) {
        // value
        rapidjson::Value value;
        value.SetObject();

        item.second->Dump(value, document);

        // name
        rapidjson::Value name;
        name.SetString(item.first.c_str(), item.first.length(), document.GetAllocator());

        // add it
        node.AddMember(name, value, document.GetAllocator());
    }

    // Int array
    for (const auto &item : m_intArray) {
        // value
        rapidjson::Value value;
        value.SetArray();
        value.Reserve(item.second.size(), document.GetAllocator());
        // Copy array
        for (const auto &arrItem : item.second) {
            rapidjson::Value arrValue;
            arrValue.SetInt64(arrItem);
            value.PushBack(arrValue, document.GetAllocator());
        }

        // name
        rapidjson::Value name;
        name.SetString(item.first.c_str(), item.first.length(), document.GetAllocator());

        // add it
        node.AddMember(name, value, document.GetAllocator());
    }

    // Double array
    for (const auto &item : m_doubleArray) {
        // value
        rapidjson::Value value;
        value.SetArray();
        value.Reserve(item.second.size(), document.GetAllocator());
        // Copy array
        for (const auto &arrItem : item.second) {
            rapidjson::Value arrValue;
            arrValue.SetDouble(arrItem);
            value.PushBack(arrValue, document.GetAllocator());
        }

        // name
        rapidjson::Value name;
        name.SetString(item.first.c_str(), item.first.length(), document.GetAllocator());

        // add it
        node.AddMember(name, value, document.GetAllocator());
    }

    // Bool array
    for (const auto &item : m_boolArray) {
        // value
        rapidjson::Value value;
        value.SetArray();
        value.Reserve(item.second.size(), document.GetAllocator());
        // Copy array
        for (const auto &arrItem : item.second) {
            rapidjson::Value arrValue;
            arrValue.SetBool(arrItem);
            value.PushBack(arrValue, document.GetAllocator());
        }

        // name
        rapidjson::Value name;
        name.SetString(item.first.c_str(), item.first.length(), document.GetAllocator());

        // add it
        node.AddMember(name, value, document.GetAllocator());
    }

    // String array
    for (const auto &item : m_stringArray) {
        // value
        rapidjson::Value value;
        value.SetArray();
        value.Reserve(item.second.size(), document.GetAllocator());
        // Copy array
        for (const auto &arrItem : item.second) {
            rapidjson::Value arrValue;
            arrValue.SetString(arrItem.c_str(), arrItem.length(), document.GetAllocator());
            value.PushBack(arrValue, document.GetAllocator());
        }

        // name
        rapidjson::Value name;
        name.SetString(item.first.c_str(), item.first.length(), document.GetAllocator());

        // add it
        node.AddMember(name, value, document.GetAllocator());
    }

    // Bool array
    for (const auto &item : m_nodeArray) {
        // value
        rapidjson::Value value;
        value.SetArray();
        value.Reserve(item.second.size(), document.GetAllocator());
        // Copy array
        for (const auto &arrItem : item.second) {
            rapidjson::Value arrValue;
            arrValue.SetObject();
            arrItem->Dump(arrValue, document);
            value.PushBack(arrValue, document.GetAllocator());
        }

        // name
        rapidjson::Value name;
        name.SetString(item.first.c_str(), item.first.length(), document.GetAllocator());

        // add it
        node.AddMember(name, value, document.GetAllocator());
    }

}

/*----------------------------------------------------------------------*/
void CSerializationNode::Deserialize(rapidjson::Value &node) {
    for (auto &val : node.GetObject()) {

        // todo nejednoznačnost
        // Int
        if (val.value.IsInt())
            SerializeInt(val.name.GetString(), val.value.GetInt64());
            // Double
        else if (val.value.IsDouble())
            SerializeDouble(val.name.GetString(), val.value.GetDouble());
        // Bool
        if (val.value.IsBool())
            SerializeBool(val.name.GetString(), val.value.GetBool());
        // String
        if (val.value.IsString())
            SerializeString(val.name.GetString(), val.value.GetString());
        // Subnode
        if (val.value.IsObject()) {
            ASerializationNode newSubnode = GetNewSubNode(val.name.GetString());
            newSubnode->Deserialize(val.value);
        } else if (val.value.IsArray()) {
            rapidjson::Value &arr = val.value;

            // register name
            if (!RegisterName(val.name.GetString())) {
                NMsc::CLogger::Log(NMsc::ELogType::ERROR,
                                   "CSerializationNode: Deserialization: duplicate name of an array.\n\tName=%\n\tPath=%",
                                   val.name.GetString(),
                                   GetPath());
            }


            // Filter array types
            bool intArr, doubleArr, boolArr, stringArr, nodeArr;
            intArr = doubleArr = boolArr = stringArr = nodeArr = true;

            for (auto &arrVal : arr.GetArray()) {
                intArr &= arrVal.IsInt64();
                doubleArr &= arrVal.IsDouble();
                boolArr &= arrVal.IsBool();
                stringArr &= arrVal.IsString();
                nodeArr &= arrVal.IsObject();
            }

            if (!(intArr || doubleArr || boolArr || stringArr || nodeArr)) {
                NMsc::CLogger::Log(NMsc::ELogType::WARNING,
                                   "CSerializationNode: Deserialization: Array is not homogeneous, skipping.  \n\tName=%\n\tPath=%",
                                   val.name.GetString(),
                                   GetPath());
                continue;
            }

            // Int array
            if (intArr) {
                std::vector<int64_t> vctr;
                vctr.reserve(arr.Size());
                // Copy all values.
                for (auto &arrVal : arr.GetArray()) {
                    vctr.push_back(arrVal.GetInt64());
                }
                SerializeIntArray(val.name.GetString(), std::move(vctr), true);
            }

            // Double array
            if (doubleArr) {
                std::vector<double> vctr;
                vctr.reserve(arr.Size());
                // Copy all values.
                for (auto &arrVal : arr.GetArray()) {
                    vctr.push_back(arrVal.GetDouble());
                }
                SerializeDoubleArray(val.name.GetString(), std::move(vctr), true);
            }

            // Double array
            if (doubleArr) {
                std::vector<double> vctr;
                vctr.reserve(arr.Size());
                // Copy all values.
                for (auto &arrVal : arr.GetArray()) {
                    vctr.push_back(arrVal.GetDouble());
                }
                SerializeDoubleArray(val.name.GetString(), std::move(vctr), true);
            }

        }

    }
}
