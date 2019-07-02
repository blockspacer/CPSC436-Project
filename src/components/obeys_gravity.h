//
// Created by Kenneth William on 2019-02-02.
//

#ifndef PANDAEXPRESS_OBEYSGRAVITY_H
#define PANDAEXPRESS_OBEYSGRAVITY_H

/***
 *  Objects assigned this component will be affected by gravity, acceleration is computed in physics_system
 *  Overloaded constructor to allow for the creation of objects that fall at slower speeds than the panda
 */


struct ObeysGravity {
    float gravityFactor;

    explicit ObeysGravity(float gravityConstant) :
            gravityFactor(gravityConstant) {}

    ObeysGravity() :
            gravityFactor(1.f) {}
};

#endif //PANDAEXPRESS_OBEYSGRAVITY_H
