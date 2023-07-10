#ifndef TYPES_H
#define TYPES_H

#include <nlohmann/json.hpp>

namespace DM {

typedef int64_t DMTime;

typedef int DMFrame;

static DMFrame timeToFrame(DMTime time, float frameRate) {
    return static_cast<DMFrame>(time * frameRate / 1000);
}

/**
 * Defines the rules on how to scale the content to fit the specified area.
 */
class DMScaleMode {
public:
    /**
     * The content is not scaled.
     */
    static const int NoneValue = 0;
    /**
     * The content is stretched to fit.
     */
    static const int Stretch = 1;
    /**
     * The content is scaled with respect to the original unscaled image's aspect ratio.
     * This is the default value.
     */
    static const int LetterBox = 2;
    /**
     * The content is scaled to fit with respect to the original unscaled image's aspect ratio.
     * This results in cropping on one axis.
     */
    static const int Zoom = 3;
};

class GLColor {
public:
    static GLColor deserialization(const nlohmann::json &obj);

    nlohmann::json serialization();

    GLColor(float red, float green, float blue, float alpha);

    GLColor(float red, float green, float blue);

    GLColor(float v);

    GLColor();

    ~GLColor();

    float red() const;

    float green() const;

    float blue() const;

    float alpha() const;

    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;
};

/**
 * Point holds two 32-bit floating point coordinates.
 */
struct Point {
    /**
     * x-axis value.
     */
    float x;
    /**
     * y-axis value.
     */
    float y;

    /**
     * Creates a Point set to (0, 0).
     */
    static const Point &Zero() {
        static const Point zero = Point::Make(0, 0);
        return zero;
    }

    /**
     * Creates a Point with specified x and y value.
     */
    static constexpr Point Make(float x, float y) {
        return {x, y};
    }

    /**
     * Returns true if fX and fY are both zero.
     */
    bool isZero() const {
        return (0 == x) & (0 == y);
    }

    /**
     * Sets fX to x and fY to y.
     */
    void set(float xValue, float yValue) {
        x = xValue;
        y = yValue;
    }

    /**
     * Adds offset (dx, dy) to Point.
     */
    void offset(float dx, float dy) {
        x += dx;
        y += dy;
    }

    /**
     * Returns the Euclidean distance from origin.
     */
    float length() const {
        return Point::Length(x, y);
    }

    /**
     * Returns true if a is equivalent to b.
     */
    friend bool operator==(const Point &a, const Point &b) {
        return a.x == b.x && a.y == b.y;
    }

    /**
     * Returns true if a is not equivalent to b.
     */
    friend bool operator!=(const Point &a, const Point &b) {
        return a.x != b.x || a.y != b.y;
    }

    /**
     * Returns a Point from b to a; computed as (a.fX - b.fX, a.fY - b.fY).
     */
    friend Point operator-(const Point &a, const Point &b) {
        return {a.x - b.x, a.y - b.y};
    }

    /**
     * Returns Point resulting from Point a offset by Point b, computed as:
     * (a.fX + b.fX, a.fY + b.fY).
     */
    friend Point operator+(const Point &a, const Point &b) {
        return {a.x + b.x, a.y + b.y};
    }

    friend Point operator*(const Point &a, const float &b) {
        return {a.x * b, a.y * b};
    }

    friend Point operator*(const float &b, const Point &a) {
        return {a.x * b, a.y * b};
    }

    friend Point operator/(const Point &a, const Point &b) {
        auto maxV = [](const float &a, const float &b) -> float { return a > b ? a : b; };
        return {a.x / maxV(b.x, 1e-6), a.y / maxV(b.y, 1e-6)};
    }

    /**
     * Returns the Euclidean distance from origin.
     */
    static float Length(float x, float y) {
        return sqrt(x * x + y * y);
    }

    /**
     * Returns the Euclidean distance between a and b.
     */
    static float Distance(const Point &a, const Point &b) {
        return Length(a.x - b.x, a.y - b.y);
    }
};

/**
 * Rect holds four float coordinates describing the upper and lower bounds of a rectangle. Rect may
 * be created from outer bounds or from position, width, and height. Rect describes an area; if its
 * right is less than or equal to its left, or if its bottom is less than or equal to its top, it is
 * considered empty.
 */
struct Rect {
    /**
     * smaller x-axis bounds.
     */
    float left;
    /**
     * smaller y-axis bounds.
     */
    float top;
    /**
     * larger x-axis bounds.
     */
    float right;
    /**
     * larger y-axis bounds.
     */
    float bottom;

