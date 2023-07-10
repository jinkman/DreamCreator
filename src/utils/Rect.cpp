#include "Types.h"

namespace DM {

void Rect::scale(float scaleX, float scaleY) {
    left *= scaleX;
    right *= scaleY;
    top *= scaleX;
    bottom *= scaleY;
}

bool Rect::setBounds(const Point pts[], int count) {
    if (count <= 0) {
        this->setEmpty();
        return true;
    }
    float minX, maxX;
    float minY, maxY;
    minX = maxX = pts[0].x;
    minY = maxY = pts[0].y;

    for (int i = 1; i < count; i++) {
        auto x = pts[i].x;
        auto y = pts[i].y;
        auto isFinite = ((x + y) * 0 == 0);
        if (!isFinite) {
            setEmpty();
            return false;
        }
        if (x < minX) {
            minX = x;
        }
        if (x > maxX) {
            maxX = x;
        }
        if (y < minY) {
            minY = y;
        }
        if (y > maxY) {
            maxY = y;
        }
    }
    setLTRB(minX, minY, maxX, maxY);
    return true;
}

#define CHECK_INTERSECT(al, at, ar, ab, bl, bt, br, bb) \
    float L = al > bl ? al : bl;                        \
    float R = ar < br ? ar : br;                        \
    float T = at > bt ? at : bt;                        \
    float B = ab < bb ? ab : bb;                        \
    do {                                                \
        if (!(L < R && T < B)) {                        \
            return false;                               \
        }                                               \
    } while (0)
// do the !(opposite) check so we return false if either arg is NaN

bool Rect::intersect(float l, float t, float r, float b) {
    CHECK_INTERSECT(l, t, r, b, left, top, right, bottom);
    this->setLTRB(L, T, R, B);
    return true;
}

bool Rect::intersect(const Rect &a, const Rect &b) {
    CHECK_INTERSECT(a.left, a.top, a.right, a.bottom, b.left, b.top, b.right, b.bottom);
    this->setLTRB(L, T, R, B);
    return true;
}

void Rect::join(float l, float t, float r, float b) {
    // do nothing if the params are empty
    if (l >= r || t >= b) {
        return;
    }
    // if we are empty, just assign
    if (left >= right || top >= bottom) {
        this->setLTRB(l, t, r, b);
    } else {
        left = left < l ? left : l;
        top = top < t ? top : t;
        right = right > r ? right : r;
        bottom = bottom > b ? bottom : b;
    }
}
} // namespace DM