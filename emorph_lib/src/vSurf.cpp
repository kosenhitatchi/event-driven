/*
 * Copyright (C) 2010 eMorph Group iCub Facility
 * Authors: Arren Glover
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
 */

#include "iCub/emorph/vSurf.h"

namespace emorph {

vSurf::vSurf(int height, int width)
{
    //resize sets everything to 0
    surface.resize(height);
    for(int j = 0; j < surface.size(); j++) {
        surface[j].resize(width);
    }

    mostRecent = 0;
}

vSurf::~vSurf()
{
    for(int j = 0; j < surface.size(); j++) {
        for(int i = 0; i < surface[j].size(); i++) {
            delete surface[j][i];
        }
    }
}

void vSurf::addEvent(vEvent &event)
{

    AddressEvent * v = event.getAs<AddressEvent>();
    if(!v) return;

    mutex.wait();

    //if(surface[v->getY()][v->getX()])
        delete surface[v->getY()][v->getX()];
    surface[v->getY()][v->getX()] = v->clone();
    mostRecent = surface[v->getY()][v->getX()];

    mutex.post();

}

AddressEvent& vSurf::getMostRecent()
{
    mutex.wait();
    return *mostRecent->clone()->getAs<AddressEvent>();
    mutex.post();
}

vQueue& vSurf::getSpatialWindow()
{
    //this should be more elegent for cameras with more/less resolution
    return getSpatialWindow(0, 128, 0, 128);

}


vQueue& vSurf::getSpatialWindow(int x, int y, int d)
{

    return getSpatialWindow(x - d, x + d, y - d, y + d);
}

vQueue& vSurf::getSpatialWindow(int xl, int xh, int yl, int yh)
{

    //first apply limits
    xl = std::max(xl, 0);
    yl = std::max(yl, 0);
    xh = std::min(xh, (int)surface[0].size());
    yh = std::min(yh, (int)surface.size());


    vQueue *qcopy = new vQueue;

    //critical section
    mutex.wait();

    for(int j = yl; j < yh; j++) {
        for(int i = xl; i < xh; i++) {
            if(surface[j][i])
                qcopy->push_back(surface[j][i]->clone());
        }
    }

    mutex.post();

    qcopy->sort();

    return *qcopy;

}


}