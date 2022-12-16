//IMPORTING REQUIRED LIBRARY FILES
#include<bits/stdc++.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include<vector>
using namespace std;
using namespace std::chrono;
using namespace std;

//Include the header for data points change this file based on the in put file you want to use
#include "100points.h"

//storing the center point of polygon
pair<int, int> mid_point;
//calculating the quadrant of a particular point
//r.first gives the first coordinate of the point
//r.second gives the second coordinate of the point
int calculate_quadrant(pair<int, int> r){
    if (r.first >= 0 && r.second >= 0)
        return 1;
    if (r.first <= 0 && r.second >= 0)
        return 2;
    if (r.first <= 0 && r.second <= 0)
        return 3;
    return 4;
}

//if line is touching the polygon
//based on this return value we will decide how to move the edge

int find_tangent(pair<int, int> a, pair<int, int> b,
                 pair<int, int> c){
    int res = (b.second-a.second)*(c.first-b.first) -
              (c.second-b.second)*(b.first-a.first);
    if (res == 0)
        return 0;
    if (res > 0)
        return 1;
    return -1;
}

//comparing functions for sorting
// this will be passed as an argument to inbuilt sort which will return boolean.
// True if second point is counter clockwise & false vise versa.
// We use mid_point a global variable which is already calculated while traversing through the points.
bool compare(pair<int, int> r1, pair<int, int> s1){
    pair<int, int> r = make_pair(r1.first - mid_point.first,
                                 r1.second - mid_point.second);
    pair<int, int> s = make_pair(s1.first - mid_point.first,
                                 s1.second - mid_point.second);
    int one = calculate_quadrant(r);
    int two = calculate_quadrant(s);
    if (one != two)
        return (one < two);
    return (r.second*s.first < s.second*r.first);
}

//finding the upper tangent for both polygons
vector<pair<int, int>> merging_hull(vector<pair<int, int> > a,
                                    vector<pair<int, int> > b){
    int n1 = a.size(), n2 = b.size();
    int ia = 0, ib = 0;
    for (int i=1; i<n1; i++)
        if (a[i].first > a[ia].first)
            ia = i;
    //calculating leftmost point of b
    for (int i=1; i<n2; i++)
        if (b[i].first < b[ib].first)
            ib=i;
    int inda = ia, indb = ib;
    bool done = 0;
    while (!done){
        done = 1;
        //based on the function call we will decide if the line passes through the polygon if it passes
        // through polygon "a" and we will will proceed to next point on polygon which in anticlockwise order.
        // Similarly we check for polygon 'b' we move clock wise.
        while (find_tangent(b[indb], a[inda], a[(inda+1)%n1])
               >=0)
            inda = (inda + 1) % n1;
        while (find_tangent(a[inda], b[indb], b[(n2+indb-1)%n2])
               <=0){
            indb = (n2+indb-1)%n2;
            done = 0;
        }
    }
    // we get upper tangent corrdinates
    int uppera = inda, upperb = indb;
    inda = ia, indb=ib;
    done = 0;
    int g = 0;
    //calculating the lower tangent
    while (!done){
        done = 1;
        //for lower tangent we move vise versa, clock wise if it passes through polygon "a" and
        // counter clock wise for polygon "b"
        while (find_tangent(a[inda], b[indb], b[(indb+1)%n2])>=0)
            indb=(indb+1)%n2;
        while (find_tangent(b[indb], a[inda],
                            a[(n1+inda-1)%n1])<=0){
            inda=(n1+inda-1)%n1;
            done=0;
        }
    }
    //lower tangent coordinates
    int lowera = inda, lowerb = indb;
    vector<pair<int, int>> result;
    //merging two polygons to get convex hull
    //for forming final hull after calculating tangents we will start by including first point of upper tangent.
    // Then we will traverse along the hull and add all points to result till we encounter first point of lower tangent.
    // Then we will include two points of lower tangent and proceed to add other points in second hull
    // in anti clock wise till we encounter second point of the upper tangent.
    int ind = uppera;
    result.push_back(a[uppera]);
    while (ind != lowera){
        ind = (ind+1)%n1;
        result.push_back(a[ind]);
    }
    ind = lowerb;
    result.push_back(b[lowerb]);
    while (ind != upperb){
        ind = (ind+1)%n2;
        result.push_back(b[ind]);
    }
    return result;
}


//Bruteforce algorithm to find the convex hull
// We will consider two points from the set of points and check if line formed by this two points will
// be an edge for the final hull. For Checking if the edge is an edge is part hull we traverse through all points and
// verify if all the points are on the same side of the line formed.
// If so we will add this edge to the hull
// finally we will sort the result while returning

vector<pair<int, int>> brute_force_for_convex_hull(vector<pair<int, int>> a){
    set<pair<int, int> >s;
    for (int i=0; i<a.size(); i++){
        for (int j=i+1; j<a.size(); j++){
            int x1 = a[i].first, x2 = a[j].first;
            int y1 = a[i].second, y2 = a[j].second;
            int a1 = y1-y2;
            int b1 = x2-x1;
            int c1 = x1*y2-y1*x2;
            int loc = 0, neg = 0;
            //Checking if point lies on same side of edge considered
            for (int k=0; k<a.size(); k++){
                if (a1*a[k].first+b1*a[k].second+c1 <= 0)
                    neg++;
                if (a1*a[k].first+b1*a[k].second+c1 >= 0)
                    loc++;
            }
            //adding the edge if all the points fall on same side edge
            if (loc == a.size() || neg == a.size()){
                s.insert(a[i]);
                s.insert(a[j]);
            }
        }
    }
    vector<pair<int, int>>result;
    for (auto e:s)
        result.push_back(e);
    //moving through anti clockwise direction
    mid_point = {0, 0};
    int n = result.size();
    // To find a point inside the convex polygon(centroid),
    // we sum up all the coordinates and then divide  by
    // n(number of points). But this would be a floating-point
    // value. So to get rid of this we multiply points
    // initially with n and then find the centre and
    // then divided it by n again.
    // elements of b)
    for (int i=0; i<n; i++){
        mid_point.first += result[i].first;
        mid_point.second += result[i].second;
        result[i].first *= n;
        result[i].second *= n;
    }
    sort(result.begin(), result.end(), compare);
    for (int i=0; i<n; i++)
        result[i] = make_pair(result[i].first/n, result[i].second/n);
    return result;
}


//returning the value of convex hull
vector<pair<int, int>> drawing_convex_hull(vector<pair<int, int>> a){
    if (a.size() <= 5)
        return brute_force_for_convex_hull(a);
    // left contains the left half points
    // right contains the right half points
    vector<pair<int, int>>left, right;
    for (int i=0; i<a.size()/2; i++)
        left.push_back(a[i]);
    for (int i=a.size()/2; i<a.size(); i++)
        right.push_back(a[i]);
    vector<pair<int, int>>left_hull = drawing_convex_hull(left);
    vector<pair<int, int>>right_hull = drawing_convex_hull(right);
    //merging the convex hulls
    return merging_hull(left_hull, right_hull);
}


int main()
{
    int n=100;
    Point *points=points100;
    vector<pair<int, int> > a;


    for(int i=0;i<n;i++)
    {
        a.push_back(make_pair(points[i].x,points[i].y));
    }
    auto start = high_resolution_clock::now();
    // sorting the set of points according to the x-coordinate
    sort(a.begin(), a.end());
    vector<pair<int, int> >res = drawing_convex_hull(a);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout<<endl;
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;
    cout << "convex hull:\n";
    for (auto e:res)
        cout << e.first << " "
             << e.second << endl;

    return 0;
}