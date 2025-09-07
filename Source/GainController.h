#pragma once

#include <JuceHeader.h>

// Controller from JUCE Decibel Slider Tutorial

// Gain controller inherits from juce's Slider class
class GainController : public juce::Slider {

public:

    GainController() {

    }

    double getValueFromText (const juce::String& text) override {

        auto minusInfinitydB = -100.0;

        // clean the decibel text
        auto decibelText = text.upToFirstOccurrenceOf ("dB", false, false).trim();

        // check if text is -inf
        return decibelText.equalsIgnoreCase ("-INF") ? minusInfinitydB : decibelText.getDoubleValue();

    }

    juce::String getTextFromValue (double value) override {

        return juce::Decibels::toString (value);

    }


private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainController)
    
};


class MainContentComponent : public juce::AudioAppComponent {

public:

    MainContentComponent() {

        // setup the gain controller
        gainController.setRange (-100, -12);
        gainController.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
        gainController.onValueChange = [this] { level = juce::Decibels::decibelsToGain ((float) gainController.getValue()); }; // convert to linear gain
        gainController.setValue (juce::Decibels::gainToDecibels (level));
        decibelLabel.setText ("Noise Level in dB", juce::dontSendNotification);

        addAndMakeVisible (gainController);
        addAndMakeVisible (decibelLabel);

        setSize (600, 100);
        setAudioChannels (0, 2);

    }

    ~MainContentComponent() override {

        shutdownAudio();

    }

    void prepareToPlay (int, double) override {

    }

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override {

        auto currentLevel = level;
        auto levelScale = currentLevel * 2.0f;

        for (auto channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel) {

            auto* buffer = bufferToFill.buffer->getWritePointer (channel, bufferToFill.startSample);

            for (auto sample = 0; sample < bufferToFill.numSamples; ++sample) {

                buffer[sample] = random.nextFloat() * levelScale - currentLevel;

            }
        }
    }

    void releaseResources() override {

    }

    void resized() override {

        decibelLabel .setBounds (10, 10, 120, 20);
        gainController.setBounds (130, 10, getWidth() - 140, 20);

    }

private:

    juce::Random random;
    GainController gainController;
    juce::Label decibelLabel;
    float level = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};