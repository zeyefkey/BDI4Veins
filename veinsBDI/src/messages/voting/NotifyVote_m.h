//
// Generated file, do not edit! Created by nedtool 5.4 from messages/voting/NotifyVote.msg.
//

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#ifndef __NOTIFYVOTE_M_H
#define __NOTIFYVOTE_M_H

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
 * Class generated from <tt>messages/voting/NotifyVote.msg:20</tt> by nedtool.
 * <pre>
 * packet NotifyVote extends NegotiationMessage
 * {
 *     int candidates[];
 *     string context;
 * }
 * </pre>
 */
class NotifyVote : public ::NegotiationMessage
{
  protected:
    int *candidates; // array ptr
    unsigned int candidates_arraysize;
    ::omnetpp::opp_string context;

  private:
    void copy(const NotifyVote& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const NotifyVote&);

  public:
    NotifyVote(const char *name=nullptr, short kind=0);
    NotifyVote(const NotifyVote& other);
    virtual ~NotifyVote();
    NotifyVote& operator=(const NotifyVote& other);
    virtual NotifyVote *dup() const override {return new NotifyVote(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual void setCandidatesArraySize(unsigned int size);
    virtual unsigned int getCandidatesArraySize() const;
    virtual int getCandidates(unsigned int k) const;
    virtual void setCandidates(unsigned int k, int candidates);
    virtual const char * getContext() const;
    virtual void setContext(const char * context);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const NotifyVote& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, NotifyVote& obj) {obj.parsimUnpack(b);}


#endif // ifndef __NOTIFYVOTE_M_H

