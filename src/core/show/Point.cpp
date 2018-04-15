/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include "Point.h"
#include "common.h"




Point::Point(int x, int y)
{
    this->x = x;
    this->y = y;
}

Point::Point(Point *src)
{
    this->x = src->x;
    this->y = src->y;
}

Point::Point()
{
    x = 0;
    y = 0;
}

/**
 * Set the point's x and y coordinates
 */
void Point::set(int x, int y)
{
    this->x = x;
    this->y = y;
}

/**
 * Negate the point's coordinates
 */
void Point::negate()
{
    x = -x;
    y = -y;
}

/**
 * Offset the point's coordinates by dx, dy
 */
void Point::offset(int dx, int dy)
{
    x += dx;
    y += dy;
}

/**
 * Returns true if the point's coordinates equal (x,y)
 */
bool Point::equals(int x, int y)
{
    return this->x == x && this->y == y;
}


//bool Point::equals(Object o)
//{
//    if (this == o) return true;
//    if (o == NULL || getClass() != o.getClass()) return false;
//
//    Point point = (Point) o;
//
//    if (x != point.x) return false;
//    if (y != point.y) return false;
//
//    return true;
//}


int Point::hashCode()
{
    int result = x;
    result = 31 * result + y;
    return result;
}


std::string Point::toString()
{
    char tmp[32] = {0};
    snprintf(tmp, 31, "%s%d%s%d%s", "Point(", x, ", ", y, ")");
    return std::string(tmp);
}

