/*
 * VotingAppl.cpp
 *
 *  Created on: 12/09/2018
 *      Author: miguel
 */

#include "VotingAppl.h"

Define_Module(VotingAppl);

VotingAppl::VotingAppl() {
}

VotingAppl::~VotingAppl() {
}

void VotingAppl::initialize(int stage){
    GeneralPlexeAgentAppl::initialize(stage);
    if (stage == 1){
        // connect negotiation application to protocol
        protocol->registerApplication(NEGOTIATION_TYPE, gate("lowerLayerIn"), gate("lowerLayerOut"), gate("lowerControlIn"), gate("lowerControlOut"));
        // set initial beliefs;
        if(getPlatoonRole() == PlatoonRole::NONE) {
            //This is the joiner vehicle. Call the connection manager to add belief to join platoon to agent
            BeliefModel bm;
            bm.setBelief("foundplatoon");
            int platoonId = 0;
            int leaderId = 0;
            bm.pushInt(&platoonId);
            bm.pushInt(&leaderId);
            manager->sendInformationToAgents(myId,&bm);
        }else{
            //This a vehicle belonging to a platoon
            if (getPlatoonRole() == PlatoonRole::LEADER) {
                //This is a leader, push the beliefs that allow an agent to know its role
                BeliefModel cbelief;
                BeliefModel pbelief;
                cbelief.setBelief("ischair");
                int platoonId = positionHelper->getPlatoonId();
                cbelief.pushInt(&platoonId);
                manager->sendInformationToAgents(myId, &cbelief);
                pbelief.setBelief("inplatoon");
                int leaderId = positionHelper->getLeaderId();
                pbelief.pushInt(&platoonId);
                pbelief.pushInt(&leaderId);
                manager->sendInformationToAgents(myId, &pbelief);
                BeliefModel platoonSpeedBelief;
                platoonSpeedBelief.setBelief("platoonspeed");
                double platoonSpeed = (positionHelper->getPlatoonSpeed() * 3.6);
                platoonSpeedBelief.pushDouble(&platoonSpeed);
                manager->sendInformationToAgents(myId, &platoonSpeedBelief);
                std::vector<int> members = positionHelper->getPlatoonFormation();
                for (uint32_t i = 0; i < members.size(); i++){
                    BeliefModel mbelief;
                    mbelief.setBelief("addmember");
                    int member = members[i];
                    mbelief.pushInt(&member);
                    manager->sendInformationToAgents(myId, &mbelief);
                }
            }
            else if (getPlatoonRole() == PlatoonRole::FOLLOWER){
                //This is a member, push beliefs
                BeliefModel pbelief;
                pbelief.setBelief("inplatoon");
                int platoonId = positionHelper->getPlatoonId();
                int leaderId = positionHelper->getLeaderId();
                pbelief.pushInt(&platoonId);
                pbelief.pushInt(&leaderId);
                manager->sendInformationToAgents(myId, &pbelief);
            }
        }
    }
}

void VotingAppl::sendRequestToJoin(int targetPlatooId, int destinationId, double preferedSpeed, double tolerance){
    RequestJoinPlatoonMessage* msg = new RequestJoinPlatoonMessage("RequestJoinPlatoonMessage");
    msg->setKind(NEGOTIATION_TYPE);
    msg->setVehicleId(myId);
    msg->setExternalId(positionHelper->getExternalId().c_str());
    msg->setPlatoonId(targetPlatooId);
    msg->setDestinationId(destinationId);
    msg->setPreferedSpeed(preferedSpeed);
    msg->setTolerance(tolerance);
    sendUnicast(msg, destinationId);
}

void VotingAppl::sendNotificationOfJoinVote(double preferedspeed, double tolerance){
    NotificationOfJoinVote* msg = new NotificationOfJoinVote("NotificationOfJoinVote");
    msg->setKind(NEGOTIATION_TYPE);
    msg->setVehicleId(myId);
    msg->setExternalId(positionHelper->getExternalId().c_str());
    msg->setDestinationId(-1);
    msg->setPreferedSpeed(preferedspeed);
    msg->setTolerance(tolerance);
    double platoonSpeed = (positionHelper->getPlatoonSpeed() * 3.6);
    double joinerSpeed = preferedspeed;
    double joinerPreference = tolerance;
    BeliefModel jbelief;
    jbelief.setBelief("openvotetojoin");
    jbelief.pushDouble(&joinerSpeed);
    jbelief.pushDouble(&joinerPreference);
    jbelief.pushDouble(&platoonSpeed);
    manager->sendInformationToAgents(myId, &jbelief);
    sendUnicast(msg, -1);
}

void VotingAppl::onPlatoonBeacon(const PlatooningBeacon* pb){
    //TODO: If this is a beacon for voting handle it
    GeneralPlexeAgentAppl::onPlatoonBeacon(pb);
}

void VotingAppl::handleLowerMsg(cMessage* msg){
    UnicastMessage* unicast = check_and_cast<UnicastMessage*>(msg);

    cPacket* enc = unicast->getEncapsulatedPacket();
    ASSERT2(enc, "received a UnicastMessage with nothing inside");

    if (enc->getKind() == NEGOTIATION_TYPE) {
        NegotiationMessage* nm = check_and_cast<NegotiationMessage*>(unicast->decapsulate());
        if (RequestJoinPlatoonMessage* msg = dynamic_cast<RequestJoinPlatoonMessage*>(nm)) {
            handleRequestToJoinNegotiation(msg);
            delete msg;
        }else if (NotificationOfJoinVote* msg = dynamic_cast<NotificationOfJoinVote*>(nm)) {
            handleNotificationOfJoinVote(msg);
            delete msg;
        }
        delete unicast;
    }
    else {
        GeneralPlexeAgentAppl::handleLowerMsg(msg);
    }
}

void VotingAppl::handleRequestToJoinNegotiation(const RequestJoinPlatoonMessage* msg){
    //Push the knowledge of a potential joiner to the agent
    int joinerId = msg->getVehicleId();
    double joinerSpeed = msg->getPreferedSpeed();
    double joinerPreference = msg->getTolerance();
    BeliefModel jbelief;
    jbelief.setBelief("requestjoin");
    jbelief.pushInt(&joinerId);
    jbelief.pushDouble(&joinerSpeed);
    jbelief.pushDouble(&joinerPreference);
    manager->sendInformationToAgents(myId, &jbelief);
}

void VotingAppl::handleNotificationOfJoinVote(const NotificationOfJoinVote* msg){
    if (positionHelper->isInSamePlatoon(msg->getVehicleId())) { // Verify that it is from this platoon
        double joinerSpeed = msg->getPreferedSpeed();
        double joinerPreference = msg->getTolerance();
        double platoonSpeed = (positionHelper->getPlatoonSpeed() * 3.6);
        BeliefModel jbelief;
        jbelief.setBelief("openvotetojoin");
        jbelief.pushDouble(&joinerSpeed);
        jbelief.pushDouble(&joinerPreference);
        jbelief.pushDouble(&platoonSpeed);
        manager->sendInformationToAgents(myId, &jbelief);
    }
}

void VotingAppl::sendToAgent(const BeliefModel* bm){
    manager->sendInformationToAgents(myId, bm);
}
