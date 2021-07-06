//
// Created by Alex Samsonov on 17/06/21.
//

#include "Rack.h"
#include "RackNodes/In.h"

namespace PROMISE
{
namespace Core
{


//==============================================================================
Rack::Rack() noexcept
    : Network()
{
}

Rack::Rack(const Node& node)
{
    data = node.getData();
}

Rack::Rack(NodeData &nodeData) noexcept
    : Network(nodeData)
{
}

//==============================================================================
void Rack::initDefaultNodeData(NodeData &d) noexcept
{
    Network::initDefaultNodeData(d);
}

juce::Identifier Rack::getTypeIdentifier()
{
    static auto identifier = juce::Identifier("Rack");
    return identifier;
}

//==============================================================================
NodeData Rack::nodeDataFactory(const String &nodeType) const noexcept
{
    NodeData newNodeData;
    if (juce::Identifier(nodeType) == In::getTypeIdentifier()) {
        newNodeData = NodeData(In::getTypeIdentifier());
        In::initDefaultNodeData(newNodeData);
    }
    return newNodeData;
}


}
}
