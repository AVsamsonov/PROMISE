//
// Created by Alex Samsonov on 5/04/21.
//

#pragma once

#include "JuceHeader.h"


class PythonShell : public juce::Component,
                    private juce::KeyListener
{
public:
    //==============================================================================
    PythonShell();
    ~PythonShell() override = default;

private:
    //==============================================================================
    juce::CodeDocument codeDocument;
    juce::CodeEditorComponent editor = {codeDocument, nullptr};
    juce::TextEditor outputDisplay;
    juce::StretchableLayoutManager layout;
    juce::StretchableLayoutResizerBar resizer = {std::addressof(layout), 1, false};

    //==============================================================================
    void resized() override;
    bool keyPressed(const juce::KeyPress &key) override {return juce::Component::keyPressed(key);}

    //==============================================================================
    bool keyPressed(const juce::KeyPress &key, juce::Component *originatingComponent) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PythonShell)

};



