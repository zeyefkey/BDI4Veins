/*
 * SimpleMergeManager.h
 *
 *  Created on: 29/08/2018
 *      Author: miguel
 */

#ifndef BASE_SIMPLEMERGEMANAGER_SIMPLEMERGEMANAGER_H_
#define BASE_SIMPLEMERGEMANAGER_SIMPLEMERGEMANAGER_H_

#include "../manager/LightJasonManager.h"
#include "../../app/plexeappl/example/SimpleMerge.h"

class SimpleMergeManager : public LightJasonManager {
public:
    SimpleMergeManager();
    virtual ~SimpleMergeManager();

    void initialize(int) override;
protected:
    void parseResponse(uint32_t msgLength) override;
};

#endif /* BASE_SIMPLEMERGEMANAGER_SIMPLEMERGEMANAGER_H_ */