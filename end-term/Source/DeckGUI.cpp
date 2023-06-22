/*
  ==============================================================================

    DeckGUI.cpp
    Created: 8 Mar 2022
    Author:  Hao Fan

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(int _id,
                 DJAudioPlayer* _player, 
                 AudioFormatManager& formatManagerToUse,
                 AudioThumbnailCache& cacheToUse
                ): player(_player),
                    id(_id),
                   waveformDisplay(id,formatManagerToUse,cacheToUse)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loop);
    addAndMakeVisible(fforward);
    addAndMakeVisible(rewind);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(volLabel);

    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedLabel);

    addAndMakeVisible(posSlider);
    addAndMakeVisible(posLabel);

    addAndMakeVisible(waveformDisplay);


    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider .addListener(this);
    speedSlider .addListener(this);
    posSlider .addListener(this);

    rewind .addListener(this);
    fforward.addListener(this);
    loop.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);


    //configure volume slider and label
    double volDefaultValue = 0.5;
    volSlider.setRange(0.0, 1.0);
    volSlider.setNumDecimalPlacesToDisplay(2);

    //set the slider to be round
    volSlider.setSliderStyle(Slider::Rotary);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
        false,
        50,
        volSlider.getTextBoxHeight()
    );
    volSlider.setValue(volDefaultValue);
    volSlider.setSkewFactorFromMidPoint(volDefaultValue);
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.attachToComponent(&volSlider, true);

    //configure speed slider and label
    double speedDefaultValue = 1.0;
    speedSlider.setRange(0.25, 4.0); //reaches breakpoint if sliderValue == 0
    speedSlider.setNumDecimalPlacesToDisplay(2);
    //set the slider to be round
    speedSlider.setSliderStyle(Slider::Rotary);

    //set the input text field
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
        false,
        50,
        speedSlider.getTextBoxHeight()
    );
    speedSlider.setValue(speedDefaultValue);
    speedSlider.setSkewFactorFromMidPoint(speedDefaultValue);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, true);

    //configure position slider and label
    posSlider.setRange(0.0, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(2);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
        false,
        50,
        posSlider.getTextBoxHeight()
    );
    posLabel.setText("Position", juce::dontSendNotification);
    posLabel.attachToComponent(&posSlider, true);
    
    //set TImer
    startTimer(500);

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    //A call to the buttons painting functions
    buttonsRepainting();

    //set color for rotarySlider 
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::skyblue);
    getLookAndFeel().setColour(Slider::rotarySliderOutlineColourId, Colours::dimgrey);
    getLookAndFeel().setColour(Slider::rotarySliderFillColourId, Colours::yellow);

}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto sliderLeft = getWidth() / 9;
   
    playButton.setBounds(0, 0, getWidth() / 6, getHeight() / 8);
    stopButton.setBounds(getWidth() / 6, 0, getWidth() / 6, getHeight() / 8);
    loadButton.setBounds(2 * getWidth() / 6, 0, getWidth() / 6, getHeight() / 8);
    loop.setBounds(3 * getWidth() / 6, 0, getWidth() / 6, getHeight() / 8);
    fforward.setBounds(4 * getWidth() / 6, 0, getWidth() / 6, getHeight() / 8);
    rewind.setBounds(5 * getWidth() / 6, 0, getWidth() / 6, getHeight() / 8);

    volSlider.setBounds(getWidth()/10, getHeight() / 8, 5*getWidth() /10, 3*getHeight() / 8-30);
    speedSlider.setBounds(6*getWidth()/10,getHeight() / 8, 5*getWidth()/10, 3*getHeight() / 8-30);
    posSlider.setBounds(sliderLeft, 3 * getHeight() / 8 , getWidth() - sliderLeft, getHeight() / 8);
    waveformDisplay.setBounds(0,  (getHeight() / 2), getWidth(),  getHeight()/2);
    
    DBG("MainComponent::resized");
}
void DeckGUI::buttonClicked(Button *button)
{
    if (button == &playButton) {
        std::cout << "Play button was clicked" << std::endl;
        player->start();
    }
    if (button == &stopButton) {
        std::cout << "Stop button was clicked" << std::endl;
        player->stop();
    }
    if (button == &loadButton) {
        FileChooser chooser{ "Select a file..." };
        if (chooser.browseForFileToOpen()) {
            //The URL of the file is passed to the loadURL function of the player and is loaded
           //  into the transportSource object of the player
            player->loadURL(URL{ chooser.getResult() });
            //The URL of the file is passed to the loadURL function of the waveformDisplay object and is loaded
           //  into the audioThumb object 
            waveformDisplay.loadURL(URL{ chooser.getResult() });

            //if the the loop toggle button is on, the player is set to loop the track
            if (loop.getToggleState() == true)
            {
                player->setLoop();
            }
        }
    }
    if (button == &loop)
    {
        if (loop.getToggleState() == true)
        {
            player->setLoop();
        }
        else
        {
            player->unsetLoop();
        }
    }
}

void DeckGUI::sliderValueChanged(Slider *slider)
{
    if (slider == &volSlider) {
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider) {
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider) {
        player->setPositionRelative(slider->getValue());
        //control play head to move with position slider pointer
        waveformDisplay.setPositionRelative(player->getPositionRelative());
    }
}

bool DeckGUI::isInterestedInFileDrag(const StringArray &files) {
    DBG("DeckGUI::isInterestedInFileDrag called. "
        + std::to_string(files.size()) + " file(s) being dragged.");
    return true;
};
void DeckGUI::filesDropped(const StringArray &files, int x, int y) {
    DBG("DeckGUI::filesDropped at " + std::to_string(x)
        + "x and " + std::to_string(y) + "y");
    if (files.size() == 1) {
        player->loadURL(URL{ File{files[0]} });
        waveformDisplay.loadURL(juce::URL{ juce::File{files[0]} });
    }
};

void DeckGUI::loadFile(juce::URL audioURL)
{
    DBG("DeckGUI::loadFile called");
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
};

void DeckGUI::timerCallback()
//check if the relative position is greater than 0
    //otherwise loading file causes error
{
    buttonTimerCallback();
    //Moving the position slider according to the new position of the transportSource 
    //normalized between 0 and 1
  
    if(player->getPositionRelative()>0){
    posSlider.setValue(player->getPositionRelative(), juce::NotificationType::dontSendNotification);
    //The playhead position is updated according to the player's relative position
    waveformDisplay.setPositionRelative(
        player->getPositionRelative());
    }
};

void DeckGUI::buttonTimerCallback()
{
    //If the rewind button is down and cue is not being used,
    //Also, if the player is playing, the state of the rewind button is set to on
    //and the player rewind function gets called
    if (rewind.isDown()==false)
    {
        if (player->transportSource.isPlaying())
        {
            rewind.setToggleState(true, false);
            player->rewind();
        }
    }
    //else the toggle state is set to false
    else
    {
        rewind.setToggleState(false, false);
    }

    //As above the same checks are done,
    //if the conditions are true, the fforward toggle state is set to true and 
    //the player forward function gets called
    if (fforward.isDown() == false)
    {
        if (player->transportSource.isPlaying())
        {
            fforward.setToggleState(true, false);
            player->forward();
        }
    }
    //else, the toggle state is set to false
    else
    {
        fforward.setToggleState(false, false);
    }
}

//Function takes care of the painting of the buttons
void DeckGUI::buttonsRepainting()
{
    //The below if conditions checks whether the mouse is over the respective button or not
    //If the mouse is over the button, it gets painted a darkcyan colour,
    //  if not, it is painted a darkslategrey colour
    if (playButton.isOver())
    {
        playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkcyan);
    }
    else
    {
        playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);
    }

    if (stopButton.isOver())
    {
        stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkcyan);
    }
    else
    {
        stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);
    }

    if (loadButton.isOver())
    {
        loadButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkcyan);
    }
    else
    {
        loadButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);
    }

    if (rewind.isOver())
    {
        rewind.setColour(juce::TextButton::buttonColourId, juce::Colours::darkcyan);
    }
    else
    {
        rewind.setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);
    }

    if (fforward.isOver())
    {
        fforward.setColour(juce::TextButton::buttonColourId, juce::Colours::darkcyan);
    }
    else
    {
        fforward.setColour(juce::TextButton::buttonColourId, juce::Colours::darkslategrey);
    }


    //The below statements sets the colour of the buttons darkorange whenever the buttons togglestates are on
    rewind.setColour(juce::TextButton::buttonOnColourId, juce::Colours::darkorange);
    fforward.setColour(juce::TextButton::buttonOnColourId, juce::Colours::darkorange);

    //This sets the colour of the tick of the loop togglebutton
    loop.getLookAndFeel().setColour(juce::ToggleButton::tickColourId, juce::Colours::orange);
}