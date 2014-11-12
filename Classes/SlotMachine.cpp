#include "SlotMachine.h"

USING_NS_GAF;

SlotMachine::SlotMachine(GAFObject* mainObject)
    : m_state(EMachineState::Initial)
{
    auto obj = mainObject->getObjectByName("obj");

    m_arm = obj->getObjectByName("arm");

    m_whiteBG = obj->getObjectByName("white_movie");
    for (int i = 0; i < 3; i++)
    {
        std::stringstream ss;
        ss << "slot" << (i + 1);

        m_bars[i] = new SlotBar(obj->getObjectByName(ss.str()));
    }
}

SlotMachine::~SlotMachine()
{
    for (int i = 0; i < 3; i++)
    {
        delete m_bars[i];
    }
}

void SlotMachine::onFinishSequence(gaf::GAFObject* object)
{
    switch (m_state)
    {
    case EMachineState::ArmTouched:
        m_arm->setAnimationFinishedPlayDelegate(nullptr);
        m_state = EMachineState::Spin;
        m_arm->playSequence("stop", true);
        break;

    default:
        break;
    }
}

void SlotMachine::start()
{
    m_state = EMachineState::ArmTouched;

    m_arm->playSequence("push");
    m_arm->setAnimationFinishedPlayDelegate(GAFAnimationStartedNextLoopDelegate_t(CC_CALLBACK_1(SlotMachine::onFinishSequence, this)));
}