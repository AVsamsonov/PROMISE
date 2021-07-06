//
// Created by Alex Samsonov on 31/05/21.
//

#pragma once

#include <JuceHeader.h>

#include "Network.h"

namespace PROMISE
{
namespace Core
{

class SongNet : public Network
{
public:
    //==============================================================================
    explicit SongNet(NodeData &nodeData) noexcept;
    ~SongNet() override = default;

    //==============================================================================
    static void initDefaultNodeData(NodeData &data) noexcept;
    static juce::Identifier getTypeIdentifier();

protected:
    //==============================================================================
    NodeData nodeDataFactory(const juce::String& nodeType) const noexcept override;

};

}
}