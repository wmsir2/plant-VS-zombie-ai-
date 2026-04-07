#include "Bullet.h"
#include "tool.h"

Bullet::Bullet(int startX, int startY, int row) : x(startX), y(startY), row(row) {}

void Bullet::update() {
    x += speed;
    if (x > 900) active = false;
}

void Bullet::draw() {
    putimagePNG(x, y, &imgBulletNormal);
}
