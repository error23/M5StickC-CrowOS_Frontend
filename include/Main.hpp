#ifndef MAIN_H
#define MAIN_H

// Lib includes
#include "M5StickC.h"
#include "OneButton.h"

// local Includes
#include "core/Defines.hpp"
#include "core/Feature.hpp"
#include "core/FeatureFactory.hpp"
#include "core/Led.hpp"
#include "core/Screen.hpp"
#include "core/Time.hpp"

// Methods

/**
 * Main setUp method
 */
void setup();

/**
 * Main loop method
 */
void loop();

/**
 * Sets up alwaysLoop = true features
 */
void setUpPermanentFeatures();

/**
 * Sets up all buttons from OneButton.h
 */
void setUpButtons();

/**
 * Tick all buttons from OneButton.h
 */
void tickButtons();

/**
 * Called on home button click
 */
void onHomeClick();

/**
 * Called on home button double click
 * @method onHomeDoubleClick
 */
void onHomeDoubleClick();

/**
 * Called on up button click
 * @method onRstClick
 */
void onUpButtonClick();

/**
 * Called on up button double click
 */
void onUpButtonDoubleClick();

/**
 * Increments feature index until featureFactories.size() than reinitialise it at 0
 */
void nextFeature();

#endif
