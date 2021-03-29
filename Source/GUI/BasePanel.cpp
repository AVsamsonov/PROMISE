//
// Created by Alex Samsonov on 27/03/21.
//

#include "BasePanel.h"

//==============================================================================
BasePanel::BasePanel() {
    topRightMenuButton.reset(new juce::ArrowButton("PanelMenu", 0.25, juce::Colours::white));
    addAndMakeVisible(topRightMenuButton.get());

    topRightMenuButton->onClick = [&] {
        juce::PopupMenu menu;
        menu.addItem("Split Panel Horizontally", [&] () {
            if (onSplitMenuItemClicked) onSplitMenuItemClicked(*this, false);
        });
        menu.addItem("Split Panel Vertically", [&] () {
            if (onSplitMenuItemClicked) onSplitMenuItemClicked(*this, true);
        });
        menu.addItem("Close Panel", [&] () {
            if (onCloseMenuItemClicked) onCloseMenuItemClicked(*this);
        });
        menu.showMenuAsync(juce::PopupMenu::Options{});
    };
}

//==============================================================================
void BasePanel::resized() {
    auto b = getLocalBounds();
    topRightMenuButton->setBounds(b.removeFromTop(20)
                                             .removeFromRight(20)
                                             .reduced(4));
}

void BasePanel::paint(juce::Graphics &g) {
    g.setColour(juce::Colours::black);
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 4, 2);
}
