/*
 * RouteVotingAppl.cpp
 *
 *  Created on: 14/10/2018
 *      Author: miguel
 */

#include "RouteVotingAppl.h"

Define_Module(RouteVotingAppl);

RouteVotingAppl::~RouteVotingAppl(){
    //cancelAndDelete(updateCurrentSpeed);
}

void RouteVotingAppl::initialize(int stage){
    GeneralPlexeAgentAppl::initialize(stage);
    if (stage == 1){
        // connect negotiation application to protocol
        protocol->registerApplication(NEGOTIATION_TYPE, gate("lowerLayerIn"), gate("lowerLayerOut"), gate("lowerControlIn"), gate("lowerControlOut"));
        // set initial beliefs
        //set speed from config file
        int desiredSpeed = ((VoteManager*)manager)->getPreferredSpeed(myId);
        //Get the desired path from the config file
        std::vector<int> desiredPath = ((VoteManager*)manager)->getPreferredPath(myId);
        BeliefModel ds("set/prefered/speed");
        ds.pushInt(&desiredSpeed);
        manager->sendInformationToAgents(myId, &ds);
        BeliefModel dp("set/prefered/path");
        dp.pushIntArray(desiredPath);
        manager->sendInformationToAgents(myId, &dp);
        BeliefModel us("update/speed");
        int currentSpeed = mobility->getSpeed() * 3.6;
        us.pushInt(&currentSpeed);
        manager->sendInformationToAgents(myId, &us);
        if(getPlatoonRole() == PlatoonRole::NONE) {
            //Set the belief in the agent that they are looking for platoon proposals
            if(par("engageNegotiations").boolValue()){
                BeliefModel bm;
                bm.setBelief("lookforplatoon");
                manager->sendInformationToAgents(myId,&bm);
                searchingForPlatoon = true;
            }
        }else{
            //This a vehicle belonging to a platoon
            BeliefModel pbelief;
            pbelief.setBelief("set/initial/beliefs");
            int platoonId = positionHelper->getPlatoonId();
            int leaderId = positionHelper->getLeaderId();
            pbelief.pushInt(&platoonId);
            pbelief.pushInt(&leaderId);
            int platoonSpeed = (positionHelper->getPlatoonSpeed() * 3.6);
            pbelief.pushInt(&platoonSpeed);
            int chair = -1;
            std::vector<int> members = positionHelper->getPlatoonFormation();
            if (getPlatoonRole() == PlatoonRole::LEADER) {
                //This is a leader, push the beliefs that allow an agent to know its role
                chair = platoonId;
                for (uint32_t i = 0; i < members.size(); i++){
                    received_votes[members[i]] = false;
                }
                leaderInitialBehaviour();
            }
            pbelief.pushInt(&chair);
            pbelief.pushIntArray(members);
            manager->sendInformationToAgents(myId, &pbelief);
        }
        updateCurrentSpeed = new cMessage("updateCurrentSpeed");
        scheduleAt(simTime() + 1, updateCurrentSpeed);
    }
}

void RouteVotingAppl::leaderInitialBehaviour(){
    sendProposal = new cMessage("sendProposal");
    scheduleAt(simTime() + 0.5, sendProposal);
    negotiationState = VoteState::CHAIR_SEARCHING_JOINERS;
}

void RouteVotingAppl::finalizeManeuver(int joinerId){
    BeliefModel mnv("start/vote/node");
    mnv.pushInt(&joinerId);
    double args = -1;
    mnv.pushDouble(&args);
    manager->sendInformationToAgents(myId, &mnv);
    cycle = VoteCycle::ROUTE_VOTE;
    ((VoteManager*)manager)->storeTimeStamp(simTime().dbl() * 1000, VoteManager::TimeStampAction::TIME_OF_VOTE_START);
}

void RouteVotingAppl::sendJoinProposal(){
    JoinProposal* msg = new JoinProposal("JoinProposal");
    fillNegotiationMessage(msg, myId, -1);
    msg->setPlatoonId(positionHelper->getPlatoonId());
    std::vector<int> memberSpeeds = ((VoteManager*)manager)->getElementsPreferredSpeed(positionHelper->getPlatoonFormation());
    msg->setMemberSpeedsArraySize(memberSpeeds.size());
    for(uint32_t i = 0; i < memberSpeeds.size(); i++){
        msg->setMemberSpeeds(i, memberSpeeds[i]);
    }
    sendUnicast(msg, -1);
}


void RouteVotingAppl::sendVoteResults(int winnerValue, int joinerId){
    VotingAppl::sendVoteResults(winnerValue, joinerId);
}

void RouteVotingAppl::sendCommitteeVoteResults(std::vector<int>& results){
    Enter_Method_Silent();
    negotiationState = VoteState::CHAIR_ELECTION_END;
    NotifyResults* msg = new NotifyResults("NotifyResults");
    //Save results for future use
    election_data.currentResult = -1;
    election_data.committeeResult = results;
    int platoonId = positionHelper->getPlatoonId();
    msg->setKind(NEGOTIATION_TYPE);
    msg->setVehicleId(myId);
    msg->setExternalId(positionHelper->getExternalId().c_str());
    msg->setDestinationId(-1);
    msg->setResult(-1);
    msg->setPlatoonId(platoonId);
    msg->setCommitteeResultArraySize(results.size());
    for(uint32_t i = 0; i < results.size(); i++) msg->setCommitteeResult(i, results[i]);
    sendUnicast(msg, -1);
    //Set a delay to start the speed vote
    startSpeedVoteDelay = new cMessage("startSpeedVoteDelay");
    scheduleAt(simTime() + 0.1, startSpeedVoteDelay);
}

