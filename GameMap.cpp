#include<iostream>
#include "GameMap.h"
#include "Plant.h"
#include "config.h"

GameMap::GameMap(int rows, int columns) : rows(rows), columns(columns) {
    map.resize(rows, std::vector<std::shared_ptr<Plant>>(columns, nullptr));
}

bool GameMap::canPlacePlant(int row, int column) const {
    if (row < 0 || row >= rows || column < 0 || column >= columns) return false;
    return map[row][column] == nullptr;
}

void GameMap::placePlant(int row, int column, std::shared_ptr<Plant> plant) {
    if (canPlacePlant(row, column)) {
        map[row][column] = plant;
    }
}

std::shared_ptr<Plant> GameMap::getPlant(int row, int column) const {
    if (row >= 0 && row < rows && column >= 0 && column < columns) {
        return map[row][column];
    }
    return nullptr;
}

void GameMap::removePlant(int row, int column) {
    if (row >= 0 && row < rows && column >= 0 && column < columns) {
        map[row][column] = nullptr;
    }
}

void GameMap::removeDeadPlants() {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            if (map[row][col] && map[row][col]->getBlood() <= 0) {
                std::cout << "Plant at (" << row << ", " << col << ") died, removing..." << std::endl;
                map[row][col] = nullptr;
            }
        }
    }
}

void GameMap::update() {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            if (map[row][col]) {
                map[row][col]->update();
            }
        }
    }
}

void GameMap::display() {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            if (map[row][col] != nullptr) {
                int x = MAP_OFFSET_X + col * GRID_WIDTH + 8;
                int y = MAP_OFFSET_Y + row * GRID_HEIGHT - 25;
                map[row][col]->draw(x, y);
            }
        }
    }
}

