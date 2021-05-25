//
// Created by Alex Samsonov on 25/03/21.
//

#include "SplittablePanel.h"
#include "TabbedPanel.h"

//==============================================================================
SplittablePanel::SplittablePanel()
    : BasePanel()
{
}

//==============================================================================
void SplittablePanel::resized()
{
    if (children.size() > 1) {
        Component* comps[] = { dynamic_cast<Component*>(children[0]), resizer.get(), dynamic_cast<Component*>(children[1]) };
        layout.layOutComponents(comps, 3, 0, 0, getWidth(), getHeight(), verticalSplit, true);
    }
    else {
        Component* comps[] = { dynamic_cast<Component*>(children[0]) };
        layout.layOutComponents(comps, 1, 0, 0, getWidth(), getHeight(), verticalSplit, true);
    }
}

//==============================================================================
std::unique_ptr<juce::XmlElement> SplittablePanel::createXml() const
{
    std::unique_ptr<juce::XmlElement> xml{ new juce::XmlElement("SplittablePanel") };
    for (auto* child : children) {
        xml->addChildElement(child->createXml().release());
    }
    if (children.size() > 1) {
        xml->setAttribute("verticalSplit", verticalSplit);
        xml->setAttribute("resizerPosition", layout.getItemCurrentPosition(1));
    }
    return xml;
}

void SplittablePanel::initialize()
{
    auto* newTabbedPanel = new TabbedPanel();
    addChild(newTabbedPanel);
}

void SplittablePanel::initializeFromXml(const juce::XmlElement &xml)
{
    if (xml.hasTagName("SplittablePanel")) {
        if (xml.hasAttribute("verticalSplit")) {
            verticalSplit = xml.getBoolAttribute("verticalSplit");
        }

        for (auto* child : xml.getChildIterator()) {
            if (child->hasTagName("SplittablePanel")) {
                auto* childSplittablePanel = new SplittablePanel();
                addChild(childSplittablePanel);
                resized();
                childSplittablePanel->initializeFromXml(*child);
            }
            else if (child->hasTagName("TabbedPanel")) {
                auto* childTabbedPanel = new TabbedPanel();
                addChild(childTabbedPanel);
                resized();
                childTabbedPanel->initializeFromXml(*child);
            }
        }

        if (xml.hasAttribute("resizerPosition")) {
            int resizerPosition = xml.getIntAttribute("resizerPosition");
            layout.setItemPosition(1, resizerPosition);
        }
    }
    resized();
}

//==============================================================================
void SplittablePanel::addChild(BasePanel *panel, int index)
{
    if (index == -1) {
        children.add(panel);
    }
    else {
        children.insert(index, panel);
    }
    panel->setParent(this);
    auto* component = dynamic_cast<juce::Component*>(panel);
    addAndMakeVisible(component);

    if (children.size() > 1) {
        auto* newResizer = new juce::StretchableLayoutResizerBar(std::addressof(layout), 1, !verticalSplit);
        resizer.reset(newResizer);
        addAndMakeVisible(newResizer);
        layout.setItemLayout(0, -0.2, -1, -1);
        layout.setItemLayout(1, 4, 4, 4);
        layout.setItemLayout(2, -0.2, -1, -1);
    }
    else {
        layout.setItemLayout(0, -1, -1, -1);
    }
}

int SplittablePanel::removeChild(BasePanel *panel, bool deletePanel)
{
    auto* component = dynamic_cast<juce::Component*>(panel);
    removeChildComponent(component);
    panel->setParent(nullptr);
    int index = children.indexOf(panel);
    children.removeObject(panel, deletePanel);
    if (children.size() < 2) {
        removeChildComponent(resizer.get());
        resizer.reset(nullptr);
        layout.setItemLayout(0, -1, -1, -1);
    }
    return index;
}

void SplittablePanel::splitChild(BasePanel *panel, bool vertically)
{
    int resizerPos = resizer ? layout.getItemCurrentPosition(1) : -1;
    int index = removeChild(panel, false);
    auto* newSplittablePanel = new SplittablePanel();
    newSplittablePanel->verticalSplit = vertically;
    addChild(newSplittablePanel, index);
    newSplittablePanel->addChild(panel);
    newSplittablePanel->addChild(new TabbedPanel());
    if (resizerPos != -1)
        layout.setItemPosition(1, resizerPos);
    resized();
}

void SplittablePanel::closeChild(BasePanel* panel)
{
    auto* parent = getParent();
    if (parent) {
        removeChild(panel);
        auto* parentSplittablePanel = dynamic_cast<SplittablePanel*>(parent);
        parentSplittablePanel->moveChildFrom(this);
    }
}

void SplittablePanel::moveChildFrom(SplittablePanel *other)
{
    auto* otherFirstChild = other->children[0];
    other->children.removeObject(otherFirstChild, false);
    int index = removeChild(other);
    addChild(otherFirstChild, index);
    resized();
}

void SplittablePanel::setChildMaximized(BasePanel *panel, bool m)
{
    maximized = m;
    if (children.size() > 1) {
        int panelIndex = children.indexOf(panel);
        int otherPanelIndex = 1 - panelIndex;
        auto* otherPanel = children[otherPanelIndex];
        auto* otherPanelComponent = dynamic_cast<Component*>(otherPanel);
        otherPanelComponent->setVisible(!maximized);
        resizer->setVisible(!maximized);

        if (maximized) {
            layout.setItemLayout(panelIndex, -1, -1, -1);
            layout.setItemLayout(1, 0, 0, 0);
            layout.setItemLayout(otherPanelIndex, 0, 0, 0);
        }
        else {
            layout.setItemLayout(0, -0.2, -1, -1);
            layout.setItemLayout(1, 4, 4, 4);
            layout.setItemLayout(2, -0.2, -1, -1);
        }
        resized();
    }
    auto* parent = getParent();
    if (parent) {
        auto* parentSplittablePanel = dynamic_cast<SplittablePanel*>(parent);
        parentSplittablePanel->setChildMaximized(this, maximized);
    }
}
