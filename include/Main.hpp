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

/**
 * Main setUp method
 */
void setup();

/**
 * Main loop method
 */
void loop();

/**
 * Main shutdown method
 */
void shutdown();

/**
 * Put device in sleep mode
 */
void sleep();

/**
 * Wakes up device
 */
void wakeUp();

/**
 * Sets up alwaysLoop = true features
 */
void setUpPermanentFeatures();

/**
 * Deletes all alwaysLoop = true fatures
 */
void shutdownPermanentFeatures();

/**
 * Deletes all feature factories
 */
void shutdownFeatureFactories();

/**
 * Increments feature index until featureFactories.size() than reinitialise it at 0
 */
void nextFeature();

/**
 * Sets up all buttons
 */
void setUpButtons();

/**
 * Tick all buttons
 */
void tickButtons();

/**
 * Called on home button click
 */
void onHomeButtonClick();

/**
 * Called on home button double click
 */
void onHomeButtonDoubleClick();

/**
 * Called on up button click
 */
void onUpButtonClick();

/**
 * Called on up button double click
 */
void onUpButtonDoubleClick();

/**
 * Called on power button click
 */
void onPowerButtonClick();

/**
 * Called on power button long click
 */
void onPowerButtonLongClick();

#endif
