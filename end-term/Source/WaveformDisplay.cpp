/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 8 Mar 2022
    Author:  Hao Fan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(int _id,
                                 AudioFormatManager& formatManagerToUse,
                                 AudioThumbnailCache& cacheToUse):
                                 audioThumb(1000, formatManagerToUse,cacheToUse),
                                 fileLoaded(false),
                                 position(0),id(_id)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::orange);

    g.setFont(18.0f);
    g.drawText("Deck: " + std::to_string(id), getLocalBounds(),
        juce::Justification::topLeft, true);
   
    if (fileLoaded) {
        g.setFont(15.0f);
        audioThumb.drawChannel(g,
            getLocalBounds(),
            0,
            audioThumb.getTotalLength(),
            0,
            1.0f);

        // set colour of playhead
        g.setColour(Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, getWidth()/20, getHeight());

        /* show the name of file when loaded*/
        g.setColour(juce::Colours::red);
        g.drawText(fileName, getLocalBounds(),
            juce::Justification::bottomLeft, true);
    }
    else {
        g.setFont(20.0f);
        g.drawText("File not loaded...", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}


void WaveformDisplay::loadURL(URL audioURL) 
{
    audioThumb.clear();

   fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
   if (fileLoaded) {
       DBG("WaveformDisplay::loadURL file loaded");
       fileName = audioURL.getFileName();
       repaint();
   }
};

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source) 
{

    repaint();

};

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position && pos>0) {
            position = pos;
            repaint();
    }
    
}