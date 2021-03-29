#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    centralPanel.reset(new SplittablePanel);
    addAndMakeVisible(centralPanel.get());

    setSize (800, 600);
}

//==============================================================================
void MainComponent::resized()
{
    centralPanel->setBounds(getLocalBounds().reduced(2));
}
