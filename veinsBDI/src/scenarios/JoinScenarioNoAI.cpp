/*
 * JoinScenarioNoAI.cpp
 *
 *  Created on: Sep 7, 2018
 *      Author: miguel
 */

#include "JoinScenarioNoAI.h"
#include "veins/modules/application/platooning/utilities/DynamicPositionManager.h"

Define_Module(JoinScenarioNoAI);

JoinScenarioNoAI::JoinScenarioNoAI() {

}

JoinScenarioNoAI::~JoinScenarioNoAI() {
}

void JoinScenarioNoAI::initialize(int stage)
{

    BaseScenario::initialize(stage);

    if (stage == 0){
        // get pointer to application
        app = FindModule<GeneralPlexeAgentAppl*>::findSubModule(getParentModule());
    }

    if (stage == 1) {

        /*CHECK FOR NON PLATOONING VEHICLES*/
        DynamicPositionManager& positions = DynamicPositionManager::getInstance();
        int aux = positions.vehToPlatoons.count(positionHelper->getId());
        if(aux == 0) {
            traciVehicle->setFixedLane(traciVehicle->getLaneIndex(), false);
            traciVehicle->setActiveController(Plexe::ACC);
            //traciVehicle->setSpeed(mobility->getSpeed());
            traciVehicle->setCruiseControlDesiredSpeed(mobility->getSpeed());
            positionHelper->setPlatoonId(-1);
            positionHelper->setIsLeader(false);
            positionHelper->setPlatoonLane(-1);

            //startManeuver = new cMessage();
            //scheduleAt(simTime() + SimTime(0.5), startManeuver);
       }else{
           double speedModifier[] =  {1, 0.6, 1.5};
           int platoonId = positionHelper->getPlatoonId();
           //traciVehicle->setSpeed(mobility->getSpeed() * speedModifier[platoonId]);
           double modSpeed = mobility->getSpeed() * speedModifier[platoonId];

           platooningVType = par("platooningVType").stdstringValue();

           traciVehicle->setFixedLane(traciVehicle->getLaneIndex(), false);
           traciVehicle->setSpeedMode(0);
           if (positionHelper->isLeader()) {
               for (int i = 1; i < positionHelper->getPlatoonSize(); i++) {
                   std::stringstream ss;
                   ss << platooningVType << "." << positionHelper->getMemberId(i);
                   traciVehicle->addPlatoonMember(ss.str(), i);
               }
               traciVehicle->enableAutoLaneChanging(false);
               traciVehicle->setFixedLane(traciVehicle->getLaneIndex(), true);
               traciVehicle->setCruiseControlDesiredSpeed(modSpeed);
           }
           else {
               std::stringstream ssl, ss;
               ssl << platooningVType << "." << positionHelper->getLeaderId();
               ss << platooningVType << "." << positionHelper->getFrontId();
               //traciVehicle->enableAutoFeed(false, ssl.str(), ss.str());
               traciVehicle->setCruiseControlDesiredSpeed(mobility->getSpeed() + 10);
           }
           positionHelper->setPlatoonSpeed(modSpeed);
       }
    }
}

void JoinScenarioNoAI::handleSelfMsg(cMessage* msg)
{

    // this takes care of feeding data into CACC and reschedule the self message
    BaseScenario::handleSelfMsg(msg);

    if (msg == startManeuver) app->startJoinManeuver(0, 0, -1);
    if (msg == test){
        delete test;
        traciVehicle->setCruiseControlDesiredSpeed(traciVehicle->getCruiseControlDesiredSpeed() - 10);
        test = new cMessage();
        scheduleAt(simTime() + SimTime(0.5), test);
    }
}
