#include "SlotMachine.h"

USING_NS_GAF;

const std::string SlotMachine::s_rewardCoins = "coins";
const std::string SlotMachine::s_rewardChips = "chips";

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
    , m_rewardType(s_rewardCoins)
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
    m_whiteBG = obj->getObjectByName("white_exit");
    m_bottomCoins = obj->getObjectByName("wincoins");
    m_rewardText = obj->getObjectByName("wintext");

    for (int i = 0; i < 3; i++)
    {
        EPrize prize = static_cast<EPrize>(i + 1);
        m_centralCoins[i] = obj->getObjectByName(getTextByPrize(prize));
    }

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

    defaultPlacing();

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
    if (m_state == EMachineState::Initial)
    {
        nextState();
    }
}

void SlotMachine::defaultPlacing()
{
    m_whiteBG->gotoAndStop("whiteenter");
    m_bottomCoins->setVisible(false);
    m_rewardText->setVisible(false);
    for (int i = 0; i < 3; i++)
    {
        m_centralCoins[i]->setVisible(false);
    }
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
            std::stringstream ss;
            ss << "rotation_" << m_rewardType;
            m_bars[i]->getBar()->playSequence(ss.str(), true); // TODO: timegaps between starts
        }

        m_countdown = 5.0f;

        break;

    case EMachineState::SpinEnd:
        for (int i = 0; i < 3; i++)
        {
            m_bars[i]->getBar()->playSequence("stop");
        }
        m_bars[2]->getBar()->setAnimationFinishedPlayDelegate(GAFAnimationStartedNextLoopDelegate_t(CC_CALLBACK_1(SlotMachine::onFinishSequence, this)));
        break;

    case EMachineState::Win:
        m_bars[2]->getBar()->setAnimationFinishedPlayDelegate(nullptr);
        showPrize(getPrize());
        break;

    default:
        break;
    }
}

SlotMachine::EPrize SlotMachine::getPrize()
{
    return EPrize::C500k;
}

void SlotMachine::showPrize(EPrize prize)
{
    std::string coinsBottomState = getTextByPrize(prize);
    coinsBottomState.append("_");
    coinsBottomState.append(m_rewardType);
    m_bottomCoins->setVisible(true);
    m_bottomCoins->gotoAndStop(coinsBottomState);

    if (prize == EPrize::None)
    {
        nextState();
        return;
    }

    m_rewardText->setVisible(true);
    m_rewardText->gotoAndStop(getTextByPrize(prize));

    switch (prize)
    {
    case EPrize::C1k:

        break;

    case EPrize::C500k:
        break;

    case EPrize::C1000k:
        break;

    default:
        break;
    }
}

std::string SlotMachine::getTextByPrize(EPrize prize)
{
    switch (prize)
    {
    case EPrize::None:
        return "notwin";

    case EPrize::C1k:
        return "win1k";

    case EPrize::C500k:
        return "win500k";

    case EPrize::C1000k:
        return "win1000k";

    default:
        return "";
    }
}