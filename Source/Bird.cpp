//
// Created by JAI PRAKASH GAUR on 03/06/2026.
//

#include "Bird.h"
#include "Constants.h"


bool Bird::init()
{
    birdSprite = ax::Sprite::create("Bird.png"sv);

    if (!birdSprite)
    {
        printf("Error while loading: 'Bird.png'\n");
        printf("Depending on how you compiled you might have to add 'Content/' in front of filenames in " "Bird.cpp\n");
    }
    else
    {
        const auto originalSize = birdSprite->getContentSize();

        //  Calculating the scale factors needed to fit exactly 80x80 pixels
        const float scaleX = 80.0f / originalSize.width;
        const float scaleY = 80.0f / originalSize.height;

        //  Apply the scale factor to the sprite image
        birdSprite->setScale(scaleX, scaleY);

        birdSprite->setPosition(ax::Vec2(0,0));

        // Add the sprite to this Bird node container
        addChild(birdSprite);
    }

    return true;
}

ax::Sprite* Bird::getSprite() const
{
    return birdSprite;
}

void Bird::gravityFall(const float delta) const
{
    birdSprite->setRotation(birdSprite->getRotation() + (BIRD_ROTATION_PER_SECOND * delta));

    birdSprite->setPositionY(birdSprite->getPositionY() - ( delta * BIRD_FALL_SPEED ));
}

void Bird::jump() const
{
    birdSprite->setRotation(MAX_DEGREES - BIRD_ROTATION_PER_SECOND);

    birdSprite->setPositionY(birdSprite->getPositionY() + BIRD_JUMP_SPEED);
}
