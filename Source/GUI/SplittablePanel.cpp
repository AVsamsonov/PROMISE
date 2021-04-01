//
// Created by Alex Samsonov on 25/03/21.
//

#include "SplittablePanel.h"

//==============================================================================
SplittablePanel::SplittablePanel()
{
    panelA.reset(new TabbedPanel());
    addAndMakeVisible(panelA.get());

    auto* hostPanel = dynamic_cast<TabbedPanel*>(panelA.get());
    hostPanel->onSplitMenuItemClicked = [&] (bool splitVertically) {
        onPanelSplitMenuItemClicked(splitVertically);
    };

    layout.setItemLayout(0, -1, -1, -1);
}

SplittablePanel::~SplittablePanel()
{
    panelA = nullptr;
    panelB = nullptr;
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
void SplittablePanel::split(bool vertically) {
    verticalSplit = vertically;

    // set the first panel to be a nested SplittablePanel
    panelA.reset(new SplittablePanel());
    auto* newPanelA = dynamic_cast<SplittablePanel*>(panelA.get());
    dynamic_cast<TabbedPanel*>(newPanelA->getPanelA())->onCloseMenuItemClicked = [&] (const TabbedPanel &panel) {
        onPanelCloseMenuItemClicked(panel);
    };
    addAndMakeVisible(panelA.get());

    // add the resizer bar
    resizer.reset(new juce::StretchableLayoutResizerBar(std::addressof(layout), 1, !verticalSplit));
    addAndMakeVisible(resizer.get());

    // add the second nested SplittablePanel
    panelB.reset(new SplittablePanel());
    auto* newPanelB = dynamic_cast<SplittablePanel*>(panelB.get());
    dynamic_cast<TabbedPanel*>(newPanelB->getPanelA())->onCloseMenuItemClicked = [&] (const TabbedPanel &panel) {
        onPanelCloseMenuItemClicked(panel);
    };
    addAndMakeVisible(panelB.get());

    // set layout
    layout.setItemLayout(0, -0.2, -1, -1);
    layout.setItemLayout(1, 4, 4, 4);
    layout.setItemLayout(2, -0.2, -1, -1);

    resized();
}

//==============================================================================
void SplittablePanel::onPanelSplitMenuItemClicked(bool splitVertically)
{
    split(splitVertically);
}

void SplittablePanel::onPanelCloseMenuItemClicked(const TabbedPanel &panel)
{
    if  (!panelB and !resizer) return;

    if (std::addressof(panel) == dynamic_cast<SplittablePanel*>(panelA.get())->getPanelA()) {
        panelA = std::move(panelB);
    }
    panelB.reset(nullptr);
    resizer.reset(nullptr);
    layout.setItemLayout(0, -1, -1, -1);

    resized();
}

