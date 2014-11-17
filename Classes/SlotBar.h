#pragma once

#include "cocos2d.h"
#include "GAF.h"
#include "SlotMachine.h"

USING_NS_GAF;

class SlotBar : public cocos2d::Ref
{
public:
    struct SequencePlaybackInfo
    {
        SequencePlaybackInfo() {}
        SequencePlaybackInfo(std::string name_, bool looped_)
        {
            name = name_;
            looped = looped_;
        }
        std::string name;
        bool looped;
    };

    static SlotBar* create(GAFObject* mainObject);

    SlotBar();
    bool init(GAFObject* mainObject);

    void update(float dt);

    void playSequenceWithTimeout(SequencePlaybackInfo sequence, float timeout);
    void randomizeSlots(int maxTypes, std::string machineType);
    void showSpinResult(SlotMachine::PrizeBar_t, std::string machineType);

    GAFObject* getBar();

private:
    GAFObject* m_slots[3];
    GAFObject* m_bar;

    float m_countdown;
    SequencePlaybackInfo m_sequence;
};