    /**
     * Returns constructed Rect set to (0, 0, 0, 0).
     */
    static constexpr Rect MakeEmpty() {
        return {0, 0, 0, 0};
    }

    /**
     * Returns constructed Rect set to float values (0, 0, w, h). Does not validate input; w or h may
     * be negative.
     */
    static constexpr Rect MakeWH(float w, float h) {
        return {0, 0, w, h};
    }

    /**
     * Returns constructed Rect set to (l, t, r, b). Does not sort input; Rect may result in left
     * greater than right, or top greater than bottom.
     */
    static constexpr Rect MakeLTRB(float l, float t, float r, float b) {
        return {l, t, r, b};
    }

    /**
     * Returns constructed Rect set to (x, y, x + w, y + h). Does not validate input; w or h may be
     * negative.
     */
    static constexpr Rect MakeXYWH(float x, float y, float w, float h) {
        return {x, y, x + w, y + h};
    }

    /**
     * Returns true if left is equal to or greater than right, or if top is equal to or greater
     * than bottom. Call sort() to reverse rectangles with negative width() or height().
     */
    bool isEmpty() const {
        // We write it as the NOT of a non-empty rect, so we will return true if any values
        // are NaN.
        return !(left < right && top < bottom);
    }

    /**
     * Returns true if left is equal to or less than right, or if top is equal to or less than
     * bottom. Call sort() to reverse rectangles with negative width() or height().
     */
    bool isSorted() const {
        return left <= right && top <= bottom;
    }

    /**
     * Returns left edge of Rect, if sorted. Call isSorted() to see if Rect is valid. Call sort() to
     * reverse left and right if needed.
     */
    float x() const {
        return left;
    }

    /**
     * Returns top edge of Rect, if sorted. Call isEmpty() to see if Rect may be invalid, and sort()
     * to reverse top and bottom if needed.
     */
    float y() const {
        return top;
    }

    /**
     * Returns span on the x-axis. This does not check if Rect is sorted, or if result fits in 32-bit
     * float; result may be negative or infinity.
     */
    float width() const {
        return right - left;
    }

    /**
     * Returns span on the y-axis. This does not check if Rect is sorted, or if result fits in 32-bit
     * float; result may be negative or infinity.
     */
    float height() const {
        return bottom - top;
    }

    /**
     * Returns average of left edge and right edge. Result does not change if Rect is sorted.
     */
    float centerX() const {
        // don't use (left + bottom) * 0.5f as that might overflow before the 0.5f
        return left * 0.5f + right * 0.5f;
    }

    /**
     * Returns average of top edge and bottom edge. Result does not change if Rect is sorted.
     */
    float centerY() const {
        // don't use (top + bottom) * 0.5f as that might overflow before the 0.5f
        return top * 0.5f + bottom * 0.5f;
    }

    /**
     * Returns true if all members in a: left, top, right, and bottom; are equal to the
     * corresponding members in b.
     * a and b are not equal if either contain NaN. a and b are equal if members contain zeroes with
     * different signs.
     */
    friend bool operator==(const Rect &a, const Rect &b) {
        return a.left == b.left && a.right == b.right && a.top == b.top && a.bottom == b.bottom;
    }

    /**
     * Returns true if any in a: left, top, right, and bottom; does not equal the corresponding
     * members in b.
     * a and b are not equal if either contain NaN. a and b are equal if members contain zeroes with
     * different signs.
     */
    friend bool operator!=(const Rect &a, const Rect &b) {
        return a.left != b.left || a.right != b.right || a.top != b.top || a.bottom != b.bottom;
    }

    /**
     * Sets Rect to (0, 0, 0, 0).
     */
    void setEmpty() {
        *this = MakeEmpty();
    }

    /**
     * Sets Rect to (left, top, right, bottom). left and right are not sorted; left is not necessarily
     * less than right. top and bottom are not sorted; top is not necessarily less than bottom.
     */
    void setLTRB(float l, float t, float r, float b) {
        left = l;
        top = t;
        right = r;
        bottom = b;
    }

