//
// Created by JAI PRAKASH GAUR on 03/06/2026.
//

#include "Obstacle.h"
#include "Constants.h"
#include <random>

auto GenerateRandomHeight() -> float
{
    // 1. Seed the random number engine
    std::random_device rd;
    std::mt19937 gen(rd());  // Mersenne Twister engine

    // 2. Define the distribution bounds [inclusive]
    std::uniform_real_distribution<float> distrib(OBSTACLE_MIN_HEIGHT, OBSTACLE_MAX_HEIGHT);

    // 3. Generate the number
    return distrib(gen);
}

bool Obstacle::init() {
    if (!Node::init()) return false;

    const auto drawNode = ax::DrawNode::create();
    addChild(drawNode);

    // Draw relative to the object's internal (0,0) origin
    constexpr auto bottomLeft = ax::Vec2(0,0);
    const auto topRight = ax::Vec2(OBSTACLE_WIDTH,GenerateRandomHeight());

    drawNode->drawSolidRect(bottomLeft, topRight, ax::Color::MAGENTA);

    setContentSize(topRight);

    return true;
}

