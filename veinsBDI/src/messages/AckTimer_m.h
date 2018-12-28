//
// Generated file, do not edit! Created by nedtool 5.4 from messages/AckTimer.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __ACKTIMER_M_H
#define __ACKTIMER_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0504
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>messages/AckTimer.msg:17</tt> by nedtool.
 * <pre>
 * //Message used for timers related to ACK routines.
 * message AckTimer
 * {
 *     // Id related to the message to be checked on
 *     long messageId;
 * }
 * </pre>
 */
class AckTimer : public ::omnetpp::cMessage
{
  protected:
    long messageId;

  private:
    void copy(const AckTimer& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const AckTimer&);

  public:
    AckTimer(const char *name=nullptr, short kind=0);
    AckTimer(const AckTimer& other);
    virtual ~AckTimer();
    AckTimer& operator=(const AckTimer& other);
    virtual AckTimer *dup() const override {return new AckTimer(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual long getMessageId() const;
    virtual void setMessageId(long messageId);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const AckTimer& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, AckTimer& obj) {obj.parsimUnpack(b);}


#endif // ifndef __ACKTIMER_M_H
