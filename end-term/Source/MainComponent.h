/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <juce_gui_basics\juce_gui_basics.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"



//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...

    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{ 100 };

    // Initialization of a player and the Deck that goes with it
    DJAudioPlayer player1{formatManager};
    DeckGUI deckGUI1{1, &player1, formatManager, thumbCache };

    //Initialization of another player and deck that go together
    DJAudioPlayer player2{formatManager};
    DeckGUI deckGUI2{2, &player2, formatManager, thumbCache };

    MixerAudioSource mixerSource;

    PlaylistComponent playlistComponent{ &deckGUI1, &deckGUI2,&playerForParsingMetaData };

    DJAudioPlayer playerForParsingMetaData{ formatManager };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
