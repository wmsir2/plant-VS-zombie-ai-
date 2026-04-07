#pragma once
#include <vector>
#include <memory>
#include "Plant.h"

class GameMap {
private:
    int rows;
    int columns;
    std::vector<std::vector<std::shared_ptr<Plant>>> map;

public:
    GameMap(int rows, int columns);
    bool canPlacePlant(int row, int column) const;
    int getRows() const { return rows; }
    int getColumns() const { return columns; }
    void placePlant(int row, int column, std::shared_ptr<Plant> plant);
    std::shared_ptr<Plant> getPlant(int row, int column) const;
    void removePlant(int row, int column);
    void display();
    void update();
    void removeDeadPlants();
};
