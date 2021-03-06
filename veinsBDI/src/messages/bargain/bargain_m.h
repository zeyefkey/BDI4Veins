//
// Generated file, do not edit! Created by nedtool 5.4 from messages/bargain/bargain.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __BARGAIN_M_H
#define __BARGAIN_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0504
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "../market/MarketMessage_m.h"
// }}

/**
 * Class generated from <tt>messages/bargain/bargain.msg:20</tt> by nedtool.
 * <pre>
 * //Base message for barganing
 * packet BargainMessage extends MarketMessage
 * {
 *     //Data to be sent over
 *     int data;
 * }
 * </pre>
 */
class BargainMessage : public ::MarketMessage
{
  protected:
    int data;

  private:
    void copy(const BargainMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const BargainMessage&);

  public:
    BargainMessage(const char *name=nullptr, short kind=0);
    BargainMessage(const BargainMessage& other);
    virtual ~BargainMessage();
    BargainMessage& operator=(const BargainMessage& other);
    virtual BargainMessage *dup() const override {return new BargainMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getData() const;
    virtual void setData(int data);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const BargainMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, BargainMessage& obj) {obj.parsimUnpack(b);}


#endif // ifndef __BARGAIN_M_H

