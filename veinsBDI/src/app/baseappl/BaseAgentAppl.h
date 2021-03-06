/*
 * BaseAgentAppl.h
 *
 *  Created on: Jul 30, 2018
 *      Author: miguel
 */

#ifndef BASEAGENTAPPL_H_
#define BASEAGENTAPPL_H_

#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "../../managers/manager/Trigger/Trigger.h"
#include "../../managers/manager/LightJasonManager.h"

using Veins::TraCIMobility;
using Veins::TraCICommandInterface;

class LightJasonManager;
class BaseAgentAppl : public Veins::BaseApplLayer/*BaseWaveApplLayer*/ {
public:
    BaseAgentAppl():manager(NULL){}
    virtual void initialize(int stage) override;
    virtual int numInitStages()const override{return 3;}
    virtual void finish();
    virtual void receiveSignal(cComponent* source, simsignal_t signalID, cObject* obj, cObject* details){};
    virtual void sendMessage(uint8_t message_type, const void* args);
protected:
    static const simsignalwrap_t mobilityStateChangedSignal;
    int myId;
    TraCIMobility* mobility;
    TraCICommandInterface* traci;
    TraCICommandInterface::Vehicle* traciVehicle;
    simtime_t lastSent; //last time this sent a message
    LightJasonManager* manager;
    uint8_t Belief(std::string key);
    virtual void onWSM(Veins::WaveShortMessage* wsm){

    }
    virtual void handleLowerMsg(cMessage* msg){}
    virtual void handleSelfMsg(cMessage* msg);
    virtual void handleLowerControl(cMessage* msg){}
    virtual void setInitialBeliefs(){}
private:
    cMessage* triggerInitialBeliefs;
};

#endif /* BASEAGENTAPPL_H_ */
