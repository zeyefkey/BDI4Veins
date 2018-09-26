//
// Generated file, do not edit! Created by nedtool 5.4 from messages/voting/RequestJoinPlatoonMessage.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __REQUESTJOINPLATOONMESSAGE_M_H
#define __REQUESTJOINPLATOONMESSAGE_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0504
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "../NegotiationMessage_m.h"
// }}

/**
 * Class generated from <tt>messages/voting/RequestJoinPlatoonMessage.msg:24</tt> by nedtool.
 * <pre>
 * //
 * // TODO generated message class
 * //
 * packet RequestJoinPlatoonMessage extends NegotiationMessage
 * {
 *     //The id of the platoon the vehicle wishes to join
 *     int platoonId;
 *     //Relevant information about vehicle preferences
 *     double preferedSpeed;
 *     double tolerance;
 * }
 * </pre>
 */
class RequestJoinPlatoonMessage : public ::NegotiationMessage
{
  protected:
    int platoonId;
    double preferedSpeed;
    double tolerance;

  private:
    void copy(const RequestJoinPlatoonMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const RequestJoinPlatoonMessage&);

  public:
    RequestJoinPlatoonMessage(const char *name=nullptr, short kind=0);
    RequestJoinPlatoonMessage(const RequestJoinPlatoonMessage& other);
    virtual ~RequestJoinPlatoonMessage();
    RequestJoinPlatoonMessage& operator=(const RequestJoinPlatoonMessage& other);
    virtual RequestJoinPlatoonMessage *dup() const override {return new RequestJoinPlatoonMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getPlatoonId() const;
    virtual void setPlatoonId(int platoonId);
    virtual double getPreferedSpeed() const;
    virtual void setPreferedSpeed(double preferedSpeed);
    virtual double getTolerance() const;
    virtual void setTolerance(double tolerance);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const RequestJoinPlatoonMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, RequestJoinPlatoonMessage& obj) {obj.parsimUnpack(b);}


#endif // ifndef __REQUESTJOINPLATOONMESSAGE_M_H
