//
// Created by Alex Samsonov on 20/06/21.
//

#include "In.h"
#include "../Rack.h"

namespace PROMISE
{
namespace Core
{

//==============================================================================
In::In(NodeData &nodeData)
    : Node(nodeData)
{

}

//==============================================================================
void In::initDefaultNodeData(NodeData &d) noexcept
{
    Node::initDefaultNodeData(d);

    // Output plugs
    Node::initOutputPlug(d, "Midi", PlugDataType::Midi);
    Node::initOutputPlug(d, "Audio", PlugDataType::Audio);
}

juce::Identifier In::getTypeIdentifier()
{
    static auto identifier = juce::Identifier("In");
    return identifier;
}

}
}