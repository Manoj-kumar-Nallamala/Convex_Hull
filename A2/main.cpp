//IMPORTING REQUIRED LIBRARY FILES
#include<bits/stdc++.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include<vector>
#include "100points.h"
using namespace std;
using namespace std::chrono;
using namespace std;


//Need a global point variable for pointing to bottom most point in the hull
Point p0;

// A utility to get the immediate next element from the current stack
Point find_next_to_top(stack<Point> &S)
{
    Point p = S.top();
    S.pop();
    Point res = S.top();
    S.push(p);
    return res;
}

// A utility function for swapping points
void swap(Point &p1, Point &p2)
{
    Point flag = p1;
    p1 = p2;
    p2 = flag;
}

// A utility  return Euclidean distance between the points
int square_distance(Point x1, Point x2)
{
    return (x1.x - x2.x)*(x1.x - x2.x) +
           (x1.y - x2.y)*(x1.y - x2.y);
}

// Function to find polar angle between th points (p, q, r).
// The function returns following values
// 0 --> p, q and r are in a line
// 1 --> Clockwise
// 2 --> Counterclockwise indicating that it is making left turn
int find_orientation(Point p, Point q, Point r)
{
    int orient_val = (q.y - p.y) * (r.x - q.x) -
                     (q.x - p.x) * (r.y - q.y);

    if (orient_val == 0) return 0;  // collinear
    if (orient_val > 0)
        return 1; //   clock wise
    else
        return 2; //  counterclock wise
}

//Sort an array of points with respect to the first point.
//If all the points are on same sine we will sort them based upon distance between the points
int sort_by_compare(const void *vp1, const void *vp2)
{
    Point *p1 = (Point *)vp1;
    Point *p2 = (Point *)vp2;

    // Find orientation
    int o = find_orientation(p0, *p1, *p2);
    if (o == 0)
        return (square_distance(p0, *
                p2) >= square_distance(p0, *p1))? -1 : 1;

    if (o == 2)
        return -1;
    else
        return 1;
}

// Prints convex hull of a set of n points.
void draw_convex_hull(Point points[], int n)
{
    // Find the bottommost point
    int min_y = points[0].y, min = 0;
    for (int i = 1; i < n; i++)
    {
        int y = points[i].y;

        // Pick the bottom-most or choose the left most point in case of tie
        if ((y < min_y) || (min_y == y &&
                            points[i].x < points[min].x))
            min_y = points[i].y, min = i;
    }

    // We have place the bottom-most point at first position
    swap(points[0], points[min]);

    // Sort n-1 points with respect to the first point.
    // A point p1 comes before p2 in sorted output if p2 has larger polar angle (in counterclockwise direction) than p1
    p0 = points[0];
    qsort(&points[1], n-1, sizeof(Point), sort_by_compare);

    // If two or more points make same angle polar with p0, Remove all but the one that is farthest from p0.
    //Because if we consider the farthest point all the remaining points will automatically will be inscribes in the hull by the edge
    // connecting p0 and farthest point.
    //In above sorting, our criteria was to keep the farthest point at the end when more than one points have same angle.
    int sizeof_modify = 1; // Initialize size of modified array
    for (int i=1; i<n; i++)
    {
        // Keep removing i while angle of i and i+1 is same with respect to p0
        while (i < n-1 && find_orientation(p0, points[i],
                                           points[i+1]) == 0)
            i++;


        points[sizeof_modify] = points[i];
        sizeof_modify++;  // Update size of modified array
    }

    // If modified array of points has less than 3 points, convex hull is not possible
    if (sizeof_modify < 3) return;

    // Create an empty stack and push first three points to it.
    stack<Point> S;
    S.push(points[0]);
    S.push(points[1]);
    S.push(points[2]);

    // Process remaining n-3 points
    for (int i = 3; i < sizeof_modify; i++)
    {
        // Keep removing top while the angle formed by points next-to-top, top, and points[i] makes a non-left turn
        //     /
        //     /
        //     /      ==> when encountered polar angle at mid point while comparing three points leads to the formation of non left
        //      \         as shown like in left side shape we will backtrack popping out the point from already added point in the stack
        //       \        we will continue to pop till we encounter a left turn
        //        \
        //          \

        while (S.size()>1 && find_orientation(find_next_to_top(S), S.top(), points[i]) != 2)
            S.pop();
        S.push(points[i]);
    }

    //Printing Stack content for showing the final convex hull formed
    while (!S.empty())
    {
        Point p = S.top();
        cout << "(" << p.x << ", " << p.y <<")" << endl;
        S.pop();
    }
}

// Driver program to test above functions
int main()
{
    int n=100;
    Point *points=points100;
    auto start = high_resolution_clock::now();
    draw_convex_hull(points, n);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout<<endl;
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;

    return 0;
}
