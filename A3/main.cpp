#include<bits/stdc++.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include<vector>
#include "100points.h"
using namespace std;
using namespace std::chrono;
using namespace std;

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int find_orientation(Point p, Point q, Point r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0; // collinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}

// Prints convex hull
void draw_convex_hull(Point points[], int n)
{
    // There must be at least 3 points
    if (n < 3) return;

    // Initialize Output
    vector<Point> convex_hull;

    // Searching for the leftmost point
    int l = 0;
    for (int i = 1; i < n; i++)
        if (points[i].x < points[l].x)
            l = i;

    // From leftmost point, keep moving counterclockwise until reach the start point again.
    // Loop execution will be done in O(h) where h is number of points in result.
    int p = l, q;
    do
    {
        // Add current point to output
        convex_hull.push_back(points[p]);

        // Search for a point 'q' such that orientation(p, q, x) is counterclockwise for all points 'x'.
        //We need to find the last visited point which is most counterclockwise
        q = (p+1)%n;
        for (int i = 0; i < n; i++)
        {
            // If we found that 'i' is more counterclockwise than current q, then
            // update q
            if (find_orientation(points[p], points[i], points[q]) == 2)
                q = i;
        }

        // q will be the most counterclockwise with respect to p
        // Set p as q for next iteration, so that q is added to
        // output 'hull'
        p = q;

    } while (p != l);

    // Print Result
    for (int i = 0; i < convex_hull.size(); i++)
        cout << "(" << convex_hull[i].x << ", "
             << convex_hull[i].y << ")\n";
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