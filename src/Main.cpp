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
Time timeHelper(120, 30);

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
	M5.MPU6886.Init();
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

				if(currentFeature != NULL) killCurrentFeature();
				currentFeature = startFeature(newFeatureIndex);
				currentFeatureIndex = newFeatureIndex;
			}

			// loop current feature if exists
			if(currentFeature != NULL) currentFeature->loop();
		}
	}
	screenHelper.loop();
}

/**
 * Main shutdown method
 */
void shutdown() {

	if(LOG_INFO) Serial.println("Info : [Main] shutdown ...");
	screenHelper.showLogo();

	killCurrentFeature();
	saveFeatureDataToServer(true);
	shutdownPermanentFeatures();
	shutdownFeatureFactories();
	smartWifi.disconnect();

	if(LOG_INFO) Serial.println("Info : [Main] shutdown Done");
	delay(100);
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
	saveFeatureDataToServer(false);
	if(currentFeature != NULL) killCurrentFeature();
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
	screenHelper.setUp();
	smartWifi.connect();
	if(currentFeatureIndex != -1) currentFeature = startFeature(currentFeatureIndex);

	sleeping = false;
}

/**
 * Initialise feature saved data from the server
 */
void initialiseFeatureData() {

	if(LOG_INFO) Serial.println("Info : [Main] initialiseFeatureData ...");

	if(!smartWifi.waitUntilReconnect()) {
		screenHelper.showError("Init failed !!", 10000);
		return;
	}

	DynamicJsonDocument responseBody(MAX_JSON_DOCUMENT_SIZE * (FeatureFactory::featureFactories.size() + 1));
	int status = webClient.sendGET("featureData", responseBody);
	if(status != 200) {
		char err[15];
		sprintf(err, "server er:%d", status);
		screenHelper.showError(err, 10000);
	}

	JsonArray featureDataDtos = responseBody.as<JsonArray>();

	for(DynamicJsonDocument featureDataDto : featureDataDtos) {

		for(auto& featureFactorySavedDataPair : FeatureFactory::featureFactories) {
			if(featureFactorySavedDataPair.first->getFeatureFactoryName() == featureDataDto["featureFactoryName"]) {

				featureFactorySavedDataPair.first->setFeatureFactoryId(featureDataDto["id"]);

				String s = "\0";
				if(!featureDataDto["savedData"].isNull()) {
					serializeJson(featureDataDto["savedData"], s);
				}

				if(LOG_DEBUG) Serial.printf("Debug : [Main] initialiseFeatureData save second = %s for featureFactoryName = %s\n", s.c_str(), featureFactorySavedDataPair.first->getFeatureFactoryName());
				featureFactorySavedDataPair.second = s;
				break;
			}
		}
	}

	if(LOG_INFO) Serial.println("Info : [Main] initialiseFeatureData Done");
}

/**
 * Sends feature saved data to the server
 *
 * @param waitForConnection if true method will call SmartWifi::waitUntilReconnect() method in the begining
 */
void saveFeatureDataToServer(boolean waitForConnection) {

	if(LOG_INFO) Serial.println("Info : [Main] saveFeatureDataToServer ...");

	if(waitForConnection && !smartWifi.waitUntilReconnect()) {
		screenHelper.showError("Init failed !!", 10000);
		return;
	}

	DynamicJsonDocument payload(MAX_JSON_DOCUMENT_SIZE * (FeatureFactory::featureFactories.size() + 1));
	JsonArray featureDataDtos = payload.to<JsonArray>();

	for(auto& featureFactorySavedDataPair : FeatureFactory::featureFactories) {

		DynamicJsonDocument featureDataDto(MAX_JSON_DOCUMENT_SIZE);

		if(featureFactorySavedDataPair.first->getFeatureFactoryId() != -1) featureDataDto["id"] = featureFactorySavedDataPair.first->getFeatureFactoryId();
		featureDataDto["featureFactoryName"] = featureFactorySavedDataPair.first->getFeatureFactoryName();

		DynamicJsonDocument savedData(MAX_JSON_DOCUMENT_SIZE);
		deserializeJson(savedData, featureFactorySavedDataPair.second);
		featureDataDto["savedData"] = savedData;

		featureDataDtos.add(featureDataDto);
	}

	DynamicJsonDocument responseBody(MAX_JSON_DOCUMENT_SIZE * (FeatureFactory::featureFactories.size() + 1));
	int status = webClient.sendPUT("featureData", payload, responseBody);
	if(status != 202) {
		char err[15];
		sprintf(err, "server er:%d", status);
		screenHelper.showError(err, 10000);
		screenHelper.loop();
	}

	if(LOG_INFO) Serial.println("Info : [Main] saveFeatureDataToServer Done");
}

