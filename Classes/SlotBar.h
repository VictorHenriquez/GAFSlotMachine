#include "cocos2d.h"
#include "GAF.h"

class SlotBar
{
public:
    SlotBar(gaf::GAFObject* mainObject);

private:
    gaf::GAFObject* m_slots[3];
};