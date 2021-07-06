//
// Created by Alex Samsonov on 31/05/21.
//

#include "Node.h"

#include <utility>
#include "Network.h"


namespace PROMISE
{
namespace Core
{

constexpr auto InputPlugPropertyPrefix = "InputPlug::";
constexpr auto OutputPlugPropertyPrefix = "OutputPlug::";

//==============================================================================
/** Provides an STL compatible iterator for the properties of ValueTree. */
class ValueTreePropertyConnector
{
public:
    explicit ValueTreePropertyConnector(ValueTree t) : tree(std::move(t))
    {}

    class Iterator
    {
    public:
        Iterator(const ValueTree &t, int position) : tree(t), pos(position)
        {}

        Iterator &operator++()
        {
            ++pos;
            return *this;
        }

        bool operator!=(const Iterator &other) const
        { return other.pos != pos || other.tree != tree; }

        std::pair<Identifier, var> operator*() const
        {
            auto propertyName = tree.getPropertyName(pos);
            return {propertyName, tree.getProperty(propertyName)};
        }

        Iterator &operator=(const Iterator &) = delete;

    private:
        const ValueTree &tree;
        int pos;
    };

    Iterator begin()
    { return Iterator(tree, 0); }

    Iterator end()
    { return Iterator(tree, tree.getNumProperties()); }

private:
    JUCE_DECLARE_NON_COPYABLE(ValueTreePropertyConnector)
    ValueTree tree;
};


//==============================================================================
Node::Node() noexcept
    : data(NodeData())
{
}

Node::Node(NodeData &nodeData)
        : data(nodeData)
{
}

//==============================================================================
void Node::initDefaultNodeData(NodeData &data) noexcept
{
    data.setProperty(getNamePropertyIdentifier(), var(), nullptr);
}

juce::Identifier Node::getTypeIdentifier()
{
    return juce::Identifier("Node");
}

//==============================================================================
NodeData Node::getData() const noexcept
{
    return data;
}

bool Node::isValid() const noexcept
{
    return data.isValid();
}

Network Node::getParent() const noexcept
{
    auto parentValueTree = data.getParent();
    return Network(parentValueTree);
}

juce::Identifier Node::getType() const noexcept
{
    return data.getType();
}

bool Node::isNetwork() const noexcept
{
    return false;
}

juce::String Node::getName() const noexcept
{
    auto name = data.getProperty(getNamePropertyIdentifier()).toString();
    return name;
}

juce::Identifier Node::getNamePropertyIdentifier()
{
    return juce::Identifier("Generic::Name");
}

void Node::initInputPlug(NodeData& nodeData, const juce::String& inputPlugName, PlugDataType plugDataType)
{
    nodeData.setProperty( juce::String(InputPlugPropertyPrefix) + inputPlugName, int(plugDataType), nullptr);
}

void Node::initOutputPlug(NodeData& nodeData, const juce::String& outputPlugName, PlugDataType plugDataType)
{
    nodeData.setProperty( juce::String(OutputPlugPropertyPrefix) + outputPlugName, int(plugDataType), nullptr);
}

void Node::setName(const String &name) noexcept
{
    data.setProperty(getNamePropertyIdentifier(), name, nullptr);
}

int Node::getNumInputPlugs() const noexcept
{
    int result = 0;
    for (const auto& x : ValueTreePropertyConnector(data)) {
        auto& propertyName = x.first;
        if (propertyName.toString().startsWith(InputPlugPropertyPrefix)) ++result;
    }
    return result;
}

int Node::getNumOutputPlugs() const noexcept
{
    int result = 0;
    for (const auto& x : ValueTreePropertyConnector(data)) {
        auto& propertyName = x.first;
        if (propertyName.toString().startsWith(OutputPlugPropertyPrefix)) ++result;
    }
    return result;
}

juce::String Node::getInputPlugName(int inputPlugIndex) const
{
    int idx = -1;
    for (const auto& x : ValueTreePropertyConnector(data)) {
        auto& propertyName = x.first;
        if (propertyName.toString().startsWith(InputPlugPropertyPrefix)) {
            if (inputPlugIndex == ++idx) {
                auto inputPlugName = propertyName.toString().replaceFirstOccurrenceOf(InputPlugPropertyPrefix, "");
                return inputPlugName;
            }
        }
    }
    throw std::runtime_error("Unable to find an input plug with index: " + std::to_string(inputPlugIndex));
}

juce::String Node::getOutputPlugName(int outputPlugIndex) const
{
    int idx = -1;
    for (const auto& x : ValueTreePropertyConnector(data)) {
        auto& propertyName = x.first;
        if (propertyName.toString().startsWith(OutputPlugPropertyPrefix)) {
            if (outputPlugIndex == ++idx) {
                auto outputPlugName = propertyName.toString().replaceFirstOccurrenceOf(OutputPlugPropertyPrefix, "");
                return outputPlugName;
            }
        }
    }
    throw std::runtime_error("Unable to find an output plug with index: " + std::to_string(outputPlugIndex));
}

Node::PlugDataType Node::getInputPlugDataType(int inputPlugIndex) const
{
    int idx = -1;
    for (const auto& x : ValueTreePropertyConnector(data)) {
        auto& propertyName = x.first;
        auto& propertyValue = x.second;
        if (propertyName.toString().startsWith(InputPlugPropertyPrefix)) {
            if (inputPlugIndex == ++idx) {
                return static_cast<PlugDataType>(int(propertyValue));
            }
        }
    }
    throw std::runtime_error("Unable to find an input plug with index: " + std::to_string(inputPlugIndex));
}

Node::PlugDataType Node::getOutputPlugDataType(int outputPlugIndex) const
{
    int idx = -1;
    for (const auto& x : ValueTreePropertyConnector(data)) {
        auto& propertyName = x.first;
        auto& propertyValue = x.second;
        if (propertyName.toString().startsWith(OutputPlugPropertyPrefix)) {
            if (outputPlugIndex == ++idx) {
                return static_cast<PlugDataType>(int(propertyValue));
            }
        }
    }
    throw std::runtime_error("Unable to find an output plug with index: " + std::to_string(outputPlugIndex));
}

bool Node::hasInputPlugWithName(const juce::String& inputPlugName) const noexcept
{
    return data.hasProperty(juce::String(InputPlugPropertyPrefix) + inputPlugName);
}

bool Node::hasOutputPlugWithName(const juce::String& outputPlugName) const noexcept
{
    return data.hasProperty(juce::String(OutputPlugPropertyPrefix) + outputPlugName);
}

void Node::setInput(const juce::String& inputPlugName, const Node& inputNode, const juce::String& outputPlugName) const
{
    auto net = getParent();
    net.connectNodes(inputNode, outputPlugName, *this, inputPlugName);
}

//==============================================================================
bool Node::operator==(const Node &other) const noexcept
{
    return (data == other.data);
}

bool Node::operator!=(const Node &other) const noexcept
{
    return (data != other.data);
}


}
}