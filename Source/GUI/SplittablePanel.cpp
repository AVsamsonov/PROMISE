//
// Created by Alex Samsonov on 25/03/21.
//

#include "SplittablePanel.h"

//==============================================================================
SplittablePanel::SplittablePanel()
    : SplittablePanel(new TabbedPanel())
{
}

SplittablePanel::SplittablePanel(TabbedPanel *srcTabbedPanel)
{
    panelA.reset(srcTabbedPanel);
    addAndMakeVisible(panelA.get());

    setupTabbedPanel();

    layout.setItemLayout(0, -1, -1, -1);
}

SplittablePanel::~SplittablePanel()
{
    panelA = nullptr;
    panelB = nullptr;
    resizer = nullptr;
}

//==============================================================================
void SplittablePanel::resized() {
    if (panelB && resizer) {
        Component* comps[] = {panelA.get(), resizer.get(), panelB.get() };
        layout.layOutComponents(comps, 3, 0, 0, getWidth(), getHeight(), verticalSplit, true);
    }
    else {
        Component* comps[] = {panelA.get() };
        layout.layOutComponents(comps, 1, 0, 0, getWidth(), getHeight(), verticalSplit, true);
    }
}

//==============================================================================
std::unique_ptr<juce::XmlElement> SplittablePanel::createXml() const
{
    if (isSplit()) {
        std::unique_ptr<juce::XmlElement> xml{ new juce::XmlElement("SplittablePanel") };
        xml->setAttribute("verticalSplit", verticalSplit);
        xml->setAttribute("resizerPosition", layout.getItemCurrentPosition(1));
        xml->addChildElement(dynamic_cast<SplittablePanel*>(getPanelA())->createXml().release());
        xml->addChildElement(dynamic_cast<SplittablePanel*>(getPanelB())->createXml().release());
        return xml;
    }
    else {
        return dynamic_cast<TabbedPanel*>(getPanelA())->createXml();
    }
}

void SplittablePanel::initializeFromXml(const juce::XmlElement &xml)
{
    if (xml.hasTagName("SplittablePanel")) {
        bool vertically = xml.getStringAttribute("verticalSplit").getIntValue();
        split(vertically);

        auto* panelAXml = xml.getChildElement(0);
        dynamic_cast<SplittablePanel*>(panelA.get())->initializeFromXml(*panelAXml);

        auto* panelBXml = xml.getChildElement(1);
        dynamic_cast<SplittablePanel*>(panelB.get())->initializeFromXml(*panelBXml);

        int resizerPosition = xml.getStringAttribute("resizerPosition").getIntValue();
        layout.setItemPosition(1, resizerPosition);
    }
    else if (xml.hasTagName("TabbedPanel")) {
        auto* tabbedPanelA = dynamic_cast<TabbedPanel*>(panelA.get());
        tabbedPanelA->initializeFromXml(xml);
    }
    resized();
}

void SplittablePanel::setupTabbedPanel() {
    auto* tabbedPanel = dynamic_cast<TabbedPanel*>(panelA.get());
    tabbedPanel->onSplitMenuItemClicked = [&] (bool splitVertically) {
        onTabbedPanelSplitMenuItemClicked(splitVertically);
    };
    tabbedPanel->onMaximizedStateChanged = [&] (bool state) {
        onTabbedPanelMaximizedStateChanged(state);
    };
}

//==============================================================================
void SplittablePanel::split(bool vertically) {
    verticalSplit = vertically;

    // set the first panel to be a nested SplittablePanel
    setupNestedPanel(panelA, true);

    // add the resizer bar
    resizer.reset(new juce::StretchableLayoutResizerBar(std::addressof(layout), 1, !verticalSplit));
    addAndMakeVisible(resizer.get());

    // add the second nested SplittablePanel
    setupNestedPanel(panelB);

    // set layout
    layout.setItemLayout(0, -0.2, -1, -1);
    layout.setItemLayout(1, 4, 4, 4);
    layout.setItemLayout(2, -0.2, -1, -1);

    resized();
}

void SplittablePanel::setupNestedPanel(std::unique_ptr<juce::Component> &nestedPanel, bool movePanelA)
{
    if (movePanelA) {
        auto* srcPanelA = dynamic_cast<TabbedPanel*>(nestedPanel.release());
        nestedPanel.reset(new SplittablePanel(srcPanelA));
    }
    else {
        nestedPanel.reset(new SplittablePanel());
    }

    auto* newPanelA = dynamic_cast<SplittablePanel*>(nestedPanel.get());
    auto* tabbedPanelA = dynamic_cast<TabbedPanel*>(newPanelA->getPanelA());
    tabbedPanelA->onCloseMenuItemClicked = [&] (const TabbedPanel &panel) {
        onTabbedPanelCloseMenuItemClicked(panel);
    };
    newPanelA->onMaximizedStateChanged = [&] (const SplittablePanel &panel, bool state) {
        onSplittablePanelMaximizedStateChanged(panel, state);
    };
    addAndMakeVisible(nestedPanel.get());
}

//==============================================================================
void SplittablePanel::onTabbedPanelSplitMenuItemClicked(bool splitVertically)
{
    split(splitVertically);
}

void SplittablePanel::onTabbedPanelCloseMenuItemClicked(const TabbedPanel &panel) {
    if (isSplit()) {
        if (std::addressof(panel) == dynamic_cast<SplittablePanel *>(panelA.get())->getPanelA()) {
            panelA = std::move(panelB);
        }
        panelB.reset(nullptr);
        resizer.reset(nullptr);
        layout.setItemLayout(0, -1, -1, -1);

        resized();
    }
    else {
        std::cout << dynamic_cast<SplittablePanel*>(panelA.get()) << std::endl;
    }
}

void  SplittablePanel::onTabbedPanelMaximizedStateChanged(bool maximizedState) const
{
    if (onMaximizedStateChanged) onMaximizedStateChanged(*this, maximizedState);
}

void SplittablePanel::onSplittablePanelMaximizedStateChanged(const SplittablePanel &panel, bool maximizedState)
{
    if (isSplit()) {
        if (std::addressof(panel) == panelA.get()) {
            if (maximizedState) {
                panelB->setVisible(false);
                resizer->setVisible(false);
                layout.setItemLayout(0, -1, -1, -1);
                layout.setItemLayout(1, 0, 0, 0);
                layout.setItemLayout(2, 0, 0, 0);
            }
            else {
                panelB->setVisible(true);
                resizer->setVisible(true);
                layout.setItemLayout(0, -0.2, -1, -1);
                layout.setItemLayout(1, 4, 4, 4);
                layout.setItemLayout(2, -0.2, -1, -1);
            }
        } else {
            if (maximizedState) {
                panelA->setVisible(false);
                resizer->setVisible(false);
                layout.setItemLayout(0, 0, 0, 0);
                layout.setItemLayout(1, 0, 0, 0);
                layout.setItemLayout(2, -1, -1, -1);
            }
            else {
                panelA->setVisible(true);
                resizer->setVisible(true);
                layout.setItemLayout(0, -0.2, -1, -1);
                layout.setItemLayout(1, 4, 4, 4);
                layout.setItemLayout(2, -0.2, -1, -1);
            }
        }
    }
    resized();
    if (onMaximizedStateChanged) onMaximizedStateChanged(*this, maximizedState);
}
