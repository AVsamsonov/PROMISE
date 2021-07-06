//
// Created by Alex Samsonov on 6/06/21.
//

#pragma once

#include "Network.h"

namespace PROMISE
{
namespace Core
{

class RacksNet : public Network
{
public:
    //==============================================================================
    RacksNet() noexcept = default;
    explicit RacksNet(NodeData &nodeData);
    ~RacksNet() override = default;

    //==============================================================================
    static void initDefaultNodeData(NodeData &data) noexcept;
    static juce::Identifier getTypeIdentifier();

protected:
    //==============================================================================
    NodeData nodeDataFactory(const juce::String& nodeType) const noexcept override;
};

}
}



