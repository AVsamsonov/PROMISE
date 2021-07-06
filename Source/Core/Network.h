//
// Created by Alex Samsonov on 31/05/21.
//

#pragma once

#include <JuceHeader.h>

#include "Node.h"

namespace PROMISE
{
namespace Core
{

class Node;

class Network : public Node
{
public:
    //==============================================================================
    Network() noexcept = default;
    explicit Network(const Node& node);
    explicit Network(NodeData &nodeData) noexcept;
    ~Network() override = default;

    //==============================================================================
    static void initDefaultNodeData(NodeData &data) noexcept;
    static juce::Identifier getTypeIdentifier();

    //==============================================================================
    bool isNetwork() const noexcept override;
    Node createNode(const juce::String& type);
    Node createNode(const juce::String& type, const juce::String& name);
    juce::Array<Node> getChildren() const noexcept;
    Node getNode(const juce::String& name) const noexcept;

    //==============================================================================
    bool operator==(const Network& other) const noexcept;
    bool operator!=(const Network& other) const noexcept;

protected:
    //==============================================================================
    virtual NodeData nodeDataFactory(const juce::String& nodeType) const noexcept;
    friend class Node;
    void connectNodes(const Node& srcNode, const juce::String& srcOutputPlugName,
                      const Node& tgtNode, const juce::String& tgtInputPlugName);

private:
    //==============================================================================
    static juce::Identifier getConnectionsPropertyIdentifier();
    juce::String ensureUniqueName(const juce::String& name) const noexcept;
};

}
}