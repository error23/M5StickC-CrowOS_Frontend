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
Time timeHelper(60, 30);

/** Led helper */
Led ledHelper;

/** Screen helper */
Screen screenHelper;

/** SmartWifi helper */
SmartWifi smartWifi;

/** Web client helper */
WebClient webClient(BACKEND_HOST, BACKEND_PORT, BACKEND_USER_USERNAME, BACKEND_USER_PASSWORD, BACKEND_BASE_PATH);

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
	smartWifi.setUp();

	initialiseFeatureFactories();
	initialiseFeatureData();
	setUpPermanentFeatures();

	if(LOG_INFO) Serial.println("Info : [Main] Setup Done");
}

/**
 * Main loop method
 */
void loop() {

	timeHelper.limitFps();
	tickButtons();

	if(timeHelper.shouldSleep() || sleeping) {
		sleep();
	}
	else {

		// Checks wifi status and try to reconnect it if is not connected
		if(!smartWifi.checkStatus()) smartWifi.reconnect();

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

					// Deserialise saved data into json document
					DynamicJsonDocument json(MAX_JSON_DOCUMENT_SIZE);
					deserializeJson(json, FeatureFactory::featureFactories[currentFeatureIndex].second);
					if(LOG_DEBUG) Serial.printf("Debug : [Main] loop retrieve second = %s\n", FeatureFactory::featureFactories[currentFeatureIndex].second.c_str());

					// Stop feature
					currentFeature->onStop(&json);

					// Serialize json document into saved data
					String s;
					serializeJson(json, s);
					FeatureFactory::featureFactories[currentFeatureIndex].second = s;
					if(LOG_DEBUG) Serial.printf("Debug : [Main] loop save second = %s\n", FeatureFactory::featureFactories[currentFeatureIndex].second.c_str());

					// kill current feature
					delete currentFeature;
				}

				// start new feature
				currentFeature = FeatureFactory::featureFactories[newFeatureIndex].first->createFeature();

				// if there is no saved data start feature with null
				if(FeatureFactory::featureFactories[newFeatureIndex].second == NULL) {
					if(LOG_DEBUG) Serial.println("Debug : [Main] loop new second = NULL");

					currentFeature->onStart(&screenHelper, &timeHelper, &ledHelper, NULL);
				}
				// Else deserialise json document and start feature with it
				else {
					DynamicJsonDocument json(MAX_JSON_DOCUMENT_SIZE);
					deserializeJson(json, FeatureFactory::featureFactories[newFeatureIndex].second);
					if(LOG_DEBUG) Serial.printf("Debug : [Main] loop retrieve new second = %s\n", FeatureFactory::featureFactories[newFeatureIndex].second.c_str());
					currentFeature->onStart(&screenHelper, &timeHelper, &ledHelper, &json);
				}

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

	if(LOG_INFO) Serial.println("Info : [Main] shutdown ...");

	// TODO : call currentFeature.onStop()
	delete currentFeature;

	shutdownPermanentFeatures();
	shutdownFeatureFactories();
	smartWifi.disconnect();

	if(LOG_INFO) Serial.println("Info : [Main] shutdown Done");
	delay(50);
	M5.Axp.PowerOff();
}

/**
 * Put device in sleep mode
 */
void sleep() {

	if(LOG_INFO) Serial.println("Info : [Main] sleep");

	if(sleeping) {
		delay(500);
		if(LOG_DEBUG) Serial.println("Debug : [Main] sleep delay = 500 ms");
		return;
	}

	sleeping = true;
	if(LOG_DEBUG) Serial.println("Debug : [Main] sleep sleeping = true");
	M5.Axp.SetSleep();
	smartWifi.disconnect();
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
	smartWifi.connect();

	sleeping = false;
}

/**
 * Initialise feature saved data from the server
 */
void initialiseFeatureData() {

	if(LOG_INFO) Serial.println("Info : [Main] initialiseFeatureData ...");

	for(auto& featureFactorySavedDataPair : FeatureFactory::featureFactories) {
		//*((char*)featureFactorySavedDataPair.second) = &"{\"test\":\"321\"}";
	}

	if(LOG_INFO) Serial.println("Info : [Main] initialiseFeatureData Done");
}

/**
 * Sends feature saved data to the server
 */
void saveFeatureDataToServer() {

	if(LOG_INFO) Serial.println("Info : [Main] saveFeatureDataToServer ...");
	if(LOG_INFO) Serial.println("Info : [Main] saveFeatureDataToServer Done");
}

/**
 * Sets up alwaysLoop = true features
 */
void setUpPermanentFeatures() {

	if(LOG_INFO) Serial.println("Info : [Main] setUpPermanentFeatures ...");

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
 * Deletes all alwaysLoop = true fatures
 */
void shutdownPermanentFeatures() {
	if(LOG_INFO) Serial.println("Info : [Main] shutdownPermanentFeatures ...");

	for(auto& permanentFeature : permanentFeatures) {

		DynamicJsonDocument json(1024);
		permanentFeature->onStop(&json);

		delete permanentFeature;
	}
	if(LOG_INFO) Serial.println("Info : [Main] shutdownPermanentFeatures Done");
}

/**
 * Deletes all feature factories
 */
void shutdownFeatureFactories() {

	if(LOG_INFO) Serial.println("Info : [Main] shutdownFeatureFactories ...");

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

	if(LOG_INFO) Serial.println("Info : [Main] setUpButtons ...");

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

	uint8_t event = M5.Axp.GetBtnPress();
	if(event == 0x01) {
		onPowerButtonLongClick();
	}
	else if(event == 0x02) {
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
