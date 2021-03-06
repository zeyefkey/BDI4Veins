/*
 * JoinAtN.h
 *
 *  Created on: 10/09/2018
 *      Author: miguel
 */

#ifndef MANEUVERS_JOINATN_H_
#define MANEUVERS_JOINATN_H_


#include <algorithm>

#include "BaseJoin.h"
#include "veins/modules/application/platooning/utilities/DynamicPositionHelper.h"

#include "veins/modules/mobility/traci/TraCIConstants.h"
#include "veins/base/utils/Coord.h"

#include "../messages/MoveToPositionN_m.h"
#include "../messages/CreateGap_m.h"
#include "../messages/CreateGapAck_m.h"
#include "../messages/CloseGap_m.h"

using namespace Veins;

class JoinAtN : public BaseJoin {
public:
    /**
     * Constructor
     *
     * @param app pointer to the generic application used to fetch parameters and inform it about a concluded maneuver
     */
    JoinAtN(GeneralPlexeAgentAppl* app);
    virtual ~JoinAtN(){};

    /**
     * This method is invoked by the generic application to start the maneuver
     *
     * @param parameters parameters passed to the maneuver
     */
    virtual void startManeuver(const void* parameters) override;

    /**
     * Handles the abortion of the maneuver when required by the generic application.
     * This method does currently nothing and it is meant for future used and improved maneuvers.
     */
    virtual void abortManeuver() override;

    /**
     * This method is invoked by the generic application when a beacon message is received
     * The maneuver must not free the memory of the message, as this might be needed by other maneuvers as well.
     */
    virtual void onPlatoonBeacon(const PlatooningBeacon* pb) override;

    /**
     * Handles a JoinPlatoonRequest in the context of this application
     *
     * @param JoinPlatoonRequest msg to handle
     */
    virtual void handleJoinPlatoonRequest(const JoinPlatoonRequest* msg) override;

    /**
     * Handles a JoinPlatoonResponse in the context of this application
     *
     * @param JoinPlatoonResponse msg to handle
     */
    virtual void handleJoinPlatoonResponse(const JoinPlatoonResponse* msg) override;

    /**
     * Handles a MoveToPosition in the context of this application
     *
     * @param MoveToPosition msg to handle
     */
    virtual void handleMoveToPosition(const MoveToPosition* msg) override;

    /**
     * Handles a MoveToPositionAck in the context of this application
     *
     * @param MoveToPositionAck msg to handle
     */
    virtual void handleMoveToPositionAck(const MoveToPositionAck* msg) override;

    /**
     * Handles a JoinFormation in the context of this application
     *
     * @param JoinFormation msg to handle
     */
    virtual void handleJoinFormation(const JoinFormation* msg) override;

    /**
     * Handles a JoinFormationAck in the context of this application
     *
     * @param JoinFormationAck msg to handle
     */
    virtual void handleJoinFormationAck(const JoinFormationAck* msg) override;

    virtual void onManeuverMessage(const ManeuverMessage* mm) override;

    virtual void handleCreateGapRequest(const CreateGap* msg);

    virtual void handleCreateGapAck(const CreateGapAck* msg);

    virtual void handleCloseGap(const CloseGap* msg);

protected:
    CreateGap* createCreateGap(int vehicleId, std::string externalId, int platoonId, int destinationId, int joinerId);
    CreateGapAck* createCreateGapAck(int vehicleId, std::string externalId, int platoonId, int destinationId);
    CloseGap* createCloseGap(int vehicleId, std::string externalId, int platoonId, int destinationId);
    MoveToPositionN* createMoveToPositionN(int vehicleId, std::string externalId, int platoonId, int destinationId, double platoonSpeed, int platoonLane, const std::vector<int>& newPlatoonFormation, int position, int frontId, int backId);
    /** Possible states a vehicle can be in during a join maneuver */
    enum class JoinManeuverState {
        IDLE, ///< The maneuver did not start
        // Joiner
        J_WAIT_REPLY, ///< The joiner sent a JoinRequest waits for a reply from the Platoon leader
        J_WAIT_INFORMATION, ///< The joiner waits for information about the Platoon
        J_MOVE_IN_POSITION, ///< The joiner moves to its position
        J_WAIT_JOIN, ///< The joiner waits for the join permission
        // Leader
        L_WAIT_JOINER_IN_POSITION, ///< The leader waits for the joiner to be in position, the followers made space already
        L_WAIT_JOINER_TO_JOIN, ///< The leader waits for the joiner to join
        B_CREATING_GAP, //The Follower is increasing its intervehicular distance
        B_HOLD_GAP //The Follower is maintaining its increased intervehicular distance
    };

