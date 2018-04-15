#include "RectF.h"




RectF::RectF()
{

}

RectF::RectF(float left, float top, float right, float bottom)
{
    this->left = left;
    this->top = top;
    this->right = right;
    this->bottom = bottom;
}

RectF::RectF(RectF * r)
{
    if (r == NULL)
    {
        left = top = right = bottom = 0.0f;
    }
    else
    {
        left = r->left;
        top = r->top;
        right = r->right;
        bottom = r->bottom;
    }
}

RectF::RectF(Rect *r)
{
    if (r == NULL)
    {
        left = top = right = bottom = 0.0f;
    }
    else
    {
        left = (float)(r->left);
        top = (float)(r->top);
        right = (float)(r->right);
        bottom = (float)(r->bottom);
    }
}

bool RectF::isEmpty()
{
    return left >= right || top >= bottom;
}

float RectF::width()
{
    return right - left;
}

float RectF::height()
{
    return bottom - top;
}

float RectF::centerY()
{
    return (top + bottom) * 0.5f;
}

void RectF::setEmpty()
{
    left = right = top = bottom = 0;
}

void RectF::set(float left, float top, float right, float bottom)
{
    this->left = left;
    this->top = top;
    this->right = right;
    this->bottom = bottom;
}

void RectF::set(RectF src)
{
    this->left = src.left;
    this->top = src.top;
    this->right = src.right;
    this->bottom = src.bottom;
}

void RectF::set(Rect src)
{
    this->left = (float)(src.left);
    this->top = (float)(src.top);
    this->right = (float)(src.right);
    this->bottom = (float)(src.bottom);
}

void RectF::offset(float dx, float dy)
{
    left += dx;
    top += dy;
    right += dx;
    bottom += dy;
}

void RectF::offsetTo(float newLeft, float newTop)
{
    right += newLeft - left;
    bottom += newTop - top;
    left = newLeft;
    top = newTop;
}

void RectF::inset(float dx, float dy)
{
    left += dx;
    top += dy;
    right -= dx;
    bottom -= dy;
}

bool RectF::contains(float x, float y)
{
    return left < right && top < bottom  // check for empty first
        && x >= left && x < right && y >= top && y < bottom;
}

bool RectF::contains(float left, float top, float right, float bottom)
{
    // check for empty first
    return this->left < this->right && this->top < this->bottom
        // now check for containment
        && this->left <= left && this->top <= top
        && this->right >= right && this->bottom >= bottom;
}

bool RectF::contains(RectF * r)
{
    // check for empty first
    return this->left < this->right && this->top < this->bottom
        // now check for containment
        && left <= r->left && top <= r->top
        && right >= r->right && bottom >= r->bottom;
}

bool RectF::intersect(float left, float top, float right, float bottom)
{
    if (this->left < right && left < this->right
        && this->top < bottom && top < this->bottom)
    {
        if (this->left < left)
        {
            this->left = left;
        }
        if (this->top < top)
        {
            this->top = top;
        }
        if (this->right > right)
        {
            this->right = right;
        }
        if (this->bottom > bottom)
        {
            this->bottom = bottom;
        }
        return true;
    }
    return false;
}

bool RectF::intersect(RectF * r)
{
    return intersect(r->left, r->top, r->right, r->bottom);
}

bool RectF::setIntersect(RectF a, RectF b)
{
    if (a.left < b.right && b.left < a.right
        && a.top < b.bottom && b.top < a.bottom)
    {
        left = (std::max)(a.left, b.left);
        top = (std::max)(a.top, b.top);
        right = (std::min)(a.right, b.right);
        bottom = (std::min)(a.bottom, b.bottom);
        return true;
    }
    return false;
}

bool RectF::intersects(float left, float top, float right, float bottom)
{
    return this->left < right && left < this->right
        && this->top < bottom && top < this->bottom;
}

bool RectF::intersects(RectF a, RectF b)
{
    return a.left < b.right && b.left < a.right
        && a.top < b.bottom && b.top < a.bottom;
}

void RectF::_union(float left, float top, float right, float bottom)
{
    if ((left < right) && (top < bottom))
    {
        if ((this->left < this->right) && (this->top < this->bottom))
        {
            if (this->left > left)
                this->left = left;
            if (this->top > top)
                this->top = top;
            if (this->right < right)
                this->right = right;
            if (this->bottom < bottom)
                this->bottom = bottom;
        }
        else
        {
            this->left = left;
            this->top = top;
            this->right = right;
            this->bottom = bottom;
        }
    }
}

void RectF::_union(RectF * r)
{
    _union(r->left, r->top, r->right, r->bottom);
}

void RectF::_union(float x, float y)
{
    if (x < left)
    {
        left = x;
    }
    else if (x > right)
    {
        right = x;
    }
    if (y < top)
    {
        top = y;
    }
    else if (y > bottom)
    {
        bottom = y;
    }
}

void RectF::sort()
{
    if (left > right)
    {
        float temp = left;
        left = right;
        right = temp;
    }
    if (top > bottom)
    {
        float temp = top;
        top = bottom;
        bottom = temp;
    }
}

int RectF::describeContents()
{
    return 0;
}
