/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 8 Mar 2022
    Author:  Hao Fan

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
                                     DeckGUI* _deckGUI2,
                                     DJAudioPlayer* _playerForParsingMetaData
                                     ):  deckGUI1(_deckGUI1),
                                         deckGUI2(_deckGUI2),
                                         playerForParsingMetaData(_playerForParsingMetaData)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    addAndMakeVisible(importButton);
    addAndMakeVisible(searchField);
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(addToPlayer1Button);
    addAndMakeVisible(addToPlayer2Button);

    // attach listeners
    importButton.addListener(this);
    searchField.addListener(this);
    addToPlayer1Button.addListener(this);
    addToPlayer2Button.addListener(this);

    // searchField configuration
    searchField.setTextToShowWhenEmpty("Search Tracks (press Enter to search)",
        juce::Colours::orange);
    searchField.onReturnKey = [this] { searchLibrary(searchField.getText()); };

    // setup table and load library from file
    tableComponent.getHeader().addColumn("Tracks Name", 1, 1);
    tableComponent.getHeader().addColumn("Length", 2, 1);
    tableComponent.getHeader().addColumn("Del", 3, 1);
    tableComponent.setModel(this);
    loadLibrary();
 
}

PlaylistComponent::~PlaylistComponent()
{
    saveLibrary();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    importButton.setBounds(0, 0, getWidth(), getHeight() / 16);
    tableComponent.setBounds(0, 1 * getHeight() / 16, getWidth(), 13 * getHeight() / 16);
    searchField.setBounds(0, 14 * getHeight() / 16, getWidth(), getHeight() / 16);
    addToPlayer1Button.setBounds(0, 15 * getHeight() / 16, getWidth() / 2, getHeight() / 16);
    addToPlayer2Button.setBounds(getWidth() / 2, 15 * getHeight() / 16, getWidth() / 2, getHeight() / 16);

    //set columns
    tableComponent.getHeader().setColumnWidth(1, 12.8 * getWidth() / 20);
    tableComponent.getHeader().setColumnWidth(2, 5 * getWidth() / 20);
    tableComponent.getHeader().setColumnWidth(3, 2 * getWidth() / 20);
}

int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
};

void PlaylistComponent::paintRowBackground(Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected) 
{
    if (rowIsSelected) {
        g.fillAll(Colours::orange);
    }
    else {
        g.fillAll(Colours::darkgrey);
    }
};

void PlaylistComponent::paintCell(Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowNumber < getNumRows()) {
        if (columnId == 1) {
            g.drawText(trackTitles[rowNumber].title,
                2,
                0,
                width - 4,
                height,
                Justification::centredLeft,
                true);
        }if (columnId == 2)
        {
            g.drawText(trackTitles[rowNumber].length,
                2,
                0,
                width - 4,
                height,
                juce::Justification::centred,
                true
            );
        }
    }
};

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    Component* existingComponentToUpdate)
{
    // set the delete button to remove tracks inside the library
    if (columnId == 3) {
        if (existingComponentToUpdate == nullptr) 
        {   
            TextButton* btn = new TextButton("X");
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
};

void PlaylistComponent::buttonClicked(Button* button)
{
    // To import tracks into the library
    if (button == &importButton)
    {
        DBG("Load button clicked");
        importToLibrary();
        tableComponent.updateContent();
    }
    //add imported tracks into the player deck
    else if (button == &addToPlayer1Button)
    {
        DBG("Add to Player 1 clicked");
        loadInPlayer(deckGUI1);
    }
    else if (button == &addToPlayer2Button)
    {
        DBG("Add to Player 2 clicked");
        loadInPlayer(deckGUI2);
    }
    else
    {
        int id = std::stoi(button->getComponentID().toStdString());
        DBG(trackTitles[id].title + " removed from Library");
        deleteFromTracks(id);
        tableComponent.updateContent();
    }

};

void PlaylistComponent::loadInPlayer(DeckGUI* deckGUI)
{
    int selectedRow{ tableComponent.getSelectedRow() };
    if (selectedRow != -1)
    {
        DBG("Adding: " << trackTitles[selectedRow].title << " to Player");
        deckGUI->loadFile(trackTitles[selectedRow].URL);
    }
    else
    {
        juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
            "Add to Deck Information:",
            "Please select a track to add to deck",
            "OK",
            nullptr
        );
    }
};

void PlaylistComponent::importToLibrary()
{
    DBG("PlaylistComponent::importToLibrary called");

    //initialize file chooser
    juce::FileChooser chooser{ "Select files" };
    if (chooser.browseForMultipleFilesToOpen())
    {
        for (const juce::File& file : chooser.getResults())
        {
            juce::String fileNameWithoutExtension{ file.getFileNameWithoutExtension() };
            if (!isInTracks(fileNameWithoutExtension)) // if not already loaded
            {
                Track newTrack{ file };
                juce::URL audioURL{ file };
                newTrack.length = getLength(audioURL);
                trackTitles.push_back(newTrack);
                DBG("loaded file: " << newTrack.title);
            }
            else // display info message
            {
                juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
                    "Load information:",
                    fileNameWithoutExtension + " already loaded",
                    "OK",
                    nullptr
                );
            }
        }
    }
};

