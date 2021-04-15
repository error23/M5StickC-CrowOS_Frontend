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
	setUpPermanentFeatures(); // TODO : should be deleted
	if(LOG_INFO) Serial.println("Info : [Main] Setup Done");
}

/**
 * Main loop method
 */
void loop() {

	tickButtons();

	for(auto& feature : permanentFeatures) {
		feature->loop();
	}

	if(FeatureFactory::featureFactories.size() != 0) {
		if(newFeatureIndex != currentFeatureIndex) {

			if(currentFeature != NULL) {
				currentFeature->onStop(&FeatureFactory::featureFactories[currentFeatureIndex].second);
				delete currentFeature;
			}

			currentFeature = FeatureFactory::featureFactories[newFeatureIndex].first->createFeature();
			currentFeature->onStart(&screenHelper, &timeHelper, &ledHelper, &FeatureFactory::featureFactories[newFeatureIndex].second);
			currentFeatureIndex = newFeatureIndex;
		}
		currentFeature->loop();
	}

	timeHelper.limitFps();
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

	if(LOG_INFO) Serial.println("Info : [Main] setUpPermanentFeatures Done");
}

/**
 * Sets up all buttons click listeners
 */
void setUpButtons() {

	homeButton.attachClick(onHomeClick);
	homeButton.attachDoubleClick(onHomeDoubleClick);
	homeButton.setDebounceTicks(DEBOUNCE_TICKS);

	upButton.attachClick(onUpButtonClick);
	upButton.attachDoubleClick(onUpButtonDoubleClick);
	upButton.setDebounceTicks(DEBOUNCE_TICKS);

	if(LOG_DEBUG) Serial.printf("Debug : [Main] setUpButtons DEBOUNCE_TICKS : %d\n", DEBOUNCE_TICKS);
	if(LOG_INFO) Serial.println("Info : [Main] setUpButtons Done");
}

/**
 * Tick all buttons in order to generate click events
 */
void tickButtons() {

	if(LOG_DEBUG) Serial.println("Debug : [Main] tickButtons");
	homeButton.tick();
	upButton.tick();
}

/**
 * Called on home button click
 */
void onHomeClick() {

	if(LOG_DEBUG) Serial.println("Debug : [Main] onHomeClick general");
	if(currentFeature != NULL) currentFeature->onHomeClick();
}

/**
 * Called on home button double click
 * @method onHomeDoubleClick
 */
void onHomeDoubleClick() {

	if(LOG_DEBUG) Serial.println("Debug : [Main] onHomeDoubleClick general");

	if(currentFeature != NULL) currentFeature->onHomeDoubleClick();
}

/**
 * Called on up button click
 * @method onRstClick
 */
void onUpButtonClick() {

	if(LOG_DEBUG) Serial.println("Debug : [Main] onUpButtonClick general");

	if(LOG_DEBUG) {
		Serial.printf("Debug : [Main] onUpButtonClick currentFeatureIndex = %d\n", currentFeatureIndex);
		Serial.printf("Debug : [Main] onUpButtonClick newFeatureIndex = %d\n", newFeatureIndex);
		Serial.printf("Debug : [Main] onUpButtonClick featureFactories.size() = %d\n", FeatureFactory::featureFactories.size());
	}

	newFeatureIndex++;
	if(newFeatureIndex >= FeatureFactory::featureFactories.size()) {
		newFeatureIndex = 0;
	}

	if(LOG_DEBUG) {
		Serial.printf("Debug : [Main] onUpButtonClick newFeatureIndex = %d\n", newFeatureIndex);
		Serial.printf("Debug : [Main] onUpButtonClick featureFactories.size() = %d\n", FeatureFactory::featureFactories.size());
	}
}

/**
 * Called on up button double click
 */
void onUpButtonDoubleClick() {

	if(LOG_DEBUG) Serial.println("Debug : [Main] onUpButtonDoubleClick general");
	screenHelper.changeBrightness();
}
