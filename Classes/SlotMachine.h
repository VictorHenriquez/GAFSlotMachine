#include "cocos2d.h"
#include "GAF.h"
#include "SlotBar.h"

class SlotMachine
{
    enum class EMachineState : uint16_t
    {
        Initial = 0,
        ArmTouched,
        Spin,
        Win,
        End,

        COUNT
    };

public:
    SlotMachine(gaf::GAFObject* mainObject);
    ~SlotMachine();

    void update(float dt);

    void start();

    void onFinishSequence(gaf::GAFObject* object);

private:
    gaf::GAFObject* m_arm;
    gaf::GAFObject* m_whiteBG;
    SlotBar* m_bars[3];

    EMachineState m_state;

    void nextState();
};