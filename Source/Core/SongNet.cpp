//
// Created by Alex Samsonov on 31/05/21.
//

#include "SongNet.h"
#include "RacksNet.h"

namespace PROMISE
{
namespace Core
{

SongNet::SongNet(NodeData &nodeData) noexcept
    : Network(nodeData)
{
    // create all context networks.
    createNode("Racks", "Racks");
}

void SongNet::initDefaultNodeData(NodeData &d) noexcept
{
    Network::initDefaultNodeData(d);
}

juce::Identifier SongNet::getTypeIdentifier()
{
    static auto identifier = juce::Identifier("Song");
    return identifier;
}

NodeData SongNet::nodeDataFactory(const String &nodeType) const noexcept
{
    NodeData newNodeData;
    if (juce::Identifier(nodeType) == RacksNet::getTypeIdentifier()) {
        newNodeData = NodeData(RacksNet::getTypeIdentifier());
        RacksNet::initDefaultNodeData(newNodeData);
    }
    return newNodeData;
}

}
}