/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md). https://axmol.dev/
 Copyright (c) 2026-present [for Custimise code] JAI PRAKASH GAUR

 Only Read-Only Permission is hereby granted for this demo project.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

 Bird Image asset from : https://opengameart.org/content/free-game-asset-grumpy-flappy-bird-sprite-sheets
 ****************************************************************************/

#include "MainScene.h"
#include "Constants.h"

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Content/' in front of filenames in MainScene.cpp\n");
}

MainScene::~MainScene()
{
    if (m_keyboardListener)
        _eventDispatcher->removeEventListener(m_keyboardListener);
    if (m_mouseListener)
        _eventDispatcher->removeEventListener(m_mouseListener);
}

bool MainScene::init()
{
    // 1. super init first
    if (!Scene::init()) return false;

    const auto visibleSize = _director->getVisibleSize();
    const auto origin      = _director->getVisibleOrigin();
    const auto safeArea    = _director->getSafeAreaRect();
    const auto safeOrigin  = safeArea.origin;

    // 1. add a menu item with "X" image, which is clicked to quit the program
    // add a "close" icon to exit the progress. it's an autorelease object
    const auto closeItem = ax::MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
                                           AX_CALLBACK_1(MainScene::menuCloseCallback, this));

    if (!closeItem || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        const float x = safeOrigin.x + safeArea.size.width - closeItem->getContentSize().width / 2;
        const float y = safeOrigin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(ax::Vec2(x, y));
    }

    // create menu, it's an autorelease object
    const auto exitGame = ax::Menu::create(closeItem, NULL);
    exitGame->setPosition(ax::Vec2::ZERO);
    addChild(exitGame);

    // 2. Adding Scene Title
    auto sceneTitle = ax::Label::createWithSystemFont("Flappy Bird", "fonts/Marker Felt.ttf", 48);
    if (!sceneTitle) problemLoading("'fonts/Marker Felt.ttf'");
    else
    {
        // position the label at the center of the screen
        sceneTitle->setPosition(ax::Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - sceneTitle->getContentSize().height));
        sceneTitle->setTextColor(ax::Color32::MAGENTA);

        // add the label as a child to this layer
        addChild(sceneTitle);
    }

    // 3. Adding Axmol Logo
    auto axmolLogo = ax::Sprite::create("AxmolLogo.png"sv);
    if (!axmolLogo) problemLoading("'AxmolLogo.png'");
    else
    {
        // position the axmolLogo on the top left of the screen
        axmolLogo->setPosition(ax::Vec2(origin.x + (axmolLogo->getContentSize().width / 2),
             origin.y + visibleSize.height - (axmolLogo->getContentSize().height / 2) ));

        // add the sprite as a child to this layer
        addChild(axmolLogo);
    }

    // 4. Creating the Bird actor at middle of the screen
    m_bird = Bird::create();
    m_bird->setPosition(ax::Vec2(safeArea.getMidX(), safeArea.getMidY()));
    addChild(m_bird);

    // 5. Adding Event Listeners
    m_keyboardListener                = ax::EventListenerKeyboard::create();
    m_keyboardListener->onKeyReleased = AX_CALLBACK_2(MainScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithFixedPriority(m_keyboardListener, 11);

    // 6. Setting Labels for Score and Mouse move coordinates (debug-purpose)
    setupMouseMoveCoordinates(safeArea);
    setupScoreLabel(safeArea);

    // scheduleUpdate() is required to ensure update(float) is called on every loop
    scheduleUpdate();

    return true;
}

void MainScene::menuCloseCallback(ax::Object*) const
{
    // Close the axmol game scene and quit the application
    _director->end();
}

void MainScene::setupScoreLabel(const ax::Rect& safeArea)
{
    m_scoreLabel = ax::Label::createWithSystemFont("Score: 0", "fonts/Marker Felt.ttf", 24);
    addChild(m_scoreLabel);

    m_scoreLabel->setPosition(ax::Vec2(safeArea.getMaxX() - 80, safeArea.getMaxY() - m_mouseCoordinatesLabel->getHeight() - 80));
    m_scoreLabel->setTextColor(ax::Color32::MAGENTA);
    m_scoreLabel->setHorizontalAlignment(ax::TextHAlignment::CENTER);
}


void MainScene::setupMouseMoveCoordinates(const ax::Rect& safeArea)
{
    // 1. Create a persistent text label to display mouse move coordinates
    m_mouseCoordinatesLabel = ax::Label::createWithSystemFont("X: 0.0, Y: 0.0", "fonts/Marker Felt.ttf", 24);
    m_mouseCoordinatesLabel->setAnchorPoint(ax::Vec2(0.0f, 1.0f));  // Anchor to top-left of the text box
    m_mouseCoordinatesLabel->setPosition(ax::Vec2(safeArea.getMaxX() - 150, safeArea.getMaxY() - 16));  // Top-right of screen
    m_mouseCoordinatesLabel->setColor(ax::Color32::YELLOW);
    addChild(m_mouseCoordinatesLabel, 999);  // 999 Ensure it renders on top of everything

    // 2. Set up the mouse event listener
    m_mouseListener = ax::EventListenerMouse::create();

    // 3. Bind the mouse movement macro/lambda
    m_mouseListener->onMouseMove = AX_CALLBACK_1(MainScene::onMouseMove, this);

    // 4. Dispatch the listener to the engine's event system
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_mouseListener, this);
}