bool PlaylistComponent::isInTracks(juce::String fileNameWithoutExtension)
{
    return (std::find(trackTitles.begin(), trackTitles.end(), fileNameWithoutExtension) != trackTitles.end());
}

void PlaylistComponent::deleteFromTracks(int id)
{
    trackTitles.erase(trackTitles.begin() + id);
};

juce::String PlaylistComponent::getLength(juce::URL audioURL)
{
    playerForParsingMetaData->loadURL(audioURL);
    double seconds{ playerForParsingMetaData->getLengthInSeconds() };
    juce::String minutes{ secondsToMinutes(seconds) };
    return minutes;
};

juce::String PlaylistComponent::secondsToMinutes(double seconds)
{
    //find seconds and minutes and make into string
    int secondsRounded{ int(std::round(seconds)) };
    juce::String min{ std::to_string(secondsRounded / 60) };
    juce::String sec{ std::to_string(secondsRounded % 60) };

    if (sec.length() < 2) // if seconds is 1 digit or less
    {
        //add '0' to seconds until seconds is length 2
        sec = sec.paddedLeft('0', 2);
    }
    return juce::String{ min + ":" + sec };
};

void PlaylistComponent::searchLibrary(juce::String searchText)
{
    // search key word of track's name in library
    DBG("Searching library for: " << searchText);
    if (searchText != "")
    {
        int rowNumber = whereInTracks(searchText);
        tableComponent.selectRow(rowNumber);
    }
    else
    {
        tableComponent.deselectAllRows();
    }
};

int PlaylistComponent::whereInTracks(juce::String searchText)
{
    // finds index where track title contains searchText
    auto it = find_if(trackTitles.begin(), trackTitles.end(),
        [&searchText](const Track& obj) {return obj.title.contains(searchText); });
    int i = -1;

    if (it != trackTitles.end())
    {
        i = std::distance(trackTitles.begin(), it);
    }

    return i;
};

void PlaylistComponent::saveLibrary()
{
    // create .csv to save library
    std::ofstream myLibrary("my-library1.csv");

    // save library to file
    for (Track& t : trackTitles)
    {
        myLibrary << t.file.getFullPathName() << "," << t.length << "\n";
    }
};

void PlaylistComponent::loadLibrary()
{
    // create input stream from saved library
    std::ifstream myLibrary("my-library1.csv");
    std::string filePath;
    std::string length;

    // Read data, line by line
    if (myLibrary.is_open()==true)
    {
        while (getline(myLibrary, filePath, ',')) {
            juce::File file{ filePath };
            Track newTrack{ file };

            getline(myLibrary, length);
            newTrack.length = length;
            trackTitles.push_back(newTrack);
        }
    }
    myLibrary.close();
};
