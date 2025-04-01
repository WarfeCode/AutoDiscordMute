#include <Geode/Geode.hpp>
#include <Geode/ui/Elements.hpp>
#include <fstream>
#include <json/json.h>

using namespace geode::prelude;

// Чтение порога из конфигурации
int getMuteThreshold() {
    std::ifstream configFile("config.json");
    Json::Value config;
    configFile >> config;
    return config["muteThreshold"].asInt();  // Читаем порог
}

// Сохранение порога в конфигурационный файл
void saveMuteThreshold(int threshold) {
    Json::Value config;
    config["muteThreshold"] = threshold;

    std::ofstream configFile("config.json");
    configFile << config;
}

class MuteThresholdDialog : public geode::ui::AlertDialog {
public:
    MuteThresholdDialog()
        : AlertDialog("Настройка порога для отключения Discord", "Введите порог в процентах (0-100):") {

        // Создание текстового поля для ввода порога
        auto inputField = geode::ui::TextInput::create();
        inputField->setText(std::to_string(getMuteThreshold()));  // Устанавливаем текущий порог
        inputField->setPosition(100, 50);  // Позиция текстового поля

        // Создание кнопки для сохранения
        auto saveButton = geode::ui::Button::create("Сохранить", [this, inputField]() {
            int threshold = std::stoi(inputField->getText());  // Получаем введённый текст
            if (threshold >= 0 && threshold <= 100) {
                saveMuteThreshold(threshold);  // Сохраняем порог
                this->dismiss();  // Закрываем диалог
            } else {
                geode::ui::AlertDialog::show("Ошибка", "Порог должен быть от 0 до 100.");
            }
        });
        saveButton->setPosition(100, 100);  // Позиция кнопки

        this->addChild(inputField);  // Добавляем поле ввода в диалог
        this->addChild(saveButton);  // Добавляем кнопку в диалог
    }
};

class $modify(PlayLayer) {
    void updateProgressbar() {
        PlayLayer::updateProgressbar();
        
        int percent = static_cast<int>(this->m_levelPercent);
        static bool isMuted = false;
        int muteThreshold = getMuteThreshold();  // Читаем порог из конфигурации

        if (percent >= muteThreshold && !isMuted) {
            sendDiscordMute(true);
            isMuted = true;
        } else if (percent < muteThreshold && isMuted) {
            sendDiscordMute(false);
            isMuted = false;
        }
    }

    // Функция для вызова диалога с настройками
    void onMenuButtonPressed() {
        // Открытие диалога для настройки порога
        auto dialog = MuteThresholdDialog();
        dialog.show();
    }
};
