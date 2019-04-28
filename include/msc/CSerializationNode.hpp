//
// Created by ddl_blue on 25.4.19.
//

#ifndef NOI_SOFTWARE_CSERIALIZATIONNODE_HPP
#define NOI_SOFTWARE_CSERIALIZATIONNODE_HPP


#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <ostream>
#include <istream>
#include <unordered_set>

#include "../../3rdParty/rapidjson/document.h"

namespace NMsc {

    class CSerializationNode;

    typedef std::shared_ptr<CSerializationNode> ASerializationNode;

    class CSerializationNode {
    public:
        static ASerializationNode GetNewTopNode();

        ASerializationNode GetNewSubNode(const std::string name);

        void SerializeInt(const std::string name, int64_t value);

        void SerializeDouble(const std::string name, double value);

        void SerializeBool(const std::string name, bool value);

        void SerializeString(const std::string name, const std::string &value);

        void SerializeString(const std::string name, std::string &&value);

        void
        SerializeIntArray(const std::string name, const std::vector<int64_t> &value, bool skipNameRegistration = false);

        void SerializeDoubleArray(const std::string name, const std::vector<double> &value,
                                  bool skipNameRegistration = false);

        void
        SerializeBoolArray(const std::string name, const std::vector<bool> &value, bool skipNameRegistration = false);

        void SerializeStringArray(const std::string name, const std::vector<std::string> &value,
                                  bool skipNameRegistration = false);

        void SerializeIntArray(const std::string name, std::vector<int64_t> &&value, bool skipNameRegistration = false);

        void
        SerializeDoubleArray(const std::string name, std::vector<double> &&value, bool skipNameRegistration = false);

        void SerializeBoolArray(const std::string name, std::vector<bool> &&value, bool skipNameRegistration = false);

        void SerializeStringArray(const std::string name, std::vector<std::string> &&value,
                                  bool skipNameRegistration = false);

        ASerializationNode AddNodeToArray(std::string arrayName);

        int64_t GetInt(const std::string &name);

        double GetDouble(const std::string &name);

        bool GetBool(const std::string &name);

        std::string GetString(const std::string &name);

        ASerializationNode GetSubnode(const std::string &name);

        const std::vector<int64_t> GetIntArray(const std::string name);

        const std::vector<double> GetDoubleArray(const std::string name);

        const std::vector<bool> GetBoolArray(const std::string name);

        const std::vector<std::string> GetStringArray(const std::string name);

        const std::vector<ASerializationNode> GetSubnodeArray(const std::string name);

        std::string GetPath();

        std::string Dump();

        static ASerializationNode Deserialize(std::istream &input);

        CSerializationNode(const std::string &name, const std::string path);

    private:

        /**
         * Add new subvalue name to node.
         * @param name of subvalue
         * @return true if name was not already taken.
         */
        bool RegisterName(std::string name);

        void Dump(rapidjson::Value &node, rapidjson::Document &document);

        void Deserialize(rapidjson::Value &node);

        // Just for debug
        std::string m_name;
        std::string m_path;

        // Names of all subelements
        std::unordered_set<std::string> m_valueNames;

        // Values stored in node
        std::map<std::string, int64_t> m_int;
        std::map<std::string, double> m_double;
        std::map<std::string, bool> m_bool;
        std::map<std::string, std::string> m_string;
        std::map<std::string, ASerializationNode> m_node;

        std::map<std::string, std::vector<int64_t>> m_intArray;
        std::map<std::string, std::vector<double>> m_doubleArray;
        std::map<std::string, std::vector<bool>> m_boolArray;
        std::map<std::string, std::vector<std::string>> m_stringArray;
        std::map<std::string, std::vector<ASerializationNode>> m_nodeArray;
    };
}

#endif //NOI_SOFTWARE_CSERIALIZATIONNODE_HPP
