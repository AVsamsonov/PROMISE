//
// Created by Alex Samsonov on 6/06/21.
//

#include "RacksNet.h"
#include "Rack.h"

namespace PROMISE
{
namespace Core
{

//==============================================================================
RacksNet::RacksNet(NodeData &nodeData)
    : Network(nodeData)
{
}

//==============================================================================
void RacksNet::initDefaultNodeData(NodeData &d) noexcept
{
    Network::initDefaultNodeData(d);
}

//==============================================================================
juce::Identifier RacksNet::getTypeIdentifier()
{
    static auto identifier = juce::Identifier("Racks");
    return identifier;
}

//==============================================================================
NodeData RacksNet::nodeDataFactory(const String &nodeType) const noexcept
{
    NodeData newNodeData;
    if (juce::Identifier(nodeType) == Rack::getTypeIdentifier()) {
        newNodeData = NodeData(Rack::getTypeIdentifier());
        Rack::initDefaultNodeData(newNodeData);
    }
    return newNodeData;
}

}
}