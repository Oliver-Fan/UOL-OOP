/*
  ==============================================================================

    WaveformDisplay.h
    Created: 8 Mar 2022 
    Author:  Hao Fan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public Component,
                        public  ChangeListener
{
public:
    WaveformDisplay(int _id,
                    AudioFormatManager& formatManagerToUse,
                    AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (Graphics&) override;
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    void loadURL(URL audioURL);
    /*  set the relative position of the playhead */
    void setPositionRelative(double pos);

private:
    int id;
    // get play head
    AudioThumbnail audioThumb;
    bool fileLoaded;
    //get position of music play progress
    double position;
    //get file name 
    String fileName;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
