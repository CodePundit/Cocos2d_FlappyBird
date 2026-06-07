/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md). https://axmol.dev/
 Copyright (c) 2026-present [for Customized code] JAI PRAKASH GAUR

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

#pragma once

#include "Bird.h"
#include "Obstacle.h"

class MainScene : public ax::Scene
{
    enum class GameState
    {
        init = 0,
        update,
        pause,
        end
    };

public:

    bool init() override;

    void update(float delta) override;

    void manageObstacles(float delta);

    // mouse
    bool onMouseMove(ax::Event* event) const;

    // Keyboard
    void onKeyReleased(ax::EventKeyboard::KeyCode code, ax::Event* event) const;

    // a selector callback
    void menuCloseCallback(ax::Object* sender) const;

    MainScene() = default;
    ~MainScene() override;

private:
    static ax::Rect getWorldBoundingBox(ax::Node* node);

    void setupMouseMoveCoordinates(const ax::Rect& safeArea);
    void setupScoreLabel(const ax::Rect& safeArea);
    auto detectCollisions() -> bool;

    GameState m_gameState                            = GameState::init;
    ax::EventListenerKeyboard* m_keyboardListener    = nullptr;
    ax::EventListenerMouse* m_mouseListener          = nullptr;

    Bird* m_bird                                     = nullptr;
    ax::Label* m_mouseCoordinatesLabel               = nullptr;
    ax::Label* m_scoreLabel                          = nullptr;
    int m_score{};
    ax::Vector<Obstacle*> m_obstacles;
};
