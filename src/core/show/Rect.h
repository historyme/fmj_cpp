#ifndef _Rect_h_
#define _Rect_h_

#include <iostream>
#include <sstream>

class Rect
{
public:
    int left;
    int top;
    int right;
    int bottom;


    /**
    * Create a new empty Rect. All coordinates are initialized to 0.
    */
    Rect() {}

    /**
    * Create a new rectangle with the specified coordinates. Note: no range
    * checking is performed, so the caller must ensure that left <= right and
    * top <= bottom.
    *
    * @param left   The X coordinate of the left side of the rectangle
    * @param top    The Y coordinate of the top of the rectangle
    * @param right  The X coordinate of the right side of the rectangle
    * @param bottom The Y coordinate of the bottom of the rectangle
    */
    Rect(int left, int top, int right, int bottom);

    /**
    * Create a new rectangle, initialized with the values in the specified
    * rectangle (which is left unmodified).
    *
    * @param r The rectangle whose coordinates are copied into the new
    *          rectangle.
    */
    Rect(Rect *r);


    int hashCode();


    std::string toString();


    /**
    * Return a string representation of the rectangle in a compact form.
    */
    std::string toShortString();


    /**
    * Return a string representation of the rectangle in a compact form.
    * @hide
    */
    std::string Rect::toShortString(std::stringstream *sb);


    /**
    * Return a string representation of the rectangle in a well-defined format.
    *
    * <p>You can later recover the Rect from this string through
    * {@link #unflattenFromString(std::string)}.
    *
    * @return Returns a new std::string of the form "left top right bottom"
    */
    std::string flattenToString();


    /**
    * Returns a Rect from a string of the form returned by {@link #flattenToString},
    * or NULL if the string is not of that form.
    */
//    Rect unflattenFromString(std::string str);

    /**
    * Print short representation to given writer->
    * @hide
    */
//    void printShortString(PrintWriter pw);


    /**
    * Returns true if the rectangle is empty (left >= right or top >= bottom)
    */
    bool isEmpty();


    /**
    * @return the rectangle's width. This does not check for a valid rectangle
    * (i.e. left <= right) so the result may be negative.
    */
    int width();


    /**
    * @return the rectangle's height. This does not check for a valid rectangle
    * (i.e. top <= bottom) so the result may be negative.
    */
    int height();


    /**
    * @return the horizontal center of the rectangle. If the computed value
    *         is fractional, this method returns the largest integer that is
    *         less than the computed value.
    */
    int centerX();


    /**
    * @return the vertical center of the rectangle. If the computed value
    *         is fractional, this method returns the largest integer that is
    *         less than the computed value.
    */
    int centerY();


    /**
    * @return the exact horizontal center of the rectangle as a float.
    */
    float exactCenterX();


    /**
    * @return the exact vertical center of the rectangle as a float.
    */
    float exactCenterY();


    /**
    * Set the rectangle to (0,0,0,0)
    */
    void setEmpty();


    /**
    * Set the rectangle's coordinates to the specified values. Note: no range
    * checking is performed, so it is up to the caller to ensure that
    * left <= right and top <= bottom.
    *
    * @param left   The X coordinate of the left side of the rectangle
    * @param top    The Y coordinate of the top of the rectangle
    * @param right  The X coordinate of the right side of the rectangle
    * @param bottom The Y coordinate of the bottom of the rectangle
    */
    void set(int left, int top, int right, int bottom);


    /**
    * Copy the coordinates from src into this rectangle.
    *
    * @param src The rectangle whose coordinates are copied into this
    *           rectangle.
    */
    void set(Rect *src);


    /**
    * Offset the rectangle by adding dx to its left and right coordinates, and
    * adding dy to its top and bottom coordinates.
    *
    * @param dx The amount to add to the rectangle's left and right coordinates
    * @param dy The amount to add to the rectangle's top and bottom coordinates
    */
    void offset(int dx, int dy);


    /**
    * Offset the rectangle to a specific (left, top) position,
    * keeping its width and height the same.
    *
    * @param newLeft   The new "left" coordinate for the rectangle
    * @param newTop    The new "top" coordinate for the rectangle
    */
    void offsetTo(int newLeft, int newTop);


    /**
    * Inset the rectangle by (dx,dy). If dx is positive, then the sides are
    * moved inwards, making the rectangle narrower-> If dx is negative, then the
    * sides are moved outwards, making the rectangle wider-> The same holds true
    * for dy and the top and bottom.
    *
    * @param dx The amount to add(subtract) from the rectangle's left(right)
    * @param dy The amount to add(subtract) from the rectangle's top(bottom)
    */
    void inset(int dx, int dy);


    /**
    * Returns true if (x,y) is inside the rectangle. The left and top are
    * considered to be inside, while the right and bottom are not. This means
    * that for a x,y to be contained: left <= x < right and top <= y < bottom.
    * An empty rectangle never contains any point.
    *
    * @param x The X coordinate of the point being tested for containment
    * @param y The Y coordinate of the point being tested for containment
    * @return true iff (x,y) are contained by the rectangle, where containment
    *              means left <= x < right and top <= y < bottom
    */
    bool contains(int x, int y);


