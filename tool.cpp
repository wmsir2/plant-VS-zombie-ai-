#include "tool.h"

void _putimagePNG(int x, int y, IMAGE* picture) {
    DWORD* dst = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture);
    int picture_width = picture->getwidth();
    int picture_height = picture->getheight();
    int graphWidth = getwidth();
    int graphHeight = getheight();

    // 优化：检查边界，避免越界访问
    int startX = max(0, x);
    int startY = max(0, y);
    int endX = min(graphWidth, x + picture_width);
    int endY = min(graphHeight, y + picture_height);

    // 跳过完全在屏幕外的图片
    if (startX >= endX || startY >= endY) return;

    for (int iy = startY - y; iy < endY - y; iy++) {
        for (int ix = startX - x; ix < endX - x; ix++) {
            int srcIdx = ix + iy * picture_width;
            int dstIdx = (ix + x) + (iy + y) * graphWidth;

            // 获取源像素的 alpha 值
            int sa = ((src[srcIdx] & 0xff000000) >> 24);
            if (sa == 0) continue;  // 完全透明跳过

            // 如果完全不透明，直接复制像素
            if (sa == 255) {
                dst[dstIdx] = src[srcIdx];
                continue;
            }

            // 透明度计算（使用右移和查表优化）
            int sr = ((src[srcIdx] & 0xff0000) >> 16);
            int sg = ((src[srcIdx] & 0xff00) >> 8);
            int sb = src[srcIdx] & 0xff;

            int dr = ((dst[dstIdx] & 0xff0000) >> 16);
            int dg = ((dst[dstIdx] & 0xff00) >> 8);
            int db = dst[dstIdx] & 0xff;

            // 使用更快的 alpha 混合: dst = src * (sa/255) + dst * ((255-sa)/255)
            int invSa = 255 - sa;
            dst[dstIdx] = (((sr * sa + dr * invSa) / 255) << 16)
                | (((sg * sa + dg * invSa) / 255) << 8)
                | ((sb * sa + db * invSa) / 255);
        }
    }
}

void putimagePNG(int x, int y, IMAGE* picture) {
    if (!picture) return;
    // 检查图片是否有效
    if (picture->getwidth() <= 0 || picture->getheight() <= 0) return;
    _putimagePNG(x, y, picture);
}

int getDelay() {
    static unsigned long long lastTime = 0;
    unsigned long long currentTime = GetTickCount();
    if (!lastTime) { lastTime = currentTime; return 0; }
    int ret = currentTime - lastTime;
    lastTime = currentTime;
    return ret;
}
