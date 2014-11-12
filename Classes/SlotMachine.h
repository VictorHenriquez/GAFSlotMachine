#include "cocos2d.h"
#include "GAF.h"
#include "SlotBar.h"

class SlotMachine : public cocos2d::Ref
{
    enum class EMachineState : uint16_t
    {
        Initial = 0,
        ArmTouched,
        Spin,
        SpinEnd,
        Win,
        End,

        COUNT
    };

public:
    static SlotMachine* create(gaf::GAFObject* mainObject);

    SlotMachine();
    ~SlotMachine();

    bool init(gaf::GAFObject* mainObject);
    void update(float dt);

    void start();

    void onFinishSequence(gaf::GAFObject* object);

private:
    gaf::GAFObject* m_arm;
    gaf::GAFObject* m_whiteBG;
    SlotBar* m_bars[3];

    EMachineState m_state;

    void nextState();

    float m_countdown;
};