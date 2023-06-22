/*
  ==============================================================================

    PlaylistComponent.h
    Created: 8 Mar 2022 
    Author:  Hao Fan

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include "Track.h"
#include "DeckGUI.h"
#include "DJAudioPlayer.h"


//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public TableListBoxModel,
                            public Button::Listener,
                            public juce::TextEditor::Listener
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1,
                      DeckGUI* _deckGUI2,
                      DJAudioPlayer* _playerForParsingMetaData);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(Graphics& g,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected)override;

    void paintCell(Graphics& g,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected)override;

    Component* refreshComponentForCell(int rowNumber,
        int columnId,
        bool isRowSelected,
        Component* existingComponentToUpdate)override;


    void buttonClicked(Button* button)override;

private:
 
    std::vector<Track> trackTitles;
    juce::TextButton importButton{ "IMPORT TRACKS" };
    juce::TextEditor searchField;
    juce::TableListBox tableComponent;
    juce::TextButton addToPlayer1Button{ "ADD TO DECK 1" };
    juce::TextButton addToPlayer2Button{ "ADD TO DECK 2" };

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* playerForParsingMetaData;

    juce::String getLength(juce::URL audioURL);
    juce::String secondsToMinutes(double seconds);

    //import tracks to library
    void importToLibrary();
    // search field in library
    void searchLibrary(juce::String searchText);
    void saveLibrary();
    void loadLibrary();

    //delete from library
    void deleteFromTracks(int id);

    //detect tracks status in library
    bool isInTracks(juce::String fileNameWithoutExtension);
    int whereInTracks(juce::String searchText);
    void loadInPlayer(DeckGUI* deckGUI);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};