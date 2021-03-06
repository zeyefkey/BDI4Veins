//
// Generated file, do not edit! Created by nedtool 5.4 from messages/market/MarketMessage.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __MARKETMESSAGE_M_H
#define __MARKETMESSAGE_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0504
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "../NegotiationMessage_m.h"
#include <unordered_set>
typedef std::unordered_set<int> IntSet;
// }}

/**
 * Class generated from <tt>messages/market/MarketMessage.msg:23</tt> by nedtool.
 * <pre>
 * //Base message for market applications
 * packet MarketMessage extends NegotiationMessage
 * {
 *     //Platoon ID of sender
 *     int platoonId;
 *     //Set of target nodes
 *     IntSet targets;
 *     //Is this message meant for the entire platoon or a select few?
 *     bool forWholePlatoon;
 *     //Identifier of the message
 *     int messageId;
 *     //Identifier of the message it is replying to
 *     int replyMessageId;
 *     //Specifies what type of message this is. e.g. BID, ACK, OK, etc..
 *     int messageType;
 * }
 * </pre>
 */
class MarketMessage : public ::NegotiationMessage
{
  protected:
    int platoonId;
    IntSet targets;
    bool forWholePlatoon;
    int messageId;
    int replyMessageId;
    int messageType;

  private:
    void copy(const MarketMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const MarketMessage&);

  public:
    MarketMessage(const char *name=nullptr, short kind=0);
    MarketMessage(const MarketMessage& other);
    virtual ~MarketMessage();
    MarketMessage& operator=(const MarketMessage& other);
    virtual MarketMessage *dup() const override {return new MarketMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getPlatoonId() const;
    virtual void setPlatoonId(int platoonId);
    virtual IntSet& getTargets();
    virtual const IntSet& getTargets() const {return const_cast<MarketMessage*>(this)->getTargets();}
    virtual void setTargets(const IntSet& targets);
    virtual bool getForWholePlatoon() const;
    virtual void setForWholePlatoon(bool forWholePlatoon);
    virtual int getMessageId() const;
    virtual void setMessageId(int messageId);
    virtual int getReplyMessageId() const;
    virtual void setReplyMessageId(int replyMessageId);
    virtual int getMessageType() const;
    virtual void setMessageType(int messageType);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const MarketMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, MarketMessage& obj) {obj.parsimUnpack(b);}


#endif // ifndef __MARKETMESSAGE_M_H

