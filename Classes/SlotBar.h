#pragma once

#include "cocos2d.h"
#include "GAF.h"
#include "SlotMachine.h"

USING_NS_GAF;

class SlotBar : public cocos2d::Ref
{
public:
    static SlotBar* create(GAFObject* mainObject);

    SlotBar();
    bool init(GAFObject* mainObject);

    void randomizeSlots(int maxTypes, std::string machineType);
    void showSpinResult(SlotMachine::PrizeBar_t, std::string machineType);

    GAFObject* getBar();

private:
    GAFObject* m_slots[3];
    GAFObject* m_bar;
};