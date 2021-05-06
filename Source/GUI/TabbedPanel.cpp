//
// Created by Alex Samsonov on 31/03/21.
//

#include "TabbedPanel.h"
#include "PythonShell.h"

struct ComponentFactory
{
    static juce::Component* makeComponent(const juce::String& name)
    {
        if (name == "Python Shell")
            return new PythonShell();

        return nullptr;
    }
};

static juce::StringArray getTabPanelNames()
{
    static juce::StringArray tabNames = { "Python Shell" };
    return tabNames;
}

//==============================================================================
TabbedPanel::TabbedPanel()
    : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop)
{
    // Add top right menu button
    addAndMakeVisible(topRightMenuButton);

    topRightMenuButton.onClick = [&] {
        onTopRightMenuButtonClicked();
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

    // Add addTab button
    addTabButtonIcon = juce::Drawable::createFromImageData(BinaryData::add_svg, BinaryData::add_svgSize);
    addTabButtonIcon->replaceColour(juce::Colours::black, juce::Colours::white);
    addTabButton.setImages(addTabButtonIcon.get(), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    addAndMakeVisible(addTabButton);

    addTabButton.onClick = [&] {
        onAddTabButtonClicked();
    };

    // Load close tab button icon
    closeTabButtonIcon = juce::Drawable::createFromImageData(BinaryData::close_svg, BinaryData::close_svgSize);
    closeTabButtonIcon->replaceColour(juce::Colours::black, juce::Colours::white);
}

//==============================================================================
void TabbedPanel::resized()
{
    juce::TabbedComponent::resized();

    auto b = getLocalBounds();
    if (!b.isEmpty()) {
        topRightMenuButton.setBounds(b.getRight()-14, 8, 12, 12);
        maximizeButton.setBounds(b.getRight()-34, 8, 12, 12);

        // calculate the addTab button position
        const auto &tabbedButtonBar = getTabbedButtonBar();
        int tabbedButtonBarWidth = 0;
        for (int tabIndex = 0; tabIndex < tabbedButtonBar.getNumTabs(); tabIndex++) {
            auto* tabButton = tabbedButtonBar.getTabButton(tabIndex);
            tabbedButtonBarWidth += tabbedButtonBar.getTargetBounds(tabButton).getWidth() + 1;
        }
        addTabButton.setBounds(tabbedButtonBarWidth + 6, 8, 12, 12);
    }
}

//==============================================================================
std::unique_ptr<juce::XmlElement> TabbedPanel::createXml() const
{
    std::unique_ptr<juce::XmlElement> xml{ new juce::XmlElement("TabbedPanel") };
    const auto &tabbedButtonBar = getTabbedButtonBar();
    for (int tabIndex = 0; tabIndex < tabbedButtonBar.getNumTabs(); tabIndex++) {
        auto* tabButton = tabbedButtonBar.getTabButton(tabIndex);
        tabButton->getButtonText();
        auto* tabElement = new juce::XmlElement("Tab");
        tabElement->setAttribute("name", tabButton->getButtonText());
        xml->addChildElement(tabElement);
    };
    return xml;
}

void TabbedPanel::initializeFromXml(const juce::XmlElement &xml)
{
    clearTabs();

    if (!xml.hasTagName("TabbedPanel")) return;
    for (auto* tabElement : xml.getChildIterator()) {
        auto tabName = tabElement->getStringAttribute("name");
        addPanelTab(tabName);
    }
}

//==============================================================================
void TabbedPanel::onTopRightMenuButtonClicked()
{
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
}

void TabbedPanel::onMaximizeButtonStageChanged()
{
    if (maximizedState != maximizeButton.getToggleState()) {
        maximizedState = maximizeButton.getToggleState();
        if (onMaximizedStateChanged) onMaximizedStateChanged(maximizeButton.getToggleState());
    }
}

void TabbedPanel::onAddTabButtonClicked()
{
    juce::PopupMenu menu;
    for (juce::String tabPanelName : getTabPanelNames())
    {
        menu.addItem(tabPanelName, true, false, [&, tabPanelName] () {
            addPanelTab(tabPanelName);
        });
    }
    menu.showMenuAsync(juce::PopupMenu::Options{});
}

void TabbedPanel::addPanelTab(const juce::String& tabName)
{
    auto colour = findColour (juce::ResizableWindow::backgroundColourId);
    addTab(tabName, colour, ComponentFactory::makeComponent(tabName), true);
    // add close tab button to the just created tab
    auto& tabbedButtonBar = getTabbedButtonBar();
    int newTabButtonIndex = tabbedButtonBar.getNumTabs()-1;
    tabbedButtonBar.setCurrentTabIndex(newTabButtonIndex);
    auto* tabButton = tabbedButtonBar.getTabButton(newTabButtonIndex);
    auto* closeTabButton = new juce::DrawableButton("PanelMaximize",
                                                                    juce::DrawableButton::ButtonStyle::ImageFitted);
    closeTabButton->setImages(closeTabButtonIcon.get(), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    closeTabButton->setSize(14, 14);
    closeTabButton->onClick = [&, tabButton] {
        onCloseTabButtonClicked(tabButton);
    };
    tabButton->setExtraComponent(closeTabButton,TabBarButton::ExtraComponentPlacement::afterText);
    resized();
}

void TabbedPanel::onCloseTabButtonClicked(juce::TabBarButton* tabBarButton)
{
    int tabBarButtonIndex = tabBarButton->getIndex();
    getTabbedButtonBar().removeTab(tabBarButtonIndex);
    if (tabBarButtonIndex == getNumTabs()) {
        setCurrentTabIndex(getNumTabs() - 1);
    }
    resized();
}
