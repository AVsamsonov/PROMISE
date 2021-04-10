//
// Created by Alex Samsonov on 31/03/21.
//

#include "TabbedPanel.h"
#include "PythonShell.h"

//==============================================================================
TabbedPanel::TabbedPanel()
    : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop)
{
    auto colour = findColour (juce::ResizableWindow::backgroundColourId);
    addTab("Python Shell", colour, new PythonShell(), true);

    // Add top right menu button
    addAndMakeVisible(topRightMenuButton);

    topRightMenuButton.onClick = [&] {
        juce::PopupMenu menu;
        menu.addItem("Split Panel Horizontally", !maximizedState, false, [&] () {
            if (onSplitMenuItemClicked) onSplitMenuItemClicked(false);
        });
        menu.addItem("Split Panel Vertically", !maximizedState, false, [&] () {
            if (onSplitMenuItemClicked) onSplitMenuItemClicked(true);
        });
        menu.addItem("Close Panel", !maximizedState, false, [&] () {
            if (onCloseMenuItemClicked) onCloseMenuItemClicked(*this);
        });
        menu.showMenuAsync(juce::PopupMenu::Options{});
    };

    // Add maximize button
    maximizeButtonIcon = juce::Drawable::createFromImageData(BinaryData::maximize_svg, BinaryData::maximize_svgSize);
    maximizeButtonIcon->replaceColour(juce::Colours::black, juce::Colours::white);
    maximizeButton.setImages(maximizeButtonIcon.get(), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    maximizeButton.setClickingTogglesState(true);
    addAndMakeVisible(maximizeButton);

    maximizeButton.onStateChange = [&] {
        onMaximizeButtonStageChanged();
    };
}

//==============================================================================
void TabbedPanel::resized()
{
    juce::TabbedComponent::resized();

    auto b = getLocalBounds();
    if (!b.isEmpty()) {
        topRightMenuButton.setBounds(b.getRight()-14, 4, 12, 12);
        maximizeButton.setBounds(b.getRight()-34, 4, 12, 12);
    }
}

//==============================================================================
void TabbedPanel::onMaximizeButtonStageChanged()
{
    if (maximizedState != maximizeButton.getToggleState()) {
        maximizedState = maximizeButton.getToggleState();
        if (onMaximizedStateChanged) onMaximizedStateChanged(maximizeButton.getToggleState());
    }
}
