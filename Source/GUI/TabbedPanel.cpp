//
// Created by Alex Samsonov on 31/03/21.
//

#include "TabbedPanel.h"

//==============================================================================
TabbedPanel::TabbedPanel()
    : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop)
{
    auto colour = findColour (juce::ResizableWindow::backgroundColourId);
    addTab("tab", colour, nullptr, true);

    // Add top right menu button
    topRightMenuButton.reset(new juce::ArrowButton("PanelMenu", 0.25, juce::Colours::white));
    addAndMakeVisible(topRightMenuButton.get());

    topRightMenuButton->onClick = [&] {
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
    maximizeButton.reset(new juce::DrawableButton("PanelMaximize", juce::DrawableButton::ButtonStyle::ImageStretched));
    maximizeButtonIcon = juce::Drawable::createFromImageData(BinaryData::maximize_svg, BinaryData::maximize_svgSize);
    maximizeButtonIcon->replaceColour(juce::Colours::black, juce::Colours::white);
    maximizeButton->setImages(maximizeButtonIcon.get(), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    maximizeButton->setClickingTogglesState(true);
    addAndMakeVisible(maximizeButton.get());

    maximizeButton->onStateChange = [&] {
        onMaximizeButtonStageChanged();
    };
}

//==============================================================================
void TabbedPanel::resized()
{
    juce::TabbedComponent::resized();

    auto b = getLocalBounds();
    if (!b.isEmpty()) {
        topRightMenuButton->setBounds(b.getRight()-14, 4, 12, 12);
        maximizeButton->setBounds(b.getRight()-34, 4, 12, 12);
    }
}

//==============================================================================
void TabbedPanel::onMaximizeButtonStageChanged()
{
    if (maximizedState != maximizeButton->getToggleState()) {
        maximizedState = maximizeButton->getToggleState();
        if (onMaximizedStateChanged) onMaximizedStateChanged(*this, maximizeButton->getToggleState());
    }
}
