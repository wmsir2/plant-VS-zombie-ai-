#include "Peashooter.h"
#include <graphics.h>
#include "config.h"
#include "tool.h"

Peashooter::Peashooter()
    : Plant("Õ„∂π…‰ ÷", 300, 0, 0), currentFrame(0) {
    lastUpdateTime = std::chrono::steady_clock::now();
}

Peashooter::Peashooter(int row, int col)
    : Plant("Õ„∂π…‰ ÷", 300, row, col), currentFrame(0) {
    lastUpdateTime = std::chrono::steady_clock::now();
}

void Peashooter::update() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime).count();
    if (elapsed >= frameDuration) {
        currentFrame = (currentFrame + 1) % PEASHOOTER_FRAME_COUNT;
        lastUpdateTime = now;
    }
}

void Peashooter::draw(int x, int y) {
    putimagePNG(x, y, &imgPeashooterFrames[currentFrame]);
}
