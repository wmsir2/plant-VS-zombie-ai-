#include "Sunflower.h"
#include <graphics.h>
#include "config.h"
#include "tool.h"

Sunflower::Sunflower()
    : Plant("向日葵", 300, 0, 0), currentFrame(0) {
    lastUpdateTime = std::chrono::steady_clock::now();
}

Sunflower::Sunflower(int row, int col)
    : Plant("向日葵", 300, row, col), currentFrame(0) {
    lastUpdateTime = std::chrono::steady_clock::now();
}

void Sunflower::update() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime).count();
    if (elapsed >= frameDuration) {
        currentFrame = (currentFrame + 1) % SUNFLOWER_FRAME_COUNT;
        lastUpdateTime = now;
    }
}

void Sunflower::draw(int x, int y) {
    putimagePNG(x, y, &imgSunflowerFrames[currentFrame]);
}
