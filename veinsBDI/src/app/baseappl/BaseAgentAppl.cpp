/*
 * BaseAgentAppl.cpp
 *
 *  Created on: Jul 30, 2018
 *      Author: miguel
 */

#include "BaseAgentAppl.h"



 void BaseAgentAppl::initialize(int stage){
     BaseApplLayer::initialize(stage); //TODO: Header subclasses BaseWave. Maybe change that?
     if(stage == 0){
         myId = getParentModule()->getId();
         findHost()->subscribe(mobilityStateChangedSignal, this);
         findHost()->subscribe(parkingStateChangedSignal, this);
     }

}

void BaseAgentAppl::changeSpeed(double speed){
    EV << "Slowing down to speed on agent command\n";
    traciVehicle->setMaxSpeed(speed);
}

void BaseAgentAppl::sendMessage(uint8_t message_type, const void* args){
    throw new cRuntimeError("Send message not implemented!");
}

void BaseAgentAppl::finish(){
    manager->unsubscribeVehicle(myId);
}
