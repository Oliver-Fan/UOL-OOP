/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 8 Mar 2022 
    Author:  Hao Fan

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer :public AudioSource {
public:
    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //Implementing the below 4 function since we inherit from PositionableAudioSource class to implement the looping function
    void setNextReadPosition(juce::int64 newPosition) ;
    juce::int64 getNextReadPosition() const ;
    juce::int64 getTotalLength() const ;
    bool isLooping() const;

    /**Loads the audio file*/
    void loadURL(URL audioURL);
    /**Sets the volume*/
    void setGain(double gain);
    /**Sets the speed*/
    void setSpeed(double ratio);
    /**Sets the playing position of audio file*/
    void setPosition(double posInSecs);
    /**Sets relative position of audio file*/
    void setPositionRelative(double pos);

    //Functions to set the playback to loop or not to loop
    void setLoop();
    void unsetLoop();
    void rewind();
    void forward();

    /**Start playing audio file*/
    void start();
    /**Stops playing audio file*/
    void stop();

    /*  get the relative position of the playhead */
    double getPositionRelative();
    /**Gets the length of transport source in seconds*/
    double getLengthInSeconds();
    AudioTransportSource transportSource;

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource>readerSource;
    ResamplingAudioSource resampleSource{ &transportSource,false,2 };
};