#include <Geode/Geode.hpp>
#include <Geode/ui/Elements.hpp>
#include <fstream>
#include <json/json.h>

using namespace geode::prelude;

// Read the mute threshold from the config
int getMuteThreshold() {
    std::ifstream configFile("config.json");
    Json::Value config;
    configFile >> config;
    return config["muteThreshold"].asInt();  // Get the threshold
}

// Save the mute threshold to the config file
void saveMuteThreshold(int threshold) {
    Json::Value config;
    config["muteThreshold"] = threshold;

    std::ofstream configFile("config.json");
    configFile << config;
}

class MuteThresholdDialog : public geode::ui::AlertDialog {
public:
    MuteThresholdDialog()
        : AlertDialog("Mute Threshold Settings", "Enter the threshold percentage (0-100):") {

        // Create text input for the mute threshold
        auto inputField = geode::ui::TextInput::create();
        inputField->setText(std::to_string(getMuteThreshold()));  // Set the current threshold
        inputField->setPosition(100, 50);  // Position of the text input field

        // Create a save button
        auto saveButton = geode::ui::Button::create("Save", [this, inputField]() {
            int threshold = std::stoi(inputField->getText());  // Get the entered text
            if (threshold >= 0 && threshold <= 100) {
                saveMuteThreshold(threshold);  // Save the threshold
                this->dismiss();  // Close the dialog
            } else {
                geode::ui::AlertDialog::show("Error", "The threshold must be between 0 and 100.");
            }
        });
        saveButton->setPosition(100, 100);  // Position of the save button

        this->addChild(inputField);  // Add the text input to the dialog
        this->addChild(saveButton);  // Add the save button to the dialog
    }
};

class $modify(PlayLayer) {
    void updateProgressbar() {
        PlayLayer::updateProgressbar();
        
        int percent = static_cast<int>(this->m_levelPercent);
        static bool isMuted = false;
        int muteThreshold = getMuteThreshold();  // Get the threshold from the config

        if (percent >= muteThreshold && !isMuted) {
            sendDiscordMute(true);  // Mute Discord
            isMuted = true;
        } else if (percent < muteThreshold && isMuted) {
            sendDiscordMute(false);  // Unmute Discord
            isMuted = false;
        }
    }

    // Function to open the settings dialog
    void onMenuButtonPressed() {
        // Show the mute threshold settings dialog
        auto dialog = MuteThresholdDialog();
        dialog.show();
    }
};