    /** data that a joiner stores about a Platoon it wants to join */
    struct TargetPlatoonData {
        int platoonId; ///< the id of the platoon to join
        int platoonLeader; ///< the id of the leader of the platoon
        int platoonLane; ///< the lane the platoon is driving on
        double platoonSpeed; ///< the speed of the platoon
        unsigned int joinIndex; ///< position in the new platoon formation, 0 based !
        std::vector<int> newFormation; ///< the new formation of the platoon
        Coord lastFrontPos; ///< the last known position of the front vehicle

        /** c'tor for TargetPlatoonData */
        TargetPlatoonData()
        {
            platoonId = INVALID_PLATOON_ID;
            platoonLeader = INVALID_INT_VALUE;
            platoonLane = INVALID_INT_VALUE;
            platoonSpeed = INVALID_DOUBLE_VALUE;
            joinIndex = INVALID_INT_VALUE;
        }

        /**
         * Initializes the TargetPlatoonData object with the contents of a
         * MoveToPosition
         *
         * @param MoveToPosition msg to initialize from
         * @see MoveToPosition
         */
        void from(const MoveToPosition* msg)
        {
            platoonId = msg->getPlatoonId();
            platoonLeader = msg->getVehicleId();
            platoonLane = msg->getPlatoonLane();
            platoonSpeed = msg->getPlatoonSpeed();
            newFormation.resize(msg->getNewPlatoonFormationArraySize());
            for (unsigned int i = 0; i < msg->getNewPlatoonFormationArraySize(); i++) {
                newFormation[i] = msg->getNewPlatoonFormation(i);
            }
            const auto it = std::find(newFormation.begin(), newFormation.end(), msg->getDestinationId());
            if (it != newFormation.end()) {
                joinIndex = std::distance(newFormation.begin(), it);
                ASSERT(newFormation.at(joinIndex) == msg->getDestinationId());
            }
        }

        /**
         * Get the id of the front vehicle
         *
         * @return int the id of the front vehicle
         */
        int frontId() const
        {
            return newFormation.at(joinIndex - 1);
        }
    };

    /** data that a leader stores about a joining vehicle */
    struct JoinerData {
        int joinerId; ///< the id of the vehicle joining the Platoon
        int joinerLane; ///< the lane chosen for joining the Platoon
        std::vector<int> newFormation;

        /** c'tor for JoinerData */
        JoinerData()
        {
            joinerId = INVALID_INT_VALUE;
            joinerLane = INVALID_INT_VALUE;
        }

        /**
         * Initializes the JoinerData object with the contents of a
         * JoinPlatoonRequest
         *
         * @param JoinPlatoonRequest jr to initialize from
         * @see JoinPlatoonRequest
         */
        void from(const JoinPlatoonRequest* msg)
        {
            joinerId = msg->getVehicleId();
            joinerLane = msg->getCurrentLaneIndex();
        }
    };

    /** the current state in the join maneuver */
    JoinManeuverState joinManeuverState;

    /** the data about the target platoon */
    std::unique_ptr<TargetPlatoonData> targetPlatoonData;

    /** the data about the current joiner */
    std::unique_ptr<JoinerData> joinerData;

    /** determine if the follower vehicle has increased its distance */
    bool gapCreated;

    /** determine if the joiner is in position */
    bool joinerInPosition;

    /** save the id of the vehicle creating the gap*/
    int gapCreatorId;
};

#endif /* MANEUVERS_JOINATN_H_ */
