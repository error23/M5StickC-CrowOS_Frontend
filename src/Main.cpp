/**
 * CrowOs Main implementation
 * @author error23
 */
#include "Main.hpp"

using namespace CrowOs::Core;

/** Home button */
OneButton homeButton(BUTTON_A_PIN, true);

/** Up button */
OneButton upButton(BUTTON_B_PIN, true);

/** Time helper */
Time timeHelper(60, 25);

/** Led helper */
Led ledHelper;

/** Screen helper */
Screen screenHelper;

/** List of permanent features **/
std::vector<Feature*> permanentFeatures;

/** Current feature */
Feature* currentFeature = NULL;

/** Current feature index */
int currentFeatureIndex = -1;

/** New feature index */
int newFeatureIndex = 0;

/** Indicates if device is sleeping */
bool sleeping = false;

/**
 * Main setUp method
 */
void setup() {

	M5.begin();
	Serial.begin(115200);
	M5.Axp.begin();
	M5.Lcd.setSwapBytes(true);
	screenHelper.setUp();
	screenHelper.showLogo();

	timeHelper.setUp();
	ledHelper.setUp();
	setUpButtons();

	initialiseFeatureFactories();
	setUpPermanentFeatures();

	if(LOG_INFO) Serial.println("Info : [Main] Setup Done");
}

/**
 * Main loop method
 */
void loop() {

	timeHelper.limitFps();
	tickButtons();

	if(timeHelper.shouldSleep()) {
		sleep();
	}
	else {

		// loop permanent features
		for(auto& feature : permanentFeatures) {
			feature->loop();
		}

		// if there is any features implemented
		if(FeatureFactory::featureFactories.size() != 0) {

			// switch normal feature if index has changed
			if(newFeatureIndex != currentFeatureIndex) {

				// kill current feature if exists
				if(currentFeature != NULL) {
					currentFeature->onStop(&FeatureFactory::featureFactories[currentFeatureIndex].second);
					delete currentFeature;
				}

				// start new feature
				currentFeature = FeatureFactory::featureFactories[newFeatureIndex].first->createFeature();
				currentFeature->onStart(&screenHelper, &timeHelper, &ledHelper, &FeatureFactory::featureFactories[newFeatureIndex].second);
				currentFeatureIndex = newFeatureIndex;
			}

			// loop current feature if exists
			if(currentFeature != NULL) currentFeature->loop();
		}
	}
}

/**
 * Main shutdown method
 */
void shutdown() {

	delete currentFeature;
	shutdownPermanentFeatures();
	shutdownFeatureFactories();

	if(LOG_INFO) Serial.println("Info : [Main] shutdown Done");
}

/**
 * Put device in sleep mode
 */
void sleep() {

	if(LOG_INFO) Serial.println("Info : [Main] sleep");
	if(sleeping) return; // TODO: if it works delay

	sleeping = true;
	M5.Axp.SetSleep();
}

/**
 * Wakes up device
 */
void wakeUp() {

	if(!sleeping) return;
	if(LOG_INFO) Serial.println("Info : [Main] wakeUp");

	timeHelper.keepWokedUp();
	M5.Axp.begin();
	M5.Lcd.setSwapBytes(true);

	sleeping = false;
}

/**
 * Sets up alwaysLoop = true features
 */
void setUpPermanentFeatures() {

	for(auto& featureFactorySavedDataPair : FeatureFactory::featureFactories) {
		if(featureFactorySavedDataPair.first->isAlwaysLoop()) {

			Feature* permanentFeature = featureFactorySavedDataPair.first->createFeature();
			delete featureFactorySavedDataPair.first;
			permanentFeature->onStart(&screenHelper, &timeHelper, &ledHelper, NULL);
			permanentFeatures.push_back(permanentFeature);

			if(LOG_DEBUG) Serial.printf("Debug : [Main] setUpPermanentFeatures %s added to permanentFeatures actualSize = %d\n", permanentFeature->getFeatureName(), permanentFeatures.size());
		}
	}
}

/**
 * Deletes all alwaysLoop = true fatures
 */
void shutdownPermanentFeatures() {

	for(auto& permanentFeature : permanentFeatures) {
		delete permanentFeature;
	}
	if(LOG_INFO) Serial.println("Info : [Main] shutdownPermanentFeatures Done");
}

