#include "SlotMachine.h"

USING_NS_GAF;

SlotMachine* SlotMachine::create(GAFObject* mainObject)
{
    SlotMachine* ret = new SlotMachine();
    if (ret && ret->init(mainObject))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_RELEASE(ret);
    return nullptr;
}

SlotMachine::SlotMachine()
    : m_state(EMachineState::Initial)
    , m_countdown(-1.0f)
{
}

SlotMachine::~SlotMachine()
{
}

bool SlotMachine::init(GAFObject* mainObject)
{
    if (mainObject == nullptr)
        return false;

    auto obj = mainObject->getObjectByName("obj");

    m_arm = obj->getObjectByName("arm");

    m_whiteBG = obj->getObjectByName("white_movie");
    for (int i = 0; i < 3; i++)
    {
        std::stringstream ss;
        ss << "slot" << (i + 1);

        m_bars[i] = SlotBar::create(obj->getObjectByName(ss.str()));
        if (m_bars[i])
        {
            m_bars[i]->retain();
        }
    }

    return true;
}

void SlotMachine::onFinishSequence(gaf::GAFObject* object)
{
    nextState();
    switch (m_state)
    {
    case EMachineState::ArmTouched:

        break;

    default:
        break;
    }
}

void SlotMachine::update(float dt)
{
    if (m_countdown >= 0.0f)
    {
        m_countdown -= dt;
        if (m_countdown < 0.0f)
        {
            nextState();
        }
    }
}

void SlotMachine::start()
{
    nextState();    
}

void SlotMachine::nextState()
{
    m_state = static_cast<EMachineState>(static_cast<uint16_t>(m_state) + 1);

    switch (m_state)
    {
    case EMachineState::ArmTouched:
        m_arm->playSequence("push");
        m_arm->setAnimationFinishedPlayDelegate(GAFAnimationStartedNextLoopDelegate_t(CC_CALLBACK_1(SlotMachine::onFinishSequence, this)));
        break;

    case EMachineState::Spin:
        m_arm->setAnimationFinishedPlayDelegate(nullptr);
        m_arm->playSequence("stop", true);

        for (int i = 0; i < 3; i++)
        {
            m_bars[i]->getBar()->playSequence("rotation", true); // TODO: timegaps between starts
        }

        m_countdown = 5.0f;

        break;

    case EMachineState::SpinEnd:
        for (int i = 0; i < 3; i++)
        {
            m_bars[i]->getBar()->playSequence("stop");
        }

        break;

    default:
        break;
    }
}