    /**
     * Sets to bounds of Point array with count entries. Returns false if count is zero or smaller, or
     * if Point array contains an infinity or NaN; in these cases sets Rect to (0, 0, 0, 0).
     * Result is either empty or sorted: left is less than or equal to right, and top is less than
     * or equal to bottom.
     */
    bool setBounds(const Point pts[], int count);

    /**
     * Sets Rect to (x, y, x + width, y + height). Does not validate input; width or height may be
     * negative.
     */
    void setXYWH(float x, float y, float width, float height) {
        left = x;
        top = y;
        right = x + width;
        bottom = y + height;
    }

    /**
     * Sets Rect to (0, 0, width, height). Does not validate input, width or height may be negative.
     */
    void setWH(float width, float height) {
        left = 0;
        top = 0;
        right = width;
        bottom = height;
    }

    /**
     * Returns Rect offset by (dx, dy).
     * If dx is negative, Rect returned is moved to the left.
     * If dx is positive, Rect returned is moved to the right.
     * If dy is negative, Rect returned is moved upward.
     * If dy is positive, Rect returned is moved downward.
     */
    Rect makeOffset(float dx, float dy) const {
        return MakeLTRB(left + dx, top + dy, right + dx, bottom + dy);
    }

    /**
     * Returns Rect, inset by (dx, dy).
     * If dx is negative, Rect returned is wider.
     * If dx is positive, Rect returned is narrower.
     * If dy is negative, Rect returned is taller.
     * If dy is positive, Rect returned is shorter.
     */
    Rect makeInset(float dx, float dy) const {
        return MakeLTRB(left + dx, top + dy, right - dx, bottom - dy);
    }

    /**
     * Returns Rect, outset by (dx, dy).
     * If dx is negative, Rect returned is narrower.
     * If dx is positive, Rect returned is wider.
     * If dy is negative, Rect returned is shorter.
     * If dy is positive, Rect returned is taller.
     */
    Rect makeOutset(float dx, float dy) const {
        return MakeLTRB(left - dx, top - dy, right + dx, bottom + dy);
    }

    /**
     * Offsets Rect by adding dx to left, right; and by adding dy to top, bottom.
     * If dx is negative, moves Rect to the left.
     * If dx is positive, moves Rect to the right.
     * If dy is negative, moves Rect upward.
     * If dy is positive, moves Rect downward.
     */
    void offset(float dx, float dy) {
        left += dx;
        top += dy;
        right += dx;
        bottom += dy;
    }

    /**
     * Offsets Rect by adding delta.fX to left, right; and by adding delta.fY to top, bottom.
     * If delta.fX is negative, moves Rect to the left.
     * If delta.fX is positive, moves Rect to the right.
     * If delta.fY is negative, moves Rect upward.
     * If delta.fY is positive, moves Rect downward.
     */
    void offset(const Point &delta) {
        this->offset(delta.x, delta.y);
    }

    /**
     * Offsets Rect so that left equals newX, and top equals newY. width and height are unchanged.
     */
    void offsetTo(float newX, float newY) {
        right += newX - left;
        bottom += newY - top;
        left = newX;
        top = newY;
    }

    /**
     * Insets Rect by (dx, dy).
     * If dx is positive, makes Rect narrower.
     * If dx is negative, makes Rect wider.
     * If dy is positive, makes Rect shorter.
     * If dy is negative, makes Rect taller.
     */
    void inset(float dx, float dy) {
        left += dx;
        top += dy;
        right -= dx;
        bottom -= dy;
    }

    /**
     * Outsets Rect by (dx, dy).
     * If dx is positive, makes Rect wider.
     * If dx is negative, makes Rect narrower.
     * If dy is positive, makes Rect taller.
     * If dy is negative, makes Rect shorter.
     */
    void outset(float dx, float dy) {
        this->inset(-dx, -dy);
    }

    /**
     * Scale the rectangle by scaleX and scaleY.
     */
    void scale(float scaleX, float scaleY);

    /**
     * Returns true if Rect intersects r, and sets Rect to intersection. Returns false if Rect does
     * not intersect r, and leaves Rect unchanged. Returns false if either r or Rect is empty, leaving
     * Rect unchanged.
     */
    bool intersect(const Rect &r) {
        return this->intersect(r.left, r.top, r.right, r.bottom);
    }

