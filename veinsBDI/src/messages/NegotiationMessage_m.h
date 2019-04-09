//
// Generated file, do not edit! Created by nedtool 5.4 from messages/NegotiationMessage.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __NEGOTIATIONMESSAGE_M_H
#define __NEGOTIATIONMESSAGE_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0504
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
    /** message type for maneuver messages */
    #include <unordered_set>
    typedef std::unordered_set<int> IntSet;
    static const int NEGOTIATION_TYPE = 22347;
// }}

/**
 * Class generated from <tt>messages/NegotiationMessage.msg:25</tt> by nedtool.
 * <pre>
 * // General message to initiate negotiation procedures.
 * // Only children of this message should be initialized.
 * packet NegotiationMessage
 * {
 *     // id of the originator of this message
 *     int vehicleId;
 *     // id of the destination of this message
 *     int destinationId;
 *     // sumo external id of the sender
 *     string externalId;
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
 *     //Specifies if this an ack type message
 *     bool isAck = false;
 * }
 * </pre>
 */
class NegotiationMessage : public ::omnetpp::cPacket
{
  protected:
    int vehicleId;
    int destinationId;
    ::omnetpp::opp_string externalId;
    int platoonId;
    IntSet targets;
    bool forWholePlatoon;
    int messageId;
    int replyMessageId;
    int messageType;
    bool isAck;

  private:
    void copy(const NegotiationMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const NegotiationMessage&);

  public:
    NegotiationMessage(const char *name=nullptr, short kind=0);
    NegotiationMessage(const NegotiationMessage& other);
    virtual ~NegotiationMessage();
    NegotiationMessage& operator=(const NegotiationMessage& other);
    virtual NegotiationMessage *dup() const override {return new NegotiationMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getVehicleId() const;
    virtual void setVehicleId(int vehicleId);
    virtual int getDestinationId() const;
    virtual void setDestinationId(int destinationId);
    virtual const char * getExternalId() const;
    virtual void setExternalId(const char * externalId);
    virtual int getPlatoonId() const;
    virtual void setPlatoonId(int platoonId);
    virtual IntSet& getTargets();
    virtual const IntSet& getTargets() const {return const_cast<NegotiationMessage*>(this)->getTargets();}
    virtual void setTargets(const IntSet& targets);
    virtual bool getForWholePlatoon() const;
    virtual void setForWholePlatoon(bool forWholePlatoon);
    virtual int getMessageId() const;
    virtual void setMessageId(int messageId);
    virtual int getReplyMessageId() const;
    virtual void setReplyMessageId(int replyMessageId);
    virtual int getMessageType() const;
    virtual void setMessageType(int messageType);
    virtual bool getIsAck() const;
    virtual void setIsAck(bool isAck);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const NegotiationMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, NegotiationMessage& obj) {obj.parsimUnpack(b);}


#endif // ifndef __NEGOTIATIONMESSAGE_M_H

