// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/* 
 * Copyright (C) 2011 Department of Robotics Brain and Cognitive Sciences -
 * Istituto Italiano di Tecnologia
 * Author: Ugo Pattacini, modified by Arren Glover(10/14)
 * email:  ugo.pattacini@iit.it
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

#ifndef __EMORPH_ECODEC_H__
#define __EMORPH_ECODEC_H__

#include <yarp/os/all.h>
#include <string>
#include <deque>

namespace emorph {

//forward declaration
class vEvent;

vEvent * createEvent(const std::string type);

/**************************************************************************/
class vQueue : public std::deque<vEvent*>
{
private:
    // copy-constructor and overaloaded "=" operator are made
    // private on purpose to avoid the problem of dangling
    // pointers since the destructor frees up the memory
    // allocated for events
    vQueue(const vQueue&);
    vQueue &operator=(const vQueue&);
    static bool temporalSort(const vEvent *e1, const vEvent *e2);

protected:
    bool owner;  

public:
    vQueue()                   { owner=true;        }
    vQueue(const bool _owner)  { owner=_owner;      }
    void setOwner(const bool owner) { this->owner=owner; }
    bool getOwner()                 { return owner;      }
    ~vQueue();

    void sort();


};


/**************************************************************************/
class vEvent
{
protected:

    bool valid; //this is just in here so i don't have to change other
                //derived classes just yet. delete it when i can
    std::string type;
    int stamp;

    virtual int nBytesCoded() const { return 1;             }

public:
    vEvent() : type("TS"), stamp(0) { }
    std::string getType() const     { return type;          }


    void setStamp(const int stamp)  { this->stamp=stamp;    }
    int getStamp() const            { return stamp;         }

    virtual int getChannel() const  { return -1;            }



    virtual vEvent &operator=(const vEvent &event);
    virtual bool operator==(const vEvent &event);
    virtual bool operator<(const vEvent &event) const
                 {return this->stamp < event.stamp; }
    virtual bool operator>(const vEvent &event) const
                 {return this->stamp > event.stamp; }

    virtual yarp::os::Bottle   encode() const;
    virtual vEvent * decode(const yarp::os::Bottle &packet, int &pos);
    virtual yarp::os::Property getContent() const;

    template<class T> T* getAs() {
        return dynamic_cast<T*>(this);
    }

};


/**************************************************************************/
class AddressEvent : public vEvent
{
protected:
    int channel;
    int polarity;
    int x;
    int y;

    virtual int nBytesCoded() const         { return 1;                 }

public:
    AddressEvent();
    AddressEvent(const AddressEvent &event);
    AddressEvent(const vEvent &event);
    //AddressEvent(const yarp::os::Bottle &packets, const int pos=0);

    vEvent &operator=(const vEvent &event);
    bool operator==(const AddressEvent &event);

    int getChannel() const                  { return channel;           }
    int getPolarity() const                 { return polarity;          }
    int getX() const                        { return x;                 }
    int getY() const                        { return y;                 }

    void setChannel(const int channel)      { this->channel=channel;    }
    void setPolarity(const int polarity)    { this->polarity=polarity;  }
    void setX(const int x)                  { this->x=x;                }
    void setY(const int y)                  { this->y=y;                }


    bool operator==(const vEvent &event) {
        return operator==(dynamic_cast<const AddressEvent&>(event)); }
    yarp::os::Bottle   encode() const ;
    yarp::os::Property getContent() const;
    virtual vEvent * decode(const yarp::os::Bottle &packet, int &pos);
};





/**************************************************************************/
class AddressEventFeatures : public AddressEvent
{
protected:
    int orientation;
    int xFlow;
    int yFlow;

public:
    AddressEventFeatures();
    AddressEventFeatures(const AddressEventFeatures &event);
    AddressEventFeatures(const yarp::os::Bottle &packets, const int pos=0);

    AddressEventFeatures &operator=(const AddressEventFeatures &event);
    bool operator==(const AddressEventFeatures &event);

    int getOrientation() const                 { return orientation;            }
    int getXFlow() const                       { return xFlow;                  }
    int getYFlow() const                       { return yFlow;                  }

    void setOrientation(const int orientation) { this->orientation=orientation; }
    void setXFlow(const int xFlow)             { this->xFlow=xFlow;             }
    void setYFlow(const int yFlow)             { this->yFlow=yFlow;             }

