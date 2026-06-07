//
// Created by JAI PRAKASH GAUR on 03/06/2026.
//

#pragma once

#include "axmol/axmol.h"


class Obstacle : public ax::Node {
public:
    // This macro automatically declares a static create() method and initializes memory management
    CREATE_FUNC(Obstacle);

    bool init() override;

};