// PlantCard.cpp
#include "PlantCard.h"

PlantCard::PlantCard(const std::string& name, const std::string& cardImagePath, const std::string& plantImagePath, int cost)
    : name(name), imagePath(cardImagePath), plantImagePath(plantImagePath), cost(cost) {
    loadimage(&image, imagePath.c_str()); // º”‘ÿø®∆¨Õº∆¨
}

void PlantCard::display(int x, int y) {
    putimage(x, y, &image); // ‘⁄÷∏∂®Œª÷√ªÊ÷∆ø®∆¨ÕºœÒ
}

const std::string& PlantCard::getPlantImagePath() const {
    return plantImagePath;
}

std::string PlantCard::getName() const {
    return name;
}

int PlantCard::getCost() const {
    return cost;
}

IMAGE& PlantCard::getImage() {
    return image;
}
