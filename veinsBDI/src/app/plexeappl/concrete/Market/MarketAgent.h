/*
 * MarketAgent.h
 *
 *  Created on: Dec 18, 2018
 *      Author: miguel
 */

#ifndef APP_PLEXEAPPL_CONCRETE_MARKET_MARKETAGENT_H_
#define APP_PLEXEAPPL_CONCRETE_MARKET_MARKETAGENT_H_

#include <random>
#include <unordered_set>

#include "veins/modules/application/platooning/utilities/DynamicPositionHelper.h"

#include "../../GeneralPlexeAgentAppl.h"
#include "../../../../utilities/LeaderPositionHelper.h"
#include "../../../../messages/AckTimer_m.h"
#include "../../../../messages/utility/MessageCache.h"


class MarketAgent : public GeneralPlexeAgentAppl {
public:
    MarketAgent(){};
    virtual ~MarketAgent();

    /** override from GeneralPlexeAgentAppl */
    virtual void initialize(int stage) override;

    enum class MessageType{
        NONE,
        ACK,
        OK,
        HELLO
    };
protected:
    /**
     *
     */
    virtual void delegateNegotiationMessage(NegotiationMessage* nm);
    /**
     * Extend from GeneralPlexeAgentAppl to handle messages related to voting
     */
    virtual void handleLowerMsg(cMessage* msg) override;
    /**
     * Fill a negotiation message with the standard information
     */
    void fillNegotiationMessage(MarketMessage* msg, int originId, int targetId,
            bool forWholePlatoon=true);
    void fillNegotiationMessage(MarketMessage* msg, int originId, int targetId,
            bool forWholePlatoon, std::unordered_set<int>& targets);
    void fillNegotiationMessage(MarketMessage* msg, int originId, int targetId,
            bool forWholePlatoon, int idOfOriginMessage);
    /**
     * Handle messages sent to self
     */
    void handleSelfMsg(cMessage* msg) override;
    /**
     * Resends a message from cache
     */
    void resendMessage(long msgId, AckTimer* at);
    /*
     * Send message with ack control
     */
    void sendMessageWithAck(MarketMessage* msg, const std::vector<int>& targets);
    void sendMessageWithAck(MarketMessage* msg, int target);
    /*
     * Check if this vehicle is the intended target of a message
     */
    bool isReceiver(MarketMessage* msg);
    /*
     * Seconds to wait for ack messages
     */
    const double ackTime = 0.2;
public:
    /*
     * Methods to send messages
     */
    void sendBid(int auctionId, int context, int bidValue, int managerId);

    void sendAuctionResults(int auctionId, int auctionIteration, int winnerId);

    void sendNotificationOfAuction(int auctionId, int context);

    void handleEndOfAuction(int auctionId, int auctionIteration, int winnerId);
    void handleEndOfAuction(int auctionId, int auctionIteration, int winnerId, int pay, int wtpSum, int context);

    void sendPay(int auctionId, int context, int pay, int managerId, int speed);
    void sendPay(int auctionId, int context, int pay, int managerId, std::vector<int> route);

    void distributePay(int auctionId, int auctionIteration, int winnerId, int payment, int wtpSum, int speed);
    void distributePay(int auctionId, int auctionIteration, int winnerId, int payment, int wtpSum, std::vector<int> route);
private:
    /*
     * Manage the messages that are sent
     */
    MessageCache messageCache;
    /*
     * Timers
     */
    cMessage* debugTimer = NULL;
    void testFunction();
};

#endif /* APP_PLEXEAPPL_CONCRETE_MARKET_MARKETAGENT_H_ */
