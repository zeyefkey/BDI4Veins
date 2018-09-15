//Defines a standard voter agent

//Belief base definition
//inplatoon(_id).
//lookforplatoon.
//targetplatoonkoin(_id).
//travelroute([_l1, .., _ln]).
//ischair.
//platoonMembers([L, M1, ..., Mn]).
minimumUtility(0.8).

//Initial Goal
!main.

generateutility(JSPEED, JPREFERENCE, PSPEED, PredictedUtility)
    :-  >>tolerance(Tolerance); >>preferedspeed(SpeedPreference);
        PredictedUtility = utility/predictplatoonspeed(JSPEED, JPREFERENCE, SpeedPreference, Tolerance, PSPEED).

//Agent Belief Set up
+!main <-
    Tolerance = utility/generatetolerance();
    SpeedPreference = utility/generatespeedpreference();
    +tolerance(Tolerance);
    +preferedspeed(SpeedPreference);
    generic/print("Agent ", MyName, " of type ", MyType ," started, has preference for speed: ", SpeedPreference ," and tolerance of: ", Tolerance).

+addmember(L) <-
    utility/storemember(L);
    -addmember(L).
        

+ballotopen() <-
    generic/print("Agent ", MyName, " got notification of a new ballot starting").

+!lookforplatoon() <-
    generic/print("Agent ", MyName, " have intention of searching for platoon").
    //transmit/self/searchForPlatoon(). //Set controller layer to send to agent all platoons that are open for joining

+foundplatoon(PID, LID) <-
    generic/print("Agent ", MyName, " found a potential platoon: ", PID);
    !attemptjoin(PID, LID).

+requestjoin(JID, JSPEED, JPREFERENCE) <-
    !handlejoinrequest(JID, JSPEED, JPREFERENCE).

+!attemptjoin(PID, LID) <-
    +targetplatoonjoin(PID);
    +leaderid(LID);
    generic/print("Agent ", MyName, " sending a request to join");
    >>preferedspeed(SPEED);
    >>tolerance(TOLERANCE);
    transmit/other/sendjoinplatoonrequest(PID, LID, SPEED, TOLERANCE).

+!handlejoinrequest(JID, JSPEED, JPREFERENCE) <-
    generic/print("Agent ", MyName, " received a request to join the platoon from ", JID, "who preferes speed:", JSPEED, " with a tolerance of ", JPREFERENCE);
    S = utility/platoonsize();
    vote/openballot("allowJoin", JID, S);
    transmit/other/vote/join(JSPEED, JPREFERENCE).
    //start a list of votes;
    //set belief of open vote in state of awaiting ack from members +openJoinBallot(JSPEED, JPREFERENCE, [])

+openvotetojoin(JSPEED, JPREFERENCE) <-
    >>platoonspeed(PSPEED);
    !handleopenvotetojoin(JSPEED, JPREFERENCE, PSPEED).

+openvotetojoin(JSPEED, JPREFERENCE, PSPEED) <-
    +platoonspeed(PSPEED);
    !handleopenvotetojoin(JSPEED, JPREFERENCE, PSPEED).
    //save the sent vote

+submitvote(VOTER, VOTE) <-
    generic/print("GOT VOTE");
    !handlesubmitvote(VOTER, VOTE).

+!handlesubmitvote(VOTER, VOTE)
    : MyName == VOTER <-
        generic/print("Got my own vote");
        vote/store(VOTER, VOTE)
    : MyName != VOTER <-
        generic/print("Got vote from", VOTER);
        vote/store(VOTER,VOTE).

+!handleopenvotetojoin(JSPEED, JPREFERENCE, PSPEED) <-
    generic/print("Agent ", MyName, "got notified of a join vote for a vehicle who preferes speed:", JSPEED, " with a tolerance of ", JPREFERENCE); 
    $generateutility(JSPEED, JPREFERENCE, PSPEED, PredictedUtility);
    generic/print("Agent ", MyName, "utility is:", PredictedUtility);
    !choosevote(PredictedUtility).

+!choosevote(PredictedUtility)
    : >>(minimumUtility(MinUtil), MinUtil < PredictedUtility ) <-
        generic/print("Agent ", MyName, "Casting positive vote:", PredictedUtility, ">", MinUtil);
        !sendvote(1)
    : >>(minimumUtility(MinUtil), MinUtil >= PredictedUtility) <-
        generic/print("Agent ", MyName, "Casting negative vote:", PredictedUtility, "<", MinUtil);
        !sendvote(0).

+!sendvote(VOTE)
    : >>isChair(_) <-
        generic/print("Agent", MyName, "Im the chair so no need to pass through omnet");
        vote/chairstore(VOTE)
    : ~>>isChair(_) <- 
        generic/print("Agent", MyName, "Sending the vote down omnet");
       transmit/other/vote/cast(VOTE). 
+inplatoon(PID, LID) <-
    generic/print("Agent ", MyName, " is in platoon ", PID, " whoose leader is: ", LID).

+ischair(PID) <-
    +isChair(PID);
    generic/print("Agent ", MyName, " is chair of platoon ", PID).

+!notify/joiner/win(JID) <-
    generic/print("APROVED!");
    vote/send/results(JID, 1).

+!notify/joiner/lose(JID) <-
    generic/print("REJECTED!");
    vote/send/results(JID, 0).