    /**
     * Constructs Rect to intersect from (left, top, right, bottom). Does not sort construction.
     * Returns true if Rect intersects construction, and sets Rect to intersection.
     * Returns false if Rect does not intersect construction, and leaves Rect unchanged.
     * Returns false if either construction or Rect is empty, leaving Rect unchanged.
     */
    bool intersect(float l, float t, float r, float b);

    /**
     * Returns true if a intersects b, and sets Rect to intersection.
     * Returns false if a does not intersect b, and leaves Rect unchanged.
     * Returns false if either a or b is empty, leaving Rect unchanged.
     */
    bool intersect(const Rect &a, const Rect &b);

    /**
     * Constructs Rect to intersect from (left, top, right, bottom). Does not sort construction.
     * Returns true if Rect intersects construction.
     * Returns false if either construction or Rect is empty, or do not intersect.
     */
    bool intersects(float l, float t, float r, float b) const {
        return Intersects(left, top, right, bottom, l, t, r, b);
    }

    /**
     * Returns true if Rect intersects r. Returns false if either r or Rect is empty, or do not
     * intersect.
     */
    bool intersects(const Rect &r) const {
        return Intersects(left, top, right, bottom, r.left, r.top, r.right, r.bottom);
    }

    /**
     * Returns true if a intersects b. Returns false if either a or b is empty, or do not intersect.
     */
    static bool Intersects(const Rect &a, const Rect &b) {
        return Intersects(a.left, a.top, a.right, a.bottom, b.left, b.top, b.right, b.bottom);
    }

    /**
     * Constructs Rect to intersect from (left, top, right, bottom). Does not sort construction.
     * Sets Rect to the union of itself and the construction. Has no effect if construction is empty.
     * Otherwise, if Rect is empty, sets Rect to construction.
     */
    void join(float l, float t, float r, float b);

    /**
     * Sets Rect to the union of itself and r. Has no effect if r is empty. Otherwise, if Rect is
     * empty, sets Rect to r.
     */
    void join(const Rect &r) {
        this->join(r.left, r.top, r.right, r.bottom);
    }

    /**
     * Returns true if: left <= x < right && top <= y < bottom. Returns false if Rect is empty.
     */
    bool contains(float x, float y) const {
        return x >= left && x < right && y >= top && y < bottom;
    }

    /**
     * Returns true if Rect contains r. Returns false if Rect is empty or r is empty. Rect contains r
     * when Rect area completely includes r area.
     */
    bool contains(const Rect &r) const {
        return left <= r.left && top <= r.top && right >= r.right && bottom >= r.bottom;
    }

    /**
     * Sets Rect by discarding the fractional portion of left and top; and rounding up right and
     * bottom.
     */
    void roundOut() {
        left = floorf(left);
        top = floorf(top);
        right = ceilf(right);
        bottom = ceilf(bottom);
    }

    /**
     * Sets Rect by rounding of left, top,  right and bottom.
     */
    void round() {
        left = roundf(left);
        top = roundf(top);
        right = roundf(right);
        bottom = roundf(bottom);
    }

    /**
     * Swaps left and right if left is greater than right; and swaps top and bottom if top is
     * greater than bottom. Result may be empty; and width() and height() will be zero or positive.
     */
    void sort() {
        if (left > right) {
            std::swap(left, right);
        }

        if (top > bottom) {
            std::swap(top, bottom);
        }
    }

    /**
     * Returns Rect with left and right swapped if left is greater than right, and with top and
     * bottom swapped if top is greater than bottom. Result may be empty, and width() and height()
     * will be zero or positive.
     */
    Rect makeSorted() const {
        return MakeLTRB(std::min(left, right), std::min(top, bottom), std::max(left, right),
                        std::max(top, bottom));
    }

private:
    static bool Intersects(float al, float at, float ar, float ab, float bl, float bt, float br,
                           float bb) {
        float L = al > bl ? al : bl;
        float R = ar < br ? ar : br;
        float T = at > bt ? at : bt;
        float B = ab < bb ? ab : bb;
        return L < R && T < B;
    }
};

} // namespace DM

#endif // TYPES_H