/**
 * Sets up alwaysLoop = true features
 */
void setUpPermanentFeatures() {

	if(LOG_INFO) Serial.println("Info : [Main] setUpPermanentFeatures ...");

	for(int i = 0; i < FeatureFactory::featureFactories.size(); i++) {
		if(FeatureFactory::featureFactories[i].first->isAlwaysLoop()) {

			Feature* permanentFeature = FeatureFactory::featureFactories[i].first->createFeature();
			delete FeatureFactory::featureFactories[i].first;
			i--;

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

		permanentFeature->onStop(NULL);
		delete permanentFeature;
	}
	if(LOG_INFO) Serial.println("Info : [Main] shutdownPermanentFeatures Done");
}

/**
 * Deletes all feature factories
 */
void shutdownFeatureFactories() {

	if(LOG_INFO) Serial.println("Info : [Main] shutdownFeatureFactories ...");

	for(int i = 0; i < FeatureFactory::featureFactories.size(); i++) {
		if(FeatureFactory::featureFactories[i].first != NULL) {
			delete FeatureFactory::featureFactories[i].first;
			i--;
		}
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
 * Starts new feature from featureFactories
 *
 * @param featureIndex featureFactories index of feature to start
 * @return started feature
 */
Feature* startFeature(const int featureIndex) {

	// start new feature
	Feature* feature = FeatureFactory::featureFactories[featureIndex].first->createFeature();

	// if there is no saved data start feature with null
	if(FeatureFactory::featureFactories[featureIndex].second == NULL) {
		if(LOG_DEBUG) Serial.printf("Debug : [Main] startFeature name = %s new second = NULL\n", feature->getFeatureName());

		feature->onStart(&screenHelper, &timeHelper, &ledHelper, NULL);
	}
	// Else deserialise json document and start feature with it
	else {
		DynamicJsonDocument json(MAX_JSON_DOCUMENT_SIZE);
		deserializeJson(json, FeatureFactory::featureFactories[featureIndex].second);
		if(LOG_DEBUG) Serial.printf("Debug : [Main] startFeature name = %s retrieve new second = %s\n", feature->getFeatureName(), FeatureFactory::featureFactories[featureIndex].second.c_str());
		feature->onStart(&screenHelper, &timeHelper, &ledHelper, &json);
	}

	return feature;
}

/**
 * Kill current feature
 */
void killCurrentFeature() {

	// Deserialise saved data into json document
	DynamicJsonDocument json(MAX_JSON_DOCUMENT_SIZE);
	deserializeJson(json, FeatureFactory::featureFactories[currentFeatureIndex].second);
	if(LOG_DEBUG) Serial.printf("Debug : [Main] killCurrentFeature retrieve second = %s\n", FeatureFactory::featureFactories[currentFeatureIndex].second.c_str());

	// Stop feature
	currentFeature->onStop(&json);

	// Serialize json document into saved data
	String s;
	serializeJson(json, s);
	FeatureFactory::featureFactories[currentFeatureIndex].second = s;
	if(LOG_DEBUG) Serial.printf("Debug : [Main] killCurrentFeature save second = %s\n", FeatureFactory::featureFactories[currentFeatureIndex].second.c_str());

	// kill current feature
	delete currentFeature;
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