bool MainScene::onMouseMove(ax::Event* event) const
{
    if (const auto* m_event = dynamic_cast<ax::EventMouse*>(event); m_event && m_mouseCoordinatesLabel) {
        const std::string coordString = fmt::format("X: {:.0f}, Y: {:.0f}", m_event->getLocation().x, m_event->getLocation().y);
        m_mouseCoordinatesLabel->setString(coordString);
    }
    return true;
}

void MainScene::onKeyReleased(const ax::EventKeyboard::KeyCode code, ax::Event* event) const
{
    if (code == ax::EventKeyboard::KeyCode::KEY_SPACE)
    {
        m_bird->jump();
    }
}

void MainScene::update(const float delta)
{
    switch (m_gameState)
    {
        case GameState::init:
        {
            m_gameState = GameState::update;
            break;
        }

        case GameState::update:
        {
            m_bird->gravityFall(delta);

            manageObstacles(delta);

            if (detectCollisions()) m_gameState = GameState::pause;

            break;
        }

        case GameState::pause:
        {
            break;
        }

        case GameState::end:
        {
            menuCloseCallback(this);
            break;
        }
    }
}

auto MainScene::detectCollisions() -> bool
{
    // Get the absolute position on screen, of the bird's center point
    ax::Vec2 birdCenter(m_bird->getSprite()->getPositionX(), m_bird->getSprite()->getPositionY());
    birdCenter = m_bird->getSprite()->getParent()->convertToWorldSpace(birdCenter);

    for (const auto& obstacle: m_obstacles)
    {
        // Get the absolute screen-space bounding box of the obstacle pipe
        ax::Rect obstacleBox = getWorldBoundingBox(obstacle);

        // Check if the bird's center point is within the bottom-left to top-right range of the pipe
        if (obstacleBox.containsPoint(birdCenter))
        {
            AXLOGD("bird.x{:.0f} Bird.y{:.0f}", birdCenter.x, birdCenter.y);
            AXLOGD("x{:.0f} top.x{:.0f}, y{:.0f} topy{:.0f}", obstacleBox.origin.x, obstacleBox.origin.x + obstacleBox.size.width, obstacleBox.origin.y, obstacleBox.origin.y + obstacleBox.size.height);
            AXLOGD("COLLISION DETECTED!");
            return true;
        }
    }
    return false;
}

void MainScene::manageObstacles(const float delta)
{
    // Position the obstacle start point at most right at the edge of the screen 1280
    const float spawnX = _director->getSafeAreaRect().getMaxX();

    bool shouldSpawn = false;
    if ( m_obstacles.empty() || ( m_obstacles.size() < TOTAL_OBSTACLES &&
        m_obstacles.back()->getPositionX() < spawnX - OBSTACLE_GAP ) )
    {
        shouldSpawn = true;
    }

    if (shouldSpawn)
    {
        // Safe instantiation through the engine's factory wrapper
        const auto obstacle = Obstacle::create();

        // Position the obstacle container right at the edge of the screen
        obstacle->setPosition(ax::Vec2(spawnX, 100));

        addChild(obstacle);

        m_obstacles.pushBack(obstacle);
    }

    // Moving the obstacles towards the left of screen
    for (const auto& obstacle: m_obstacles)
    {
        const auto new_x = obstacle->getPositionX() - (OBSTACLE_SPEED * delta);
        obstacle->setPositionX(new_x);
    }

    // Cleaning up the obstacle object which are gone left of the screen
    while (!m_obstacles.empty() && m_obstacles.front()->getPositionX() + OBSTACLE_WIDTH < 0)
    {
        // Using this as success for Bird to avoid the Obstacle and increasing Score
        m_score += 7;
        const std::string scoreString = fmt::format("Score: {}", m_score);
        m_scoreLabel->setString(scoreString);

        removeChild(m_obstacles.front());
        m_obstacles.erase(0);
    }
}

ax::Rect MainScene::getWorldBoundingBox(ax::Node* node)
{
    // 1. Get the local bounding box
    const ax::Rect localRect = node->getBoundingBox();

    // 2. Convert its local (0,0) origin point into absolute screen space
    const ax::Vec2 worldOrigin = node->getParent()->convertToWorldSpace(localRect.origin);

    // 3. Return a new Rect completely mapped to screen space
    return {worldOrigin.x, worldOrigin.y, localRect.size.width, localRect.size.height};
}
