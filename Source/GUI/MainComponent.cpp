#include "MainComponent.h"
#include "PreferencesDialog.h"

//==============================================================================
MainComponent::MainComponent()
{
    std::unique_ptr<juce::XmlElement> savedAudioState (getAppProperties().getUserSettings()->getXmlValue ("audioDeviceState"));
    audioDeviceManager.initialise(256, 256, savedAudioState.get(), true);

    desktopSelector.onClick = [&] {
        onDesktopSelectorClicked();
    };
    addAndMakeVisible(desktopSelector);

    addAndMakeVisible(centralPanel);

    #if JUCE_MAC
    juce::PopupMenu extraMenu;
    extraMenu.addItem("Preferences...", [&] {
        showPreferencesDialog();
    });
    juce::MenuBarModel::setMacMainMenu(this, std::addressof(extraMenu));
    #endif

    setSize (800, 600);
}

MainComponent::~MainComponent()
{
    #if JUCE_MAC
    juce::MenuBarModel::setMacMainMenu(nullptr);
    #endif
}

//==============================================================================
void MainComponent::resized()
{
    auto b = getLocalBounds();
    desktopSelector.setBounds(b.getRight()-102, 2, 100, 24);
    centralPanel.setBounds(b.withTrimmedTop(25).reduced(2));
}

//==============================================================================
juce::StringArray MainComponent::getMenuBarNames()
{
    return juce::StringArray();
}

juce::PopupMenu MainComponent::getMenuForIndex(int /*topLevelMenuIndex*/, const juce::String &/*menuName*/)
{
    return juce::PopupMenu();
}

void MainComponent::menuItemSelected(int /*menuItemID*/, int /*topLevelMenuIndex*/)
{

}

//==============================================================================
void MainComponent::showPreferencesDialog()
{
    auto* prefDialog = new PreferencesDialog (audioDeviceManager);

    juce::DialogWindow::LaunchOptions o;
    o.content.setOwned(prefDialog);
    o.dialogTitle                   = "Preferences";
    o.componentToCentreAround       = this;
    o.dialogBackgroundColour        = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
    o.escapeKeyTriggersCloseButton  = true;
    o.useNativeTitleBar             = true;
    o.resizable                     = false;

    auto* w = o.create();
    w->enterModalState(true, juce::ModalCallbackFunction::create([&] (int) {
        std::unique_ptr<XmlElement> audioState (audioDeviceManager.createStateXml());
        getAppProperties().getUserSettings()->setValue ("audioDeviceState", audioState.get());
        getAppProperties().getUserSettings()->saveIfNeeded();
    }), true);
}

void MainComponent::onDesktopSelectorClicked()
{
    juce::PopupMenu menu;

    // Add desktop items
    for (const juce::String& desktopName : getDesktopNamesList())
    {
        menu.addItem(desktopName, true, desktopName == currentDesktopName, [&] {
            setCurrentDesktop(currentDesktopName);
        });
    }

    menu.addSeparator();

    // Add desktop commands
    menu.addItem("New Desktop", nullptr);
    menu.addItem("Save Current Desktop", [&] {
        saveCurrentDesktop(currentDesktopName);
    });
    menu.addItem("Save Current Desktop As...", [&] {
        juce::AlertWindow w ("Save Current Desktop As...",
                       "",
                       AlertWindow::QuestionIcon);
        w.addTextEditor ("desktopName", "Desktop", "Desktop Name:");
        w.addButton ("OK",     1, juce::KeyPress (juce::KeyPress::returnKey, 0, 0));
        w.addButton ("Cancel", 0, juce::KeyPress (juce::KeyPress::escapeKey, 0, 0));
        if (w.runModalLoop() != 0)
        {
            auto desktopName = w.getTextEditorContents("desktopName");
            saveCurrentDesktop(desktopName);
        }
    });

    // Show menu
    menu.showMenuAsync(juce::PopupMenu::Options{});
}

juce::StringArray MainComponent::getDesktopNamesList()
{
    return juce::StringArray({ "Default" });
}

juce::String MainComponent::getDesktopsDir()
{
    return juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getFullPathName() +
           juce::File::getSeparatorChar() + "Application Support" +
           juce::File::getSeparatorChar() + getAppName() +
           juce::File::getSeparatorChar() + "Desktops";
}

void MainComponent::setCurrentDesktop(const juce::String& desktopName)
{
    if (desktopName == currentDesktopName) return;

    // Read the desktop file
    auto desktopsDirPath = getDesktopsDir();
    auto desktopFileName = desktopsDirPath + juce::File::getSeparatorChar() + desktopName + ".desktop";
    auto desktopFile = juce::File(desktopFileName);
    if (desktopFile.existsAsFile()) {
        auto xml = juce::parseXML(desktopFile);
        centralPanel.initializeFromXml(*xml);
    }
    else {
        centralPanel.initialize();
    }
    currentDesktopName = desktopName;
    desktopSelector.setButtonText(desktopName);
}

void MainComponent::saveCurrentDesktop(const juce::String& desktopName)
{
    auto desktopsDirPath = getDesktopsDir();
    juce::File(desktopsDirPath).createDirectory();

    auto desktopFileName = desktopsDirPath + juce::File::getSeparatorChar() + desktopName + ".desktop";
    auto xml = centralPanel.createXml();
    juce::File(desktopFileName).replaceWithText(xml->toString());
}
