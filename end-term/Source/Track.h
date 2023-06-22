/*
  ==============================================================================

    Track.h
    Created: 8 Mar 2022 12:02:43am
    Author:  Hao Fan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Track
{
public:
    Track(juce::File _file);
    juce::File file;
    juce::URL URL;
    juce::String title;
    juce::String length;
    /**objects are compared by title*/
    bool operator==(const juce::String& other) const;
};
