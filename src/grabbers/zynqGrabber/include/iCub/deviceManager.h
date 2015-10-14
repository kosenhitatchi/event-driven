//
//  deviceManager.h
//  eMorph
//
//  Created by Chiara Bartolozzi on 30/07/15.
//
//

#ifndef __eMorph__deviceManager__
#define __eMorph__deviceManager__

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <yarp/os/all.h>
#include <fstream>
#include <iCub/vsCtrl.h>

#define DEBUG
/**
 * @brief The deviceManager class opens the device and implements a constant
 * read
 */
class deviceManager : yarp::os::Thread {

private:

#ifdef DEBUG
    std::ofstream writeDump;
    std::ofstream readDump;
#endif


    //read thread related functions and variables
    unsigned int readCount;
    unsigned int maxBufferSize;

    std::vector<char> *readBuffer;
    std::vector<char> *accessBuffer;
    std::vector<char> buffer1;
    std::vector<char> buffer2;

    yarp::os::Semaphore safety;
    virtual void run();

protected:
    
    bool bufferedRead;      // read thread related variable
    
    //device member variables
    int devDesc;            // file descriptor for device
    std::string deviceName; // name of the device


public:

    virtual bool openDevice();
    virtual void closeDevice();

    int writeDevice(std::vector<unsigned int> &deviceData);
    const std::vector<char>& readDevice(int &nBytesRead);
    
    deviceManager(bool bufferedRead = false, unsigned int maxBufferSize = MAX_BUF_SIZE);

    const unsigned int getBufferSize() { return maxBufferSize; }
    const std::string getDevType() {return deviceName; }
    
};

/* -------------------------------------------------------------------
 
    inherited class to manage vsctrl_i2c devices 
 
  ------------------------------------------------------------------- */

class vsctrlDevManager : public deviceManager {
    
private:
    
    vsctrl_ioctl_arg_t iocVsctrlArg;     // vsctrl
    fpgaStatus_t fpgaStat;
    
    
    std::string chipName;
    std::string channel;
    std::map<std::string, unsigned int> mBiases;
    //unsigned int header;
    std::vector<std::string> biasNames; // ordered
    std::vector<int> biasValues;
    
    unsigned int getBias(std::string biasName);
    void printBiases();
    std::vector<unsigned int> prepareBiases();


public:
    
    virtual bool openDevice();
    virtual void closeDevice();

    vsctrlDevManager(std::string channel, std::string chip);// : deviceManager(false,1666), fpgaStat(false,false,false,false,false) {}
    
    // ---- ioctl for i2c device ---- //
    int chipReset();
    int chipPowerDown();
    int chipPowerUp();
    
    int getFpgaStatus();
    int clearFpgaStatus(std::string clr);
    int getFpgaRel();
    int getFpgaInfo();
    int writeAerTimings(uint8_t ack_rel, uint8_t sample, uint8_t ack_set);
    
    int writeBgTimings(uint8_t prescaler, uint8_t hold, uint8_t ck_active, uint8_t latch_setup, uint8_t latch_active);
    int getBgTimings();
    int getAerTimings();
    int initDevice();
    int writeGPORegister(uint32_t data);
    int readGPORegister();
    
    bool programBiases();
    bool setBias(std::string biasName, unsigned int biasValue);

};

/*  -------------------------------------------------------------------
 
        inherited class to manage aer (hpucore or spinn) device
 
  ------------------------------------------------------------------- */

class aerDevManager : public deviceManager {

public:
    
    aerDevManager(std::string dev); 
    
    virtual bool openDevice();
    virtual void closeDevice();
    
    void write_generic_sp2neu_reg (int devDesc, unsigned int offset,
                                   unsigned int data);
    unsigned int read_generic_sp2neu_reg (int devDesc, unsigned int offset);
    void usage (void);
    
    bool readFifoFull();
    bool readFifoEmpty();
    bool writeFifoAFull();
    bool writeFifoFull();
    bool writeFifoEmpty();
    int timeWrapCount();
    
};

/*  -------------------------------------------------------------------
    
    inherited class to manage aer (fx2_0) device
 
  ------------------------------------------------------------------- */

class aerfx2_0DevManager : public deviceManager {

public:
    
    aerfx2_0DevManager();
                       
    virtual bool openDevice();
    
};



#endif /* defined(__eMorph__deviceManager__) */