void RouteVotingAppl::delegateNegotiationMessage(NegotiationMessage* nm){
    if(JoinProposal* jp = dynamic_cast<JoinProposal*>(nm)){
        if(searchingForPlatoon){
            int platoonId = ((LeaderPositionHelper*)positionHelper)->isPlatoonLeader(jp->getVehicleId());
            if( platoonId >= 0 ){
                BeliefModel platoonBelief;
                platoonBelief.setBelief("pushplatoon/start");
                platoonBelief.pushInt(&platoonId);
                double speedSum = 0;
                for(uint32_t i = 0; i < jp->getMemberSpeedsArraySize(); i++){
                    speedSum += jp->getMemberSpeeds(i);
                }
                int platoonSpeed = speedSum / jp->getMemberSpeedsArraySize();
                platoonBelief.pushInt(&platoonSpeed);
                int leaderId = jp->getVehicleId();
                platoonBelief.pushInt(&leaderId);
                manager->sendInformationToAgents(myId, &platoonBelief);
                searchingForPlatoon = false;
                negotiationState = VoteState::JOINER_AWAITING_ACK_JOIN_REQUEST;
                std::cout << "888888888 GOT THE JOIN PROPOSAL 888888888888" << std::endl;
            }
        }
        delete jp;
    }else
        VotingAppl::delegateNegotiationMessage(nm);
}

void RouteVotingAppl::handleNotificationOfResults(const NotifyResults* msg){
    if(msg->getResult() > -1){
        VotingAppl::handleNotificationOfResults(msg);
    }else{
        BeliefModel result("handle/results/committee");
        std::vector<int> resultsVector(msg->getCommitteeResultArraySize());
        for(uint32_t i = 0; i < msg->getCommitteeResultArraySize(); i++) resultsVector[i] = msg->getCommitteeResult(i);
        result.pushIntArray(resultsVector);
        manager->sendInformationToAgents(myId, &result);
    }
    negotiationState = VoteState::NONE;
}

void RouteVotingAppl::handleRequestToJoinNegotiation(const RequestJoinPlatoonMessage* msg){
    if(negotiationState == VoteState::CHAIR_SEARCHING_JOINERS){
        negotiationState = VoteState::CHAIR_ELECTION_ONGOING;
        VotingAppl::handleRequestToJoinNegotiation(msg);
    }
    sendAck(AckType::JOIN_REQUEST_RECEIVED, msg->getVehicleId());
}

void RouteVotingAppl::handleAck(const Ack* msg){
    AckType type = (AckType)(msg->getAckType());
    if((type == AckType::JOIN_REQUEST_RECEIVED) && (negotiationState==VoteState::JOINER_AWAITING_ACK_JOIN_REQUEST)){
        delete copy;
        negotiationState = VoteState::AWAITING_RESULTS;
        //delete awaitAckTimer;
        cancelAndDelete(awaitAckTimer);
        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::normal_distribution<double> distribution(50,2.0);
        double delay = distribution(gen) * 0.01;
        awaitAckTimer = new cMessage("awaitAckTimer");
        scheduleAt(simTime() + delay, awaitAckTimer);
    }else{
        VotingAppl::handleAck(msg);
    }
}

void RouteVotingAppl::handleSelfMsg(cMessage* msg){
    if(msg == sendProposal){
        delete msg;
        sendJoinProposal();
        sendProposal = new cMessage("sendProposal");
        scheduleAt(simTime() + 0.5, sendProposal);
    }else if(msg == awaitAckTimer){
        switch(negotiationState){
        case VoteState::JOINER_AWAITING_ACK_JOIN_REQUEST:{
            delete msg;
            awaitAckTimer = NULL;
            RequestJoinPlatoonMessage* resend = dynamic_cast<RequestJoinPlatoonMessage*>(copy->dup());
            sendUnicast(resend, resend->getDestinationId());
            awaitAckTimer = new cMessage("awaitAckTimer");
            scheduleAt(simTime() + 0.1, awaitAckTimer);
            ((VoteManager*)manager)->incrementRetransmission();
            break;
        }
        default:
            VotingAppl::handleSelfMsg(msg);
            break;
        }
    }else if(msg == updateCurrentSpeed){
        delete msg;
        BeliefModel us("update/speed");
        int speed = mobility->getSpeed() * 3.6;
        us.pushInt(&speed);
        manager->sendInformationToAgents(myId, &us);
        updateCurrentSpeed = new cMessage("updateCurrentSpeed");
        scheduleAt(simTime() + 1, updateCurrentSpeed);
    }else if(msg == startSpeedVoteDelay){
        delete msg;
        startSpeedVoteDelay = NULL;
        BeliefModel mnv("start/vote/speed");
        double arg = 0;
        mnv.pushDouble(&arg);
        manager->sendInformationToAgents(myId, &mnv);
        cycle = VoteCycle::SPEED_VOTE;
    }  else{
        VotingAppl::handleSelfMsg(msg);
    }
}
