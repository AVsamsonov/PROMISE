//
// Created by Alex Samsonov on 16/05/21.
//

#pragma once

#include <JuceHeader.h>


class BasePanel {
public:
    BasePanel() = default;
    virtual ~BasePanel() = default;

    void setParent(BasePanel* panel);
    BasePanel* getParent() const;

    virtual std::unique_ptr<juce::XmlElement> createXml() const = 0;
    virtual void initializeFromXml(const juce::XmlElement &xml) = 0;
private:
    BasePanel* parent = nullptr;
};



