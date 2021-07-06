//
// Created by Alex Samsonov on 31/05/21.
//

#include <JuceHeader.h>
#include "Network.h"
#include <stdexcept>

namespace PROMISE
{
namespace Core
{

//==============================================================================
Network::Network(NodeData &nodeData) noexcept
    : Node(nodeData)
{
}

Network::Network(const Node& node)
{
    data = node.getData();
}

//==============================================================================
void Network::initDefaultNodeData(NodeData &d) noexcept
{
    Node::initDefaultNodeData(d);
    d.setProperty(getConnectionsPropertyIdentifier(), juce::Array<juce::var>(), nullptr);
}

juce::Identifier Network::getTypeIdentifier()
{
    static auto identifier = juce::Identifier("Network");
    return identifier;
}

//==============================================================================
juce::Identifier Network::getConnectionsPropertyIdentifier()
{
    static auto identifier = juce::Identifier("Network::Connections");
    return identifier;
}

bool Network::isNetwork() const noexcept
{
    return true;
}

Node Network::createNode(const String &type, const String &name)
{
    NodeData newNodeData = nodeDataFactory(type);
    if (!newNodeData.isValid()) {
        throw std::runtime_error("Invalid node type");
    }
    data.appendChild(newNodeData, nullptr);
    auto newNode = Node(newNodeData);
    newNode.setName(ensureUniqueName(name));
    return newNode;
}

Node Network::createNode(const String &type)
{
    return createNode(type, type);
}

juce::String Network::ensureUniqueName(const String &name) const noexcept
{
    int suffix = 1;
    auto newName = name;
    while (true) {
        bool done = true;
        for (const auto& childNode : getChildren()) {
            if (childNode.getName() == newName) {
                newName = name + juce::String(suffix);
                ++suffix;
                done = false;
                break;
            }
        }
        if (done)
            return newName;
    }
}

juce::Array<Node> Network::getChildren() const noexcept
{
    juce::Array<Node> result;
    for (auto childData : data) {
        auto childNode = Node(childData);
        result.add(childNode);
    }
    return result;
}

Node Network::getNode(const String &name) const noexcept
{
    for (const auto& node : getChildren()) {
        if (node.getName() == name) {
            return node;
        }
    }
    return Node();
}

//==============================================================================
bool Network::operator==(const Network &other) const noexcept
{
    return (data == other.data);
}

bool Network::operator!=(const Network &other) const noexcept
{
    return (data != other.data);
}

//==============================================================================
NodeData Network::nodeDataFactory(const juce::String& nodeType) const noexcept
{
    NodeData newNodeData;
    if (juce::Identifier(nodeType) == Node::getTypeIdentifier()) {
        newNodeData = NodeData(Node::getTypeIdentifier());
        Node::initDefaultNodeData(newNodeData);
        return newNodeData;
    }
    return newNodeData;
}

void Network::connectNodes(const Node &srcNode, const juce::String &srcOutputPlugName,
                           const Node &tgtNode, const juce::String &tgtInputPlugName)
{
    if (!srcNode.hasOutputPlugWithName(srcOutputPlugName)) {
        auto msg = "Node " + srcNode.getName() + " has no output plug " + srcOutputPlugName;
        throw std::runtime_error(msg.toStdString());
    }

    if (!tgtNode.hasInputPlugWithName(tgtInputPlugName)) {
        auto msg = "Node " + tgtNode.getName() + " has no input plug " + tgtInputPlugName;
        throw std::runtime_error(msg.toStdString());
    }

    juce::Array<var> connection = { srcNode.getName(), srcOutputPlugName, tgtNode.getName(), tgtInputPlugName };
    auto connections = data.getProperty(getConnectionsPropertyIdentifier()).getArray();
    connections->add(connection);
}

}
}