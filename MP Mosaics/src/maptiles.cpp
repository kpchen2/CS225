/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    MosaicCanvas* mosaic = new MosaicCanvas(theSource.getRows(), theSource.getColumns());

    vector<Point<3>> points;
    for (unsigned i = 0; i < theTiles.size(); i++) {
        points.push_back(convertToXYZ(theTiles.at(i).getAverageColor()));
    }

    KDTree<3> tree(points);
    for (int i = 0; i < theSource.getRows(); i++) {
        for (int j = 0; j < theSource.getColumns(); j++) {
            Point<3> query = convertToXYZ(theSource.getRegionColor(i,j));
            Point<3> p = tree.findNearestNeighbor(query);

            unsigned index;
            TileImage* tile = nullptr;
            for (index = 0; index < points.size(); index++) {
                if (points.at(index) == p) {
                    tile = &(theTiles.at(index));
                }
            }
            mosaic->setTile(i, j, tile);
        }
    }
    return mosaic;
}

