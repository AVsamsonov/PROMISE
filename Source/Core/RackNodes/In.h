//
// Created by Alex Samsonov on 20/06/21.
//

#pragma once

#include "../Node.h"

namespace PROMISE
{
namespace Core
{

class In : public Node
{
public:
    //==============================================================================
    In() noexcept = default;
    explicit In(NodeData &nodeData);
    ~In() override = default;

    //==============================================================================
    static void initDefaultNodeData(NodeData &data) noexcept;
    static juce::Identifier getTypeIdentifier();
};

}
}