    /**
    * Returns true iff the 4 specified sides of a rectangle are inside or equal
    * to this rectangle. i.e. is this rectangle a superset of the specified
    * rectangle. An empty rectangle never contains another rectangle.
    *
    * @param left The left side of the rectangle being tested for containment
    * @param top The top of the rectangle being tested for containment
    * @param right The right side of the rectangle being tested for containment
    * @param bottom The bottom of the rectangle being tested for containment
    * @return true iff the the 4 specified sides of a rectangle are inside or
    *              equal to this rectangle
    */
    bool contains(int left, int top, int right, int bottom);


    /**
    * Returns true iff the specified rectangle r is inside or equal to this
    * rectangle. An empty rectangle never contains another rectangle.
    *
    * @param r The rectangle being tested for containment.
    * @return true iff the specified rectangle r is inside or equal to this
    *              rectangle
    */
    bool contains(Rect *r);


    /**
    * If the rectangle specified by left,top,right,bottom intersects this
    * rectangle, return true and set this rectangle to that intersection,
    * otherwise return false and do not change this rectangle. No check is
    * performed to see if either rectangle is empty. Note: To just test for
    * intersection, use {@link #intersects(Rect, Rect)}.
    *
    * @param left The left side of the rectangle being intersected with this
    *             rectangle
    * @param top The top of the rectangle being intersected with this rectangle
    * @param right The right side of the rectangle being intersected with this
    *              rectangle.
    * @param bottom The bottom of the rectangle being intersected with this
    *             rectangle.
    * @return true if the specified rectangle and this rectangle intersect
    *              (and this rectangle is then set to that intersection) else
    *              return false and do not change this rectangle.
    */
    bool intersect(int left, int top, int right, int bottom);


    /**
    * If the specified rectangle intersects this rectangle, return true and set
    * this rectangle to that intersection, otherwise return false and do not
    * change this rectangle. No check is performed to see if either rectangle
    * is empty. To just test for intersection, use intersects()
    *
    * @param r The rectangle being intersected with this rectangle.
    * @return true if the specified rectangle and this rectangle intersect
    *              (and this rectangle is then set to that intersection) else
    *              return false and do not change this rectangle.
    */
    bool intersect(Rect *r);


    /**
    * If rectangles a and b intersect, return true and set this rectangle to
    * that intersection, otherwise return false and do not change this
    * rectangle. No check is performed to see if either rectangle is empty.
    * To just test for intersection, use intersects()
    *
    * @param a The first rectangle being intersected with
    * @param b The second rectangle being intersected with
    * @return true iff the two specified rectangles intersect. If they do, set
    *              this rectangle to that intersection. If they do not, return
    *              false and do not change this rectangle.
    */
    bool setIntersect(Rect *a, Rect *b);


    /**
    * Returns true if this rectangle intersects the specified rectangle.
    * In no event is this rectangle modified. No check is performed to see
    * if either rectangle is empty. To record the intersection, use intersect()
    * or setIntersect().
    *
    * @param left The left side of the rectangle being tested for intersection
    * @param top The top of the rectangle being tested for intersection
    * @param right The right side of the rectangle being tested for
    *              intersection
    * @param bottom The bottom of the rectangle being tested for intersection
    * @return true iff the specified rectangle intersects this rectangle. In
    *              no event is this rectangle modified.
    */
    bool intersects(int left, int top, int right, int bottom);

    /**
    * Returns true iff the two specified rectangles intersect. In no event are
    * either of the rectangles modified. To record the intersection,
    * use {@link #intersect(Rect)} or {@link #setIntersect(Rect, Rect)}.
    *
    * @param a The first rectangle being tested for intersection
    * @param b The second rectangle being tested for intersection
    * @return true iff the two specified rectangles intersect. In no event are
    *              either of the rectangles modified.
    */
    bool intersects(Rect *a, Rect *b);


    /**
    * Update this Rect to enclose itself and the specified rectangle. If the
    * specified rectangle is empty, nothing is done. If this rectangle is empty
    * it is set to the specified rectangle.
    *
    * @param left The left edge being unioned with this rectangle
    * @param top The top edge being unioned with this rectangle
    * @param right The right edge being unioned with this rectangle
    * @param bottom The bottom edge being unioned with this rectangle
    */
    void _union(int left, int top, int right, int bottom);


    /**
    * Update this Rect to enclose itself and the specified rectangle. If the
    * specified rectangle is empty, nothing is done. If this rectangle is empty
    * it is set to the specified rectangle.
    *
    * @param r The rectangle being unioned with this rectangle
    */
    void _union(Rect *r);

    /**
    * Update this Rect to enclose itself and the [x,y] coordinate. There is no
    * check to see that this rectangle is non-empty.
    *
    * @param x The x coordinate of the point to add to the rectangle
    * @param y The y coordinate of the point to add to the rectangle
    */
    void _union(int x, int y);


    /**
    * Swap top/bottom or left/right if there are flipped (i.e. left > right
    * and/or top > bottom). This can be called if
    * the edges are computed separately, and may have crossed over each other->
    * If the edges are already correct (i.e. left <= right and top <= bottom)
    * then nothing is done.
    */
    void sort();


    /**
    * Parcelable interface methods
    */
    int describeContents();



    /**
    * Scales up the rect by the given scale.
    * @hide
    */
    void scale(float scale);


    /**
    * Scales up the rect by the given scale, rounding values toward the inside.
    * @hide
    */
    void scaleRoundIn(float scale);

};
#endif



