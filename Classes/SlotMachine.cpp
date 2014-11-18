#include "SlotMachine.h"
#include "SlotBar.h"

USING_NS_GAF;

const std::string SlotMachine::s_rewardCoins = "coins";
const std::string SlotMachine::s_rewardChips = "chips";
const int SlotMachine::s_fruitCount = 5;
const float SlotMachine::s_barTimeout = 0.2f;

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
    , m_rewardType(s_rewardChips)
{
    srand(time(nullptr));
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
    m_winFrame = obj->getObjectByName("frame");
    m_spinningRays = obj->getObjectByName("spinning_rays");

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

        m_bars[i]->randomizeSlots(s_fruitCount, m_rewardType);
    }

    defaultPlacing();

    return true;
}

void SlotMachine::onFinishSequence(gaf::GAFObject* object)
{
    nextState();
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

    for (int i = 0; i < 3; i++)
    {
        m_bars[i]->update(dt);
    }
}

void SlotMachine::start()
{
    if (m_state == EMachineState::Initial)
    {
        nextState();
    }
}

void SlotMachine::switchType()
{
    if (m_rewardType.compare(s_rewardChips) == 0)
    {
        m_rewardType = s_rewardCoins;
    }
    else if (m_rewardType.compare(s_rewardCoins) == 0)
    {
        m_rewardType = s_rewardChips;
    }

    int state = static_cast<int>(m_state) - 1;
    if (state < 0)
    {
        state = static_cast<int>(EMachineState::COUNT) - 1;
    }
    m_state = static_cast<EMachineState>(state);
    nextState();

    for (int i = 0; i < 3; i++)
    {
        m_bars[i]->switchSlotType(s_fruitCount);
    }
}

void SlotMachine::defaultPlacing()
{
    m_whiteBG->gotoAndStop("whiteenter");
    m_winFrame->playSequence("stop");
    m_arm->playSequence("stop");
    m_bottomCoins->setVisible(false);
    m_bottomCoins->setLooped(false);
    m_rewardText->playSequence("notwin", true);
    //m_spinningRays->playSequence("spin", true);

    for (int i = 0; i < 3; i++)
    {
        m_centralCoins[i]->setVisible(false);
    }
    for (int i = 0; i < 3; i++)
    {
        m_bars[i]->getBar()->playSequence("statics");
        //m_bars[i]->randomizeSlots(s_fruitCount, m_rewardType);
    }
}

void SlotMachine::nextState()
{
    m_state = static_cast<EMachineState>(static_cast<uint16_t>(m_state) + 1);
    if (m_state == EMachineState::COUNT)
    {
        m_state = static_cast<EMachineState>(0);
    }

    resetCallbacks();

    switch (m_state)
    {
    case EMachineState::Initial:
        defaultPlacing();
        break;

    case EMachineState::ArmTouched:
        m_arm->playSequence("push");
        m_arm->setAnimationFinishedPlayDelegate(GAFAnimationStartedNextLoopDelegate_t(CC_CALLBACK_1(SlotMachine::onFinishSequence, this)));
        break;

    case EMachineState::Spin:
        m_arm->playSequence("stop");

        for (int i = 0; i < 3; i++)
        {
            std::stringstream ss;
            ss << "rotation_" << m_rewardType;
            SlotBar::SequencePlaybackInfo sequence(ss.str(), true);
            m_bars[i]->playSequenceWithTimeout(sequence, s_barTimeout * i);
        }

        m_countdown = 3.0f;

        break;

    case EMachineState::SpinEnd:
        {
            m_prize = generatePrize();
            PrizeMatrix_t spinResult = generateSpinResult(m_prize);
            for (int i = 0; i < 3; i++)
            {
                m_bars[i]->showSpinResult(spinResult[i], m_rewardType);
                SlotBar::SequencePlaybackInfo sequence("stop", false);
                m_bars[i]->playSequenceWithTimeout(sequence, s_barTimeout * i);
            }
            m_countdown = s_barTimeout * 4;
        }
        break;

    case EMachineState::Win:
        showPrize(m_prize);
        break;

    case EMachineState::End:
        m_whiteBG->resumeAnimation();
        m_whiteBG->setAnimationStartedNextLoopDelegate(GAFAnimationStartedNextLoopDelegate_t(CC_CALLBACK_1(SlotMachine::onFinishSequence, this)));
        break;

    default:
        break;
    }
}

void SlotMachine::resetCallbacks()
{
    m_whiteBG->setAnimationStartedNextLoopDelegate(nullptr);
    m_arm->setAnimationFinishedPlayDelegate(nullptr);
    m_countdown = -1.0f;
}

SlotMachine::EPrize SlotMachine::generatePrize()
{
    int count = static_cast<int>(EPrize::COUNT);
    int prize = rand() % count;
    return static_cast<EPrize>(prize);
}

SlotMachine::PrizeMatrix_t SlotMachine::generateSpinResult(EPrize prize)
{
    PrizeMatrix_t result(3, PrizeBar_t(3));

    for (int i = 0; i < 3; i++)
    {
        result[i][0] = rand() % s_fruitCount;
        result[i][2] = rand() % s_fruitCount;
    }

    int centralFruit;
    switch (prize)
    {
    case EPrize::None:
        centralFruit = rand() % s_fruitCount;
        break;
    case EPrize::C1k:
        centralFruit = rand() % (s_fruitCount / 2);
        break;
    case EPrize::C500k:
        centralFruit = rand() % (s_fruitCount / 2) + s_fruitCount / 2;
        break;
    case EPrize::C1000k:
        centralFruit = s_fruitCount - 1;
        break;
    default:
        break;
    }

    if (prize == EPrize::None)
    {
        result[0][1] = centralFruit;
        result[1][1] = centralFruit;
        result[2][1] = centralFruit;
        while (result[2][1] == result[1][1])
        {
            result[2][1] = rand() % s_fruitCount; // last fruit should be another
        }
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            result[i][1] = centralFruit;
        }
    }

    return result;
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

    m_winFrame->playSequence("win", true);
    m_rewardText->playSequence(getTextByPrize(prize), true);

    int idx = static_cast<int>(prize)-1;
    m_centralCoins[idx]->setVisible(true);
    m_centralCoins[idx]->playSequence(m_rewardType, true);

    m_countdown = 5.0f;
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

GAFObject* SlotMachine::getArm()
{
    return m_arm;
}