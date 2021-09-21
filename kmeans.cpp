// Copyright [2021] <delvecja
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <utility>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include "Kmeans.h"

// It is ok to use the following namespace delarations in C++ source
// files only. They must never be used in header files.
using namespace std;
using namespace std::string_literals;

/**
 * Simple method to read a file and add data to the points array
 * 
 * \param[in] in The file stream for reading the contents of the file.
 * 
 * \param points The vector of Points used to store the data that is 
 * read from the file.
 * 
 * \param n The number of columns to read from the file.
 */
void readFile(ifstream &in, PointList &points, int n) {
    string line;
    // Read every line of file and store n columns from each row in points
    // Ignore Headers
    getline(in, line);
    while (getline(in, line)) {
        // istring stream to read tab seperated columns
        istringstream is(line);
        Point p(n);
        // For n columns add the number to the point
        for (int i = 0; i < n; i++) {
            double num;
            is >> num;
            p[i] = num;
        }
        points.push_back(p);
    }
}

/**
 * Finds the nearest centroid to each point and assigns that point to the
 * cluster.
 * 
 * \param centIdx passed by reference because it is modified to include the
 * index of the cluster for each point.
 * 
 * \param centroids passed by reference but is not modified in this method
 * 
 * \param points A list of points to cluster passed by reference and not
 * modified.
 * 
 * \param k The number of centroids used for clustering
 */
void findClusters(const PointList& centroids, IntVec& centIdx, 
                                        const PointList& points, int k) {
        // Find nearest centroid to each data point
        for (size_t p = 0; p < points.size(); p++) {
            // Find nearest centroid
            double minDist = numeric_limits<double>::max();
            int minIdx = 0;
            // For every centroid check which one is closest to the point
            for (size_t c = 0; c < centroids.size(); c++) {
                double d = distance(points[p], centroids[c]);
                if (d < minDist) {
                    minIdx = static_cast<int>(c);
                    minDist = d;
                }
            }
            centIdx[p] = minIdx; 
        }
}

/**
 * Counts the number of points assigned to a specific cluster.
 * 
 * \param centIdx The centroids for each data point.
 * 
 * \param i The centroid to be counted.
 * 
 * \return The count of points belonging to the centroid.
 */
int countIdx(const IntVec& centIdx, const int i) {
    int count = 0;
    for (int index : centIdx) {
        if (index == i) count++;
    }
    return count;
}

/**
 * Takes all the points in a given cluster and adds them together element-wise 
 * utilizizing a feature of std::valarray.
 * 
 * \param points List of points to be clustered and is passed by const 
 * reference because it is not modified 
 * but there is no need to create a copy everytime this method is called.
 * 
 * \param centIdx The centroid assignment for every point and is passed by 
 * const reference because it is not modified.
 * 
 * \param i The centroid that we are looking at.
 * 
 * \return A point which is the sum of all points belonging to cluster i.
 */
Point sumPoints(const PointList& points, const IntVec& centIdx, const int i) {
    Point sum(points[0].size());
    for (size_t index = 0; index < centIdx.size(); index++) {
        if (centIdx[index] == i) {
            sum += points[index];
        }
    }
    return sum;
}

/**
 * Compares two PointList containers.
 * 
 * \param a First PointList to be compared and is passed by constant reference 
 * because it is unchanged by this method and we do not want to make a copy.
 * 
 * \param b Second PointList to be comparedand is passed by constant reference 
 * because it is unchanged by this method and we do not want to make a copy.
 * 
 * \return True if the two PointLists are the same and false if they are not.
 */
bool compare(const PointList& a, const PointList& b) {
    if (a.size() != b.size()) return false;
    // for each point in Point List compare them
    for (size_t i = 0; i < a.size(); i++) {
        // comparing two valarrays in a and b
        if (!((a[i] == b[i]).min())) {
            return false;
        }
    }
    return true;
}

/**
 * Updates the clusters based on the average of the chosen clusters.
 * 
 * \param points A list of points to be clustered. It is passed by const 
 * reference because it is not modified, but there is no need to create a copy 
 * everytime this method is called.
 * 
 * \param centroids The centroids for each cluster are updated in this method
 * which is why it is passed by reference.
 * 
 * \param centIdx The centroid assignment for each point. It is not updated in
 * this method, but it is passed by const reference because we do not want to 
 * make a copy everytime this method is called.
 * 
 * \param k The number of clusters being used.
 * 
 * \return True if the centroids
 */
bool updateClusters(PointList& centroids, const IntVec& centIdx, 
                                        const PointList& points, int k) {
    PointList prevCentroids = centroids;
    // Assign new clusters based on centroids
    for (int i = 0; i < k; i++) {
        Point sum = sumPoints(points, centIdx, i);
        sum /= countIdx(centIdx, i);
        centroids[i] = sum;
    }
    return compare(prevCentroids, centroids);
}

int main(int argc, char *argv[]) {
    // Open file for reading
    ifstream in(argv[1]);
    // Number of columns to be read from the file
    int n = *argv[2] - '0';
    // Number of centroids to use for clustering
    int k = *argv[3] - '0';
    PointList points;
    readFile(in, points, n);
    // Initialize PointList of centroids 
    PointList centroids;
    // Centroid assignment for each data point
    IntVec centIdx;
    int reps = 100;
    int rep = 0;
    if (k != 0) {
        centIdx.resize(points.size());
        // use helper method to get random centroids
        centroids = getInitCentroid(points, k);
        // for as many iterations as we want in this case we chose 100
        while (rep < reps) {
        // first find k clusters and assign each data point a centroid
        findClusters(centroids, centIdx, points, k);
        // then update the clusters based on the new centroids
        // if there was no change in the update we are done 
        if (updateClusters(centroids, centIdx, points, k)) break;
        rep++;
        }
    }
    // use method to send data
    writeResults(points, centroids, centIdx, cout);
    return 0;
}

// End of source code
