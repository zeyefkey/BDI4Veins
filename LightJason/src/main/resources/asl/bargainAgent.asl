//--------------//
//Define beliefs//
//--------------//


//----------------//
//Entry point goal//
//----------------//
!main.


//---------------------------//
//Definition of logical rules//
//---------------------------//


//---------------//
//Plan definition//
//---------------//

//Entry point plan
+!main.
//Initialize beliefs
+!setup/beliefs(ISMERGER) : true <-
    //generic/print(MyName, ": merger status: ", ISMERGER);
    +isMerger(ISMERGER);
    return(0)
.

//Environmental triggers
+!bargain/start(VID) : true <-
    offer/send(VID, 0)
.

+!bargain/receive(VID, AMOUNT) : true <-
    offer/evaluate(VID, AMOUNT)
.

+!bargain/receive/result(VID, RESULT) : true <-
    offer/send(VID, RESULT)
.

+!bargain/receive/payout(VID, PAYOUT) : true <-
    offer/receive(VID, PAYOUT);
    return(0)
.