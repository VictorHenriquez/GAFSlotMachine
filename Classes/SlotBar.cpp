#include "SlotBar.h"

USING_NS_GAF;

SlotBar* SlotBar::create(GAFObject* mainObject)
{
    SlotBar* ret = new SlotBar();
    if (ret && ret->init(mainObject))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_RELEASE(ret);
    return nullptr;
}

SlotBar::SlotBar()
    : m_countdown(-1.0f)
{
}

bool SlotBar::init(GAFObject* mainObject)
{
    if (mainObject == nullptr)
        return false;

    m_bar = mainObject;
    
    for (int i = 0; i < 3; i++)
    {
        std::stringstream ss;
        ss << "fruit" << (i + 1);
        m_slots[i] = mainObject->getObjectByName(ss.str());

        if (m_slots[i] == nullptr)
            return false;
    }

    return true;
}

void SlotBar::update(float dt)
{
    if (m_countdown >= 0.0f)
    {
        m_countdown -= dt;
        if (m_countdown < 0.0f)
        {
            m_bar->playSequence(m_sequence.name, m_sequence.looped);
        }
    }
}

void SlotBar::playSequenceWithTimeout(SequencePlaybackInfo sequence, float timeout)
{
    m_countdown = timeout;
    m_sequence = sequence;
}

void SlotBar::randomizeSlots(int maxTypes, std::string machineType)
{
    for (int i = 0; i < 3; i++)
    {
        int slotImagePos = rand() % maxTypes + 1;
        std::stringstream ss;
        ss << slotImagePos << "_" << machineType;
        m_slots[i]->playSequence(ss.str(), true);
    }
}

void SlotBar::switchSlotType(int maxSlots)
{
    for (int i = 0; i < 3; i++)
    {
        int curFrame = m_slots[i]->getCurrentFrameIndex();
        const int maxFrame = m_slots[i]->getTotalFrameCount();
        curFrame += maxSlots;
        if (curFrame > maxFrame)
        {
            curFrame = curFrame % maxSlots;
        }

        m_slots[i]->clearSequence();
        m_slots[i]->gotoAndStop(curFrame);
    }
}

void SlotBar::showSpinResult(SlotMachine::PrizeBar_t fruits, std::string machineType)
{
    for (int i = 0; i < 3; i++)
    {
        std::stringstream ss;
        ss << fruits[i] + 1 << "_" << machineType;
        m_slots[i]->playSequence(ss.str(), true);
    }
}

GAFObject* SlotBar::getBar()
{
    return m_bar;
}