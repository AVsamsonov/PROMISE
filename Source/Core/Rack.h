//
// Created by Alex Samsonov on 17/06/21.
//

#pragma once

#include "Network.h"

namespace PROMISE
{
namespace Core
{

class Rack : public Network
{
public:
    //==============================================================================
    Rack() noexcept;
    Rack(const Node& node);
    explicit Rack(NodeData &nodeData) noexcept;
    ~Rack() override = default;

    //==============================================================================
    static void initDefaultNodeData(NodeData &data) noexcept;
    static juce::Identifier getTypeIdentifier();

protected:
    //==============================================================================
    NodeData nodeDataFactory(const juce::String& nodeType) const noexcept override;
};


}
}