    int getLength() const { return 2; }
    bool operator==(const vEvent &event) { return operator==(dynamic_cast<const AddressEventFeatures&>(event)); }
    yarp::os::Bottle   encode() const;
    yarp::os::Property getContent() const;
};


/**************************************************************************/
class AddressEvent3D : public vEvent
{
protected:
    int disparity;
    int polarity;
    int x;
    int y;

public:
    AddressEvent3D();    
    AddressEvent3D(const AddressEvent3D &event);
    AddressEvent3D(const yarp::os::Bottle &packets, const int pos=0);

    AddressEvent3D &operator=(const AddressEvent3D &event);
    bool operator==(const AddressEvent3D &event);

    int getDisparity() const               { return disparity;          }
    int getPolarity() const                { return polarity;           }
    int getX() const                       { return x;                  }
    int getY() const                       { return y;                  }

    void setDisparity(const int disparity) { this->disparity=disparity; }
    void setPolarity(const int polarity)   { this->polarity=polarity;   }
    void setX(const int x)                 { this->x=x;                 }
    void setY(const int y)                 { this->y=y;                 }

    int getLength() const { return 1; }
    bool operator==(const vEvent &event) { return operator==(dynamic_cast<const AddressEvent3D&>(event)); }
    yarp::os::Bottle   encode() const;
    yarp::os::Property getContent() const;
};


/**************************************************************************/
class AddressEvent3DFeatures : public AddressEvent3D
{
protected:
    int orientation;
    int xFlow;
    int yFlow;

public:
    AddressEvent3DFeatures();
    AddressEvent3DFeatures(const AddressEvent3DFeatures &event);
    AddressEvent3DFeatures(const yarp::os::Bottle &packets, const int pos=0);

    AddressEvent3DFeatures &operator=(const AddressEvent3DFeatures &event);
    bool operator==(const AddressEvent3DFeatures &event);

    int getOrientation() const                 { return orientation;            }
    int getXFlow() const                       { return xFlow;                  }
    int getYFlow() const                       { return yFlow;                  }

    void setOrientation(const int orientation) { this->orientation=orientation; }
    void setXFlow(const int xFlow)             { this->xFlow=xFlow;             }
    void setYFlow(const int yFlow)             { this->yFlow=yFlow;             }

    int getLength() const { return 2; }
    bool operator==(const vEvent &event) { return operator==(dynamic_cast<const AddressEvent3DFeatures&>(event)); }
    yarp::os::Bottle   encode() const;
    yarp::os::Property getContent() const;
};


/**************************************************************************/
class ClusterEvent : public vEvent
{
protected:
    int id;
    int channel;
    int xCog;
    int yCog;

public:
    ClusterEvent();
    ClusterEvent(const ClusterEvent &event);
    ClusterEvent(const vEvent &event);
    //ClusterEvent(const yarp::os::Bottle &packets, const int pos=0);

    ClusterEvent &operator=(const ClusterEvent &event);
    bool operator==(const ClusterEvent &event);

    int getChannel() const             { return channel;        }
    int getId()      const             { return id;             }
    int getXCog()    const             { return xCog;           }
    int getYCog()    const             { return yCog;           }

    void setChannel(const int channel) { this->channel = channel; }
    void setId(const int id)           { this->id = id;     }
    void setXCog(const int xCog)       { this->xCog = xCog;       }
    void setYCog(const int yCog)       { this->yCog = yCog;       }

    bool operator==(const vEvent &event) { return operator==(dynamic_cast<const ClusterEvent&>(event)); }
    yarp::os::Bottle   encode() const;
    yarp::os::Property getContent() const;
    virtual vEvent * decode(const yarp::os::Bottle &packet, int &pos);
};


/**************************************************************************/
class ClusterEventFeatures1 : public ClusterEvent
{
public:
    ClusterEventFeatures1();
    ClusterEventFeatures1(const yarp::os::Bottle &packets, const int pos=0);

    int getLength() const { return 2; }
    yarp::os::Bottle encode() const;
};


/**************************************************************************/
class ClusterEventFeatures2 : public ClusterEventFeatures1
{
protected:
    int shapeType;
    int xSize;
    int ySize;
    int numAE;

public:
    ClusterEventFeatures2();
    ClusterEventFeatures2(const ClusterEventFeatures2 &event);
    ClusterEventFeatures2(const yarp::os::Bottle &packets, const int pos=0);

    ClusterEventFeatures2 &operator=(const ClusterEventFeatures2 &event);
    bool operator==(const ClusterEventFeatures2 &event);

    int getShapeType() const               { return shapeType;          }
    int getXSize() const                   { return xSize;              }
    int getYSize() const                   { return ySize;              }
    int getNumAE() const                   { return numAE;              }