/**
 * Deletes all feature factories
 */
void shutdownFeatureFactories() {

	for(auto& featureFactorySavedDataPair : FeatureFactory::featureFactories) {
		if(featureFactorySavedDataPair.first != NULL) delete featureFactorySavedDataPair.first;
	}

	if(LOG_INFO) Serial.println("Info : [Main] shutdownFeatureFactories Done");
}

/**
 * Increments feature index until featureFactories.size() than reinitialise it at 0
 */
void nextFeature() {

	if(LOG_DEBUG) {
		Serial.printf("Debug : [Main] nextFeature currentFeatureIndex = %d\n", currentFeatureIndex);
		Serial.printf("Debug : [Main] nextFeature newFeatureIndex = %d\n", newFeatureIndex);
		Serial.printf("Debug : [Main] nextFeature featureFactories.size() = %d\n", FeatureFactory::featureFactories.size());
	}

	newFeatureIndex++;
	if(newFeatureIndex >= FeatureFactory::featureFactories.size()) {
		newFeatureIndex = 0;
	}

	if(LOG_DEBUG) {
		Serial.printf("Debug : [Main] nextFeature newFeatureIndex = %d\n", newFeatureIndex);
		Serial.printf("Debug : [Main] nextFeature featureFactories.size() = %d\n", FeatureFactory::featureFactories.size());
	}
}

/**
 * Sets up all buttons
 */
void setUpButtons() {

	homeButton.attachClick(onHomeButtonClick);
	homeButton.attachDoubleClick(onHomeButtonDoubleClick);
	homeButton.setDebounceTicks(DEBOUNCE_TICKS);

	upButton.attachClick(onUpButtonClick);
	upButton.attachDoubleClick(onUpButtonDoubleClick);
	upButton.setDebounceTicks(DEBOUNCE_TICKS);

	if(LOG_DEBUG) Serial.printf("Debug : [Main] setUpButtons DEBOUNCE_TICKS : %d\n", DEBOUNCE_TICKS);
	if(LOG_INFO) Serial.println("Info : [Main] setUpButtons Done");
}

/**
 * Tick all buttons
 */
void tickButtons() {

	if(LOG_DEBUG) Serial.println("Debug : [Main] tickButtons");
	homeButton.tick();
	upButton.tick();

	if(M5.Axp.GetBtnPress() == 0x2) {
		onPowerButtonLongClick();
	}
	else if(M5.Axp.GetBtnPress()) {
		onPowerButtonClick();
	}
}

/**
 * Called on home button click
 */
void onHomeButtonClick() {

	if(LOG_DEBUG) Serial.println("Debug : [Main] onHomeClick general");
	if(sleeping) return;
	timeHelper.keepWokedUp();
	if(currentFeature != NULL) currentFeature->onHomeClick();
}

/**
 * Called on home button double click
 */
void onHomeButtonDoubleClick() {

	if(LOG_DEBUG) Serial.println("Debug : [Main] onHomeDoubleClick general");
	if(sleeping) return;
	timeHelper.keepWokedUp();
	if(currentFeature != NULL) currentFeature->onHomeDoubleClick();
}

/**
 * Called on up button click
 */
void onUpButtonClick() {

	if(LOG_DEBUG) Serial.println("Debug : [Main] onUpButtonClick general");
	if(sleeping) return;
	timeHelper.keepWokedUp();
	nextFeature();
}

/**
 * Called on up button double click
 */
void onUpButtonDoubleClick() {

	if(LOG_DEBUG) Serial.println("Debug : [Main] onUpButtonDoubleClick general");
	if(sleeping) return;
	timeHelper.keepWokedUp();
	screenHelper.changeBrightness();
}

/**
 * Called on power button click
 */
void onPowerButtonClick() {

	if(LOG_DEBUG) Serial.println("Debug : [Main] onPowerButtonClick general");

	if(sleeping) {
		wakeUp();
	}
	else {
		sleep();
	}
}

/**
 * Called on power button long click
 */
void onPowerButtonLongClick() {

	if(LOG_DEBUG) Serial.println("Debug : [Main] onPowerButtonLongClick general");
	if(sleeping) return;
	timeHelper.keepWokedUp();
	shutdown();
}
