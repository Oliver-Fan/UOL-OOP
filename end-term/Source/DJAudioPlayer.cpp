/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 8 Mar 2022 
    Author:  Hao Fan

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
:formatManager(_formatManager)
{
}
DJAudioPlayer::~DJAudioPlayer() {
}

//==============================================================================
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
   
    transportSource.prepareToPlay(samplesPerBlockExpected,sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

};
void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
    resampleSource.getNextAudioBlock(bufferToFill);
};
void DJAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    resampleSource.releaseResources();
    
};

void DJAudioPlayer::loadURL(URL audioURL) {
    DBG("DJAudioPlayer::loadURL called");
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr)//if good file!
    {
        std::unique_ptr<AudioFormatReaderSource>newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());

    }
};
void DJAudioPlayer::setGain(double gain) {
    //set maximum and minimum value for volume slider
    if (gain < 0 || gain >1.0) {
        std::cout << "DJAudioPlayer::setGain gain shoule be between 0 & 1 " << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }

   
};
void DJAudioPlayer::setSpeed(double ratio) {
    //set maximum and minimum value for speed slider
    if (ratio < 0.25 || ratio >4.0) {
        std::cout << "DJAudioPlayer::setSpeed ratio shoule be between 0.25 & 4 " << std::endl;
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
};
void DJAudioPlayer::setPosition(double posInSecs) {
    transportSource.setPosition(posInSecs);
};

void DJAudioPlayer::setPositionRelative(double pos) {
    //set minimum and maximum value for position slider
    if (pos < 0 || pos >1.0) {
        std::cout << "DJAudioPlayer::setPositionRelative pos shoule be between 0 & 1 " << std::endl;
    }
    else {
        double posInsecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInsecs);
    }
};

void DJAudioPlayer::start() {
    transportSource.start();
};
void DJAudioPlayer::stop() {
    transportSource.stop();
};

double DJAudioPlayer::getPositionRelative()
//get music play progress position
{
    return transportSource.getCurrentPosition()/ transportSource.getLengthInSeconds();
};

double DJAudioPlayer::getLengthInSeconds()
{
    return transportSource.getLengthInSeconds();
};

void DJAudioPlayer::rewind()
{
    //First a check is made to see if after moving back,
    //  the new position value is less than 0 or not 
    if (transportSource.getCurrentPosition() - 1 > 0)
    {
        transportSource.setPosition(transportSource.getCurrentPosition() - 1.5);
    }
}

//Moving the playback forward
void DJAudioPlayer::forward()
{
    //Calculating the last position on the transportSource - end of the track
    double last_pos{ transportSource.getLengthInSeconds() };

    //If the new calculated value is equal or greater to the last position 
    //It will not move and it will not update the position
    //It will update the position only if the new position is in range
    if (transportSource.getCurrentPosition() + 0.5 != last_pos || transportSource.getCurrentPosition() + 0.5 > last_pos)
    {
        transportSource.setPosition(transportSource.getCurrentPosition() + 1.5);
    }
}

//The below 4 functions are pure virtual functions that need to be implemented
//  since we inherit from the PositionableAudioSource class
void DJAudioPlayer::setNextReadPosition(juce::int64 newPosition)
{

}

juce::int64 DJAudioPlayer::getNextReadPosition() const
{
    return readerSource->getNextReadPosition();
}

juce::int64 DJAudioPlayer::getTotalLength() const
{
    return readerSource->getTotalLength();
}

//Returning true if it is looping, or false if it is not
bool DJAudioPlayer::isLooping() const
{
    return false;
}

//Setting the playback to loop
void DJAudioPlayer::setLoop()
{
    //if the readerSource is not a null pointer, the readerSource is set to loop
    //using the function from the PositionableAudioSource class
    if (readerSource != nullptr)
    {
        readerSource->setLooping(true);
    }
}

//Setting the playback not to loop
void DJAudioPlayer::unsetLoop()
{
    //if the readerSource is not a null pointer, the readerSource is set to not loop
    //using the function from the PositionableAudioSource class
    if (readerSource != nullptr)
    {
        readerSource->setLooping(false);
    }
}