    void setShapeType(const int shapeType) { this->shapeType=shapeType; }
    void setXSize(const int xSize)         { this->xSize=xSize;         }
    void setYSize(const int ySize)         { this->ySize=ySize;         }
    void setNumAE(const int numAE)         { this->numAE=numAE;         }

    int getLength() const { return 3; }
    bool operator==(const vEvent &event) { return operator==(dynamic_cast<const ClusterEventFeatures2&>(event)); }
    yarp::os::Bottle   encode() const;
    yarp::os::Property getContent() const;
};


/**************************************************************************/
class ClusterEventFeatures3 : public ClusterEventFeatures2
{
protected:
    int shapeProb;
    int xVel;
    int yVel;
    int numAE;

public:
    ClusterEventFeatures3();
    ClusterEventFeatures3(const ClusterEventFeatures3 &event);
    ClusterEventFeatures3(const yarp::os::Bottle &packets, const int pos=0);

    ClusterEventFeatures3 &operator=(const ClusterEventFeatures3 &event);
    bool operator==(const ClusterEventFeatures3 &event);

    int getShapeProb() const               { return shapeProb;          }
    int getXVel()      const               { return xVel;               }
    int getYVel()      const               { return yVel;               }
    int getNumAE()     const               { return numAE;              }

    void setShapeProb(const int shapeProb) { this->shapeProb=shapeProb; }
    void setXVel(const int xVel)           { this->xVel=xVel;           }
    void setYVel(const int yVel)           { this->yVel=yVel;           }
    void setNumAE(const int numAE)         { this->numAE=numAE;         }

    int getLength() const { return 4; }
    bool operator==(const vEvent &event) { return operator==(dynamic_cast<const ClusterEventFeatures3&>(event)); }
    yarp::os::Bottle   encode() const;
    yarp::os::Property getContent() const;
};


/**************************************************************************/
class ClusterEvent3D : public vEvent
{
protected:
    int channel;
    int disparity;
    int xCog;
    int yCog;

public:
    ClusterEvent3D();
    ClusterEvent3D(const ClusterEvent3D &event);
    ClusterEvent3D(const yarp::os::Bottle &packets, const int pos=0);

    ClusterEvent3D &operator=(const ClusterEvent3D &event);
    bool operator==(const ClusterEvent3D &event);

    int getChannel() const                 { return channel;            }
    int getDisparity() const               { return disparity;          }
    int getXCog() const                    { return xCog;               }
    int getYCog() const                    { return yCog;               }

    void setChannel(const int channel)     { this->channel=channel;     }
    void setDisparity(const int disparity) { this->disparity=disparity; }
    void setXCog(const int xCog)           { this->xCog=xCog;           }
    void setYCog(const int yCog)           { this->yCog=yCog;           }

    int getLength() const { return 1; }
    bool operator==(const vEvent &event) { return operator==(dynamic_cast<const ClusterEvent3D&>(event)); }
    yarp::os::Bottle   encode() const;
    yarp::os::Property getContent() const;
};


/**************************************************************************/
class ClusterEvent3DFeatures1 : public ClusterEvent3D
{
protected:
    int numAE;

public:
    ClusterEvent3DFeatures1();
    ClusterEvent3DFeatures1(const ClusterEvent3DFeatures1 &event);
    ClusterEvent3DFeatures1(const yarp::os::Bottle &packets, const int pos=0);

    ClusterEvent3DFeatures1 &operator=(const ClusterEvent3DFeatures1 &event);
    bool operator==(const ClusterEvent3DFeatures1 &event);

    int  getNumAE() const          { return numAE;      }
    void setNumAE(const int numAE) { this->numAE=numAE; }

    int getLength() const { return 2; }
    bool operator==(const vEvent &event) { return operator==(dynamic_cast<const ClusterEvent3DFeatures1&>(event)); }
    yarp::os::Bottle   encode() const;
    yarp::os::Property getContent() const;
};


/**************************************************************************/
class ClusterEvent3DFeatures2 : public ClusterEvent3DFeatures1
{
protected:
    int shapeType;
    int xSize;
    int ySize;

public:
    ClusterEvent3DFeatures2();
    ClusterEvent3DFeatures2(const ClusterEvent3DFeatures2 &event);
    ClusterEvent3DFeatures2(const yarp::os::Bottle &packets, const int pos=0);

    ClusterEvent3DFeatures2 &operator=(const ClusterEvent3DFeatures2 &event);
    bool operator==(const ClusterEvent3DFeatures2 &event);

    int getShapeType() const               { return shapeType;          }
    int getXSize() const                   { return xSize;              }
    int getYSize() const                   { return ySize;              }

