#include "SlotBar.h"

USING_NS_GAF;

SlotBar::SlotBar(GAFObject* mainObject)
{
    for (int i = 0; i < 3; i++)
    {
        std::stringstream ss;
        ss << "fruit" << (i + 1);
        m_slots[i] = mainObject->getObjectByName(ss.str());
    }
}