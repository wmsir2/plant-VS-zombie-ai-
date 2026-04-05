#include "tool.h"

void _putimagePNG(int x, int y, IMAGE* picture) {
    DWORD* dst = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture);
    int picture_width = picture->getwidth();
    int picture_height = picture->getheight();
    int graphWidth = getwidth();
    int graphHeight = getheight();

    for (int iy = 0; iy < picture_height; iy++) {
        for (int ix = 0; ix < picture_width; ix++) {
            int dstX = (ix + x) + (iy + y) * graphWidth;
            if (dstX < 0 || dstX >= graphWidth * graphHeight) continue;

            int sa = ((src[ix + iy * picture_width] & 0xff000000) >> 24);
            if (sa == 0) continue; // 完全透明跳过

            int sr = ((src[ix + iy * picture_width] & 0xff0000) >> 16);
            int sg = ((src[ix + iy * picture_width] & 0xff00) >> 8);
            int sb = src[ix + iy * picture_width] & 0xff;

            int dr = ((dst[dstX] & 0xff0000) >> 16);
            int dg = ((dst[dstX] & 0xff00) >> 8);
            int db = dst[dstX] & 0xff;

            dst[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)
                | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)
                | (sb * sa / 255 + db * (255 - sa) / 255);
        }
    }
}

void putimagePNG(int x, int y, IMAGE* picture) {
    if (!picture) return;
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
