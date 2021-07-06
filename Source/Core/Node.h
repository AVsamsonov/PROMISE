//
// Created by Alex Samsonov on 31/05/21.
//

#pragma once

#include <JuceHeader.h>

namespace PROMISE
{
namespace Core
{

using NodeData = juce::ValueTree;

class Network;

class Node
{
public:
    //==============================================================================
    enum class PlugDataType { Midi, Audio };

    //==============================================================================
    Node() noexcept;
    explicit Node(NodeData &nodeData);
    virtual ~Node() = default;

    //==============================================================================
    static void initDefaultNodeData(NodeData &data) noexcept;
    static juce::Identifier getTypeIdentifier();

    //==============================================================================
    NodeData getData() const noexcept;
    bool isValid() const noexcept;
    Network getParent() const noexcept;
    juce::Identifier getType() const noexcept;
    virtual bool isNetwork() const noexcept;
    juce::String getName() const noexcept;
    void setName(const juce::String& name) noexcept;
    int getNumInputPlugs() const noexcept;
    int getNumOutputPlugs() const noexcept;
    juce::String getInputPlugName(int inputPlugIndex) const;
    juce::String getOutputPlugName(int outputPlugIndex) const;
    PlugDataType getInputPlugDataType(int inputPlugIndex) const;
    PlugDataType getOutputPlugDataType(int outputPlugIndex) const;
    bool hasInputPlugWithName(const juce::String& inputPlugName) const noexcept;
    bool hasOutputPlugWithName(const juce::String& outputPlugName) const noexcept;
    void setInput(const juce::String& inputPlugName, const Node& inputNode, const juce::String& outputPlugName) const;

    //==============================================================================
    bool operator==(const Node& other) const noexcept;
    bool operator!=(const Node& other) const noexcept;

protected:
    //==============================================================================
    NodeData data;
    static void initInputPlug(NodeData& nodeData, const juce::String& inputPlugName, PlugDataType plugDataType);
    static void initOutputPlug(NodeData& nodeData, const juce::String& outputPlugName, PlugDataType plugDataType);

private:
    //==============================================================================
    static juce::Identifier getNamePropertyIdentifier();
};

}
}