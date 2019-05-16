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

    /**
     * Node of a serialization tree. Contains data for serialization. Can be serialized to JSON, or deserialized from it.
     */
    class CSerializationNode {
    public:
        /**
         * Get new top node of a tree node structure. Used when creating new serialization tree to serialize the project.
         * @return New top node for tree
         */
        static ASerializationNode GetNewTopNode();

        /**
         * Create a new subnode for this node
         * @param name Name of this node
         * @return New node
         * @param skipNameRegistration Skip check for unique name
         */
        ASerializationNode GetNewSubNode(const std::string name, bool skipNameRegistration = false);

        /**
         * Add an Int64 value to the node.
         * @param name Name of the value
         * @param value The value
         * @param skipNameRegistration Skip check for unique name
         */
        void SerializeInt(const std::string name, int64_t value, bool skipNameRegistration = false);

        /**
         * Add an double value to the node.
         * @param name Name of the value
         * @param value The value
         * @param skipNameRegistration Skip check for unique name
         */
        void SerializeDouble(const std::string name, double value, bool skipNameRegistration = false);

        /**
         * Add an bool value to the node.
         * @param name Name of the value
         * @param value The value
         * @param skipNameRegistration Skip check for unique name
         */
        void SerializeBool(const std::string name, bool value, bool skipNameRegistration = false);

        /**
         * Add an string value to the node.
         * @param name Name of the value
         * @param value The value
         * @param skipNameRegistration Skip check for unique name
         */
        void SerializeString(const std::string name, const std::string &value, bool skipNameRegistration = false);

        /**
         * Add an string value to the node.
         * @param name Name of the value
         * @param value The value
         * @param skipNameRegistration Skip check for unique name
         */
        void SerializeString(const std::string name, std::string &&value, bool skipNameRegistration = false);

        /**
         * Add INT64 array to the node
         * @param name Name of the array
         * @param value The array
         * @param skipNameRegistration Skip check for unique name
         */
        void
        SerializeIntArray(const std::string name, const std::vector<int64_t> &value, bool skipNameRegistration = false);

        /**
         * Add double array to the node
         * @param name Name of the array
         * @param value The array
         * @param skipNameRegistration Skip check for unique name
         */
        void SerializeDoubleArray(const std::string name, const std::vector<double> &value,
                                  bool skipNameRegistration = false);

        /**
         * Add bool array to the node
         * @param name Name of the array
         * @param value The array
         * @param skipNameRegistration Skip check for unique name
         */
        void
        SerializeBoolArray(const std::string name, const std::vector<bool> &value, bool skipNameRegistration = false);

        /**
         * Add string array to the node
         * @param name Name of the array
         * @param value The array
         * @param skipNameRegistration Skip check for unique name
         */
        void SerializeStringArray(const std::string name, const std::vector<std::string> &value,
                                  bool skipNameRegistration = false);

        /**
         * Add INT64 array to the node
         * @param name Name of the array
         * @param value The array
         * @param skipNameRegistration Skip check for unique name
         */
        void SerializeIntArray(const std::string name, std::vector<int64_t> &&value, bool skipNameRegistration = false);

        /**
         * Add double array to the node
         * @param name Name of the array
         * @param value The array
         * @param skipNameRegistration Skip check for unique name
         */
        void
        SerializeDoubleArray(const std::string name, std::vector<double> &&value, bool skipNameRegistration = false);

        /**
         * Add bool array to the node
         * @param name Name of the array
         * @param value The array
         * @param skipNameRegistration Skip check for unique name
         */
        void SerializeBoolArray(const std::string name, std::vector<bool> &&value, bool skipNameRegistration = false);

        /**
         * Add string array to the node
         * @param name Name of the array
         * @param value The array
         * @param skipNameRegistration Skip check for unique name
         */
        void SerializeStringArray(const std::string name, std::vector<std::string> &&value,
                                  bool skipNameRegistration = false);

        /**
         * Add new node to a node array
         * @param arrayName Name of the array
         * @return Node that was added into the array
         */
        ASerializationNode AddNodeToArray(std::string arrayName);

        /**
         * Get value from the node. If not found, returns a default value and writes a message into the log.
         * @param name Name of the value
         * @return The value. Default value if not found.
         */
        int64_t GetInt(const std::string &name);

        /**
         * Get value from the node. If not found, returns a default value and writes a message into the log.
         * @param name Name of the value
         * @return The value. Default value if not found.
         */
        double GetDouble(const std::string &name);

        /**
         * Get value from the node. If not found, returns a default value and writes a message into the log.
         * @param name Name of the value
         * @return The value. Default value if not found.
         */
        bool GetBool(const std::string &name);

        /**
         * Get value from the node. If not found, returns a default value and writes a message into the log.
         * @param name Name of the value
         * @return The value. Default value if not found.
         */
        std::string GetString(const std::string &name);

        /**
         * Get subnode from the node. If not found, returns a empty node and writes a message into the log.
         * @param name Name of the subnode
         * @return The subnode. Empty node if not found
         */
        ASerializationNode GetSubnode(const std::string &name);

        /**
         * Get int array from the node. If not found, returns empty array and writes a message into the log.
         * @param name Name of the array
         * @return The found array or empty array otherwise
         */
        const std::vector<int64_t> GetIntArray(const std::string name);

        /**
         * Get double array from the node. If not found, returns empty array and writes a message into the log.
         * @param name Name of the array
         * @return The found array or empty array otherwise
         */
        const std::vector<double> GetDoubleArray(const std::string name);

        /**
         * Get bool array from the node. If not found, returns empty array and writes a message into the log.
         * @param name Name of the array
         * @return The found array or empty array otherwise
         */
        const std::vector<bool> GetBoolArray(const std::string name);

        /**
         * Get int string from the node. If not found, returns empty array and writes a message into the log.
         * @param name Name of the array
         * @return The found array or empty array otherwise
         */
        const std::vector<std::string> GetStringArray(const std::string name);

        /**
         * Get subnode array from the node. If not found, returns empty array and writes a message into the log.
         * @param name Name of the array
         * @return The found array or empty array otherwise
         */
        const std::vector<ASerializationNode> GetSubnodeArray(const std::string name);

        /**
         * Get path from root node to this node.
         * @return Path of this node
         */
        std::string GetPath();

        /**
         * Serialize this node and this subtree
         * @return This subtree serialized to JSON
         */
        std::string Dump();

        /**
         * Create a node tree from JSON
         * @param input Input stream with JSON
         * @return Root node of the deserialized tree
         */
        static ASerializationNode Deserialize(std::istream &input);

        /**
         * Create new serialization node. For internal purposes only
         * @param name Name of this node
         * @param path Path to this node
         */
        CSerializationNode(const std::string &name, const std::string path);

    private:

        /**
         * Add new subvalue name to node.
         * @param name of subvalue
         * @return true if name was not already taken.
         */
        bool RegisterName(std::string name);

        /**
         * Recursive function that creates RapidJson tree from CSerializationNode tree
         * @param node RapidJson node
         * @param document RapidJson document
         */
        void Dump(rapidjson::Value &node, rapidjson::Document &document);

        /**
         * Fills this node with information from RapidJson node
         * @param node RapidJson node
         */
        void Deserialize(rapidjson::Value &node);

        // Just for debug
        /// Name of this node
        std::string m_name;
        /// Path from root to this node
        std::string m_path;

        // Names of all subelements
        /// Names of all values. Used for checks for unique value names
        std::unordered_set<std::string> m_valueNames;

        // Values stored in node
        /// Values stored in the node
        std::map<std::string, int64_t> m_int;
        /// Values stored in the node
        std::map<std::string, double> m_double;
        /// Values stored in the node
        std::map<std::string, bool> m_bool;
        /// Values stored in the node
        std::map<std::string, std::string> m_string;
        /// Subnodes stored in the node
        std::map<std::string, ASerializationNode> m_node;

        /// Value arrays stored in the node
        std::map<std::string, std::vector<int64_t>> m_intArray;
        /// Value arrays stored in the node
        std::map<std::string, std::vector<double>> m_doubleArray;
        /// Value arrays stored in the node
        std::map<std::string, std::vector<bool>> m_boolArray;
        /// Value arrays stored in the node
        std::map<std::string, std::vector<std::string>> m_stringArray;
        /// Subnode arrays stored in the node
        std::map<std::string, std::vector<ASerializationNode>> m_nodeArray;
    };
}

#endif //NOI_SOFTWARE_CSERIALIZATIONNODE_HPP
