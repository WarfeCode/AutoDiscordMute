#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Windows.h>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <json/json.h>

#pragma comment(lib, "ws2_32.lib")

using namespace geode::prelude;

const std::string DISCORD_GATEWAY = "wss://gateway.discord.gg/?v=9&encoding=json";

// Отправка команды на Deaf в Discord через WebSocket
void sendDiscordMute(bool mute) {
    // Код подключения к Discord WebSocket и отправки команды Deaf
    // Пример с JSON-пакетом для управления Deafen (наушниками)
    // ...
}

class $modify(PlayLayer) {
    void updateProgressbar() {
        PlayLayer::updateProgressbar();
        
        int percent = static_cast<int>(this->m_levelPercent);
        static bool isMuted = false;

        if (percent >= 70 && !isMuted) { // 70% - триггер Deafen
            sendDiscordMute(true);
            isMuted = true;
        } else if (percent < 70 && isMuted) {
            sendDiscordMute(false);
            isMuted = false;
        }
    }
};
