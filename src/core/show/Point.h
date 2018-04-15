#ifndef _Point_h_
#define _Point_h_
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



/**
 * Point holds two integer coordinates
 */
class Point
{
public:
    int x;
    int y;

    Point();

    Point(int x, int y);

    Point(Point *src);

    /**
     * Set the point's x and y coordinates
     */
    void set(int x, int y);

    /**
     * Negate the point's coordinates
     */
    void negate();

    /**
     * Offset the point's coordinates by dx, dy
     */
    void offset(int dx, int dy);

    /**
     * Returns true if the point's coordinates equal (x,y)
     */
    bool equals(int x, int y);


    //bool equals(Object o);


    int hashCode();

    std::string toString();

};

#endif