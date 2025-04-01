cmake_minimum_required(VERSION 3.16)
project(AutoDiscordMute)

# Ищем Geode SDK
find_package(Geode REQUIRED)

# Создаём библиотеку для мода
add_library(${PROJECT_NAME} SHARED src/main.cpp)

# Линкуем с Geode
target_link_libraries(${PROJECT_NAME} Geode)