    void setShapeType(const int shapeType) { this->shapeType=shapeType; }
    void setXSize(const int xSize)         { this->xSize=xSize;         }
    void setYSize(const int ySize)         { this->ySize=ySize;         }

    int getLength() const { return 3; }
    bool operator==(const vEvent &event) { return operator==(dynamic_cast<const ClusterEvent3DFeatures2&>(event)); }
    yarp::os::Bottle   encode() const;
    yarp::os::Property getContent() const;
};


/**************************************************************************/
class ClusterEvent3DFeatures3 : public ClusterEvent3DFeatures2
{
protected:
    int shapeProb;
    int xVel;
    int yVel;

public:
    ClusterEvent3DFeatures3();
    ClusterEvent3DFeatures3(const ClusterEvent3DFeatures3 &event);
    ClusterEvent3DFeatures3(const yarp::os::Bottle &packets, const int pos=0);

    ClusterEvent3DFeatures3 &operator=(const ClusterEvent3DFeatures3 &event);
    bool operator==(const ClusterEvent3DFeatures3 &event);

    int getShapeProb() const               { return shapeProb;          }
    int getXVel() const                    { return xVel;               }
    int getYVel() const                    { return yVel;               }

    void setShapeProb(const int shapeProb) { this->shapeProb=shapeProb; }
    void setXVel(const int xVel)           { this->xVel=xVel;           }
    void setYVel(const int yVel)           { this->yVel=yVel;           }

    int getLength() const { return 4; }
    bool operator==(const vEvent &event) { return operator==(dynamic_cast<const ClusterEvent3DFeatures3&>(event)); }
    yarp::os::Bottle   encode() const;
    yarp::os::Property getContent() const;
};


 /**************************************************************************/
class HoughEvent : public vEvent
{
 protected:
  int channel;             // reference to the camera                  (bit0)
  int xCoc;                // x position of the center of the circle   (bit7-1)       
  int yCoc;                // y position of the center of the circle   (bit15-8)
  int radius;              // reference to the radius of the circle    (bit25-16)

public:
    HoughEvent();
    HoughEvent(const HoughEvent &event);
    HoughEvent(const yarp::os::Bottle &packets, const int pos=0);

    HoughEvent &operator=(const HoughEvent &event);
    bool operator==(const HoughEvent &event);

    /* returns the camera that produced the Hough event */
    int getChannel() const                 { return channel;            }

    /* returns the radius of the circle */
    int getRadius() const                  { return radius;             }

    /* returns the x coordinate of the center of circle*/
    int getXCoc() const                    { return xCoc;               }

    /* returns the y coordinate of the center of circle*/
    int getYCoc() const                    { return yCoc;               }

    /*set channel information*/
    void setChannel(const int channel)     { this->channel = channel;    }

    /*set the radius information*/
    void setRadius(const int radius)       { this->radius = radius;      }
    
    /*set the x coordinate of the center of circle */
    void setXCoc(const int xCoc)           { this->xCoc = xCoc;          }

    /*set the y coordinate of the center of circle */
    void setYCoc(const int yCoc)           { this->yCoc = yCoc;          }

    int getLength() const { return 1; }
    bool operator==(const vEvent &event) { return operator==(dynamic_cast<const HoughEvent&>(event)); }
    yarp::os::Bottle   encode() const;
    yarp::os::Property getContent() const;
};

/**************************************************************************/
class ClusterEventGauss : public ClusterEvent
{
protected:
    int numAE;
    int xSigma2;
    int ySigma2;
    int xySigma;

public:
    ClusterEventGauss();
    ClusterEventGauss(const ClusterEventGauss &event);
    ClusterEventGauss(const yarp::os::Bottle &packets, const int pos=0);

    ClusterEventGauss &operator=(const ClusterEventGauss &event);
    bool operator==(const ClusterEventGauss &event);

    int getNumAE()      const               { return numAE;            }
    int getXSigma2()    const               { return xSigma2;          }
    int getYSigma2()    const               { return ySigma2;          }
    int getXYSigma()    const               { return xySigma;          }

    void setNumAE(const int numAE)          { this->numAE=numAE;       }
    void setXSigma2(const int xSigma2)      { this->xSigma2=xSigma2;   }
    void setYSigma2(const int ySigma2)      { this->ySigma2=ySigma2;   }
    void setXYSigma(const int xySigma)      { this->xySigma=xySigma;   }

    int getLength() const { return 3; }
    bool operator==(const vEvent &event) { return operator==(dynamic_cast<const ClusterEventGauss&>(event)); }
    yarp::os::Bottle   encode() const;
    yarp::os::Property getContent() const;
};

}

#endif

