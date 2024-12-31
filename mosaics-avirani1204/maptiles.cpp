/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include <set>

#include "maptiles.h"
using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    //check if input is valid 
    if (theTiles.empty() || theSource.getRows() == 0 || theSource.getColumns() == 0) {
        return NULL;
    }
    //make a new mosaic w same num of rows and columns as the source image
    MosaicCanvas* canvas = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
    //make a vector of point<3> for the kdtree & a map but how??
    vector<Point<3>> tilePts; // average tiles
    // map<Point<3>, TileImage*> pointToTileMap;
    //convert each tileimage's avg color to a point<3>
    size_t idx = 0;

    map<Point<3>, int> pointToTileMap;
    
    while(idx < theTiles.size()) {
        LUVAPixel avgColr = theTiles[idx].getAverageColor();
        // std::cout << theTiles[idx].getAverageColor() << std::endl;
        //making sure pt is valid before proceeding
        Point<3> pt = convertToXYZ(avgColr);
        // if (!pointToTileMap.count(pt)) {
        tilePts.push_back(pt);
            // std::cout << "Point val is: " << pt << std::endl;
        // pointToTileMap[pt] = &theTiles[idx]; //map the point<3>
        pointToTileMap[pt] = idx;
        // }
        idx++; //increment manually
    }

    // std::cout << "\nBuilding KDTree with " << tilePts.size() << " points." << std::endl;

    //check if points are valid to buikld the tree
    // if (tilePts.empty()) {
    //     delete canvas;
    //     return nullptr;
    // }
    //build the kdtree from the vector of points 

    KDTree<3> tree(tilePts);
    // printf("Built tree"); 
    //find nearest tile image for each region in sourceimage
    // printf("\nSource dims: %d, %d", theSource.getRows(), theSource.getColumns());

    set<Point<3>> visited;

    for(int r = 0; r < theSource.getRows(); r++) {
        for (int c = 0; c < theSource.getColumns(); c++) {
            //get color for curr region directly
            LUVAPixel regColr = theSource.getRegionColor(r, c);
            // std::cout << "after (" << r << ", " << c << ") at" << regColr << std::endl;
            //make the region color into a point<3> for the kdtree query
            Point<3> regPt = convertToXYZ(regColr);
            // std::cout << "after (" << r << ", " << c << ") at" << regPt << std::endl;
            // std::cout << "Finding nearest neighbor for region (" << r << ", " << c << ")" << std::endl;
            //find closest tile's point & put correspondint tile in canvas
            Point<3> closestPt = tree.findNearestNeighbor(regPt);
            visited.insert(closestPt);

            // std::cout << "before " << r << ", " << c << ") at" << closestPt << std::endl;
            // make sure nearest point exists in map 
            // if (pointToTileMap.count(closestPt)) {
            canvas->setTile(r, c, &theTiles[pointToTileMap[closestPt]]);
            // std::cout << "after (" << r << ", " << c << ") at" << closestPt << std::endl;
            // std::cout << "\tTile set at (" << r << ", " << c << ")" << std::endl;
            // } else {
            //     std::cout << "Triggered at: " << r << ", "<< c << std::endl;
            //     delete canvas;
            //     return nullptr;
            // }
        }
    }
    printf("Populated canvas");
    return canvas;
}

