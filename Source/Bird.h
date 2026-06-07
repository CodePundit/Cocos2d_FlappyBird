//
// Created by JAI PRAKASH GAUR on 03/06/2026.
//

#pragma once

#include "axmol/axmol.h"


class Bird : public ax::Node {
public:
    // This macro automatically declares a static create() method and initializes memory management
    CREATE_FUNC(Bird);

    bool init() override;
    ax::Sprite* getSprite() const;

    void gravityFall(float delta) const;
    void jump() const;

private:
    ax::Sprite* birdSprite{};
};