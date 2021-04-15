#include "MainComponent.h"
#include "PreferencesDialog.h"

//==============================================================================
MainComponent::MainComponent()
{
    std::unique_ptr<juce::XmlElement> savedAudioState (getAppProperties().getUserSettings()->getXmlValue ("audioDeviceState"));
    audioDeviceManager.initialise(256, 256, savedAudioState.get(), true);

    centralPanel.reset(new SplittablePanel);
    addAndMakeVisible(centralPanel.get());

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
    centralPanel->setBounds(getLocalBounds().reduced(2));
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
