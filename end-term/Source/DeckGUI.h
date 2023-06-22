/*
  ==============================================================================

    DeckGUI.h
    Created: 8 Mar 2022 
    Author:  Hao Fan

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"


//==============================================================================
/*
*/
class DeckGUI : public Component,
                public Button::Listener,
                public Slider::Listener,
                public FileDragAndDropTarget,
                public Timer
{
public:
    DeckGUI(int _id,
            DJAudioPlayer* player,
            AudioFormatManager & formatManagerToUse,
            AudioThumbnailCache & cacheToUse);
    ~DeckGUI() override;

    void paint (Graphics&) override;
    void resized() override;

    /*  implement Button::Listener  */
    void buttonClicked(Button *) override;

    /**  implement Slider::Listener**/
    void sliderValueChanged(Slider  *slider) override;

    /**Detects if file is being dragged over deck*/
    bool isInterestedInFileDrag(const StringArray &files) override;

    /**Detects if file is dropped onto deck*/
    void filesDropped(const StringArray &files, int x, int y) override;

    /**Listen for changes to the waveform*/
    void timerCallback()override;

    //A function that takes care of the painting and graphical representation of the buttons
    void buttonsRepainting();

private:
    int id;
    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };
    TextButton loadButton{ "LOAD" };
    TextButton fforward{ ">back<" };
    TextButton rewind{ ">forward<" };
    ToggleButton loop{ "LOOP" };

    Slider volSlider;
    Slider posSlider;
    Slider speedSlider;
   
    /*   A label to show the title of volume control silder  */
    Label volLabel;
    /*   A label to show the title of play speed control silder  */
    Label speedLabel;
    /*   A label to show the title of playhead position control silder  */
    Label posLabel;

    DJAudioPlayer* player;

    WaveformDisplay waveformDisplay;
    void loadFile(juce::URL audioURL);
    friend class PlaylistComponent;

    void buttonTimerCallback();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
