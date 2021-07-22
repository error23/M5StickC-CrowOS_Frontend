/**
 * PrinterFeature class implementation
 */
#include "feature/PrinterFeature.hpp"

namespace CrowOs {
	namespace Feature {

		/**
		 * Initialise new PrinterFeature
		 */
		PrinterFeature::PrinterFeature()
			: Feature("PrinterFeature")
			, FETCH_BACKEND_DELAY(5000)
			, screen(NULL)
			, webClient(BACKEND_HOST, BACKEND_PORT, BACKEND_USER_USERNAME, BACKEND_USER_PASSWORD, BACKEND_BASE_PATH)
			, shouldRedrawScreen(true)
			, lastPrinterListFetch(0)
			, lastPrinterFetch(0)
			, foregroundColor(TFT_CYAN)
			, backgroundColor(0x2A)
			, printerSize(0)
			, printerIndex(0)
			, viewIndex(0)
			, printers({}) {
		}

		/**
		 * Called after Feature creation before loop when state changes to this feature
		 * You should initialise all your variables here and restore savedData to your class if savedData is not null
		 *
		 * @param screenHelper Screen helper used to manage screen
		 * @param timeHelper   Time helper used to manage upTime and sleepTime
		 * @param ledHelper    Led helper used to manage Led state
		 * @param savedData    pointer to feature persistent data
		 */
		void PrinterFeature::onStart(Core::Screen* screenHelper, Core::Time* timeHelper, Core::Led* ledHelper, DynamicJsonDocument* savedData) {

			if(LOG_INFO) Serial.println("Info : [PrinterFeature] onStart");

			// Strore useful helpers
			screen = screenHelper;

			// Get saved data
			if(LOG_DEBUG) Serial.printf("Debug : [PrinterFeature] onStart printerIndex = %d", printerIndex);
			if(savedData != NULL) printerIndex = (*savedData)["printerIndex"];
			if(LOG_DEBUG) Serial.printf(", new printerIndex = %d\n", printerIndex);

			// Clear screen and show frame fro progress bar
			screen->setScreenOrientation(Core::Screen::SCREEN_RIGHT_LANDSCAPE);
			screen->setBackground(backgroundColor);
			screen->clearLCD();
		}

		/**
		 * Called before Feature destroying after loop when state changes from this feature to another
		 * You should destroy all your variables here and save the persistent one into savedData pointer
		 *
		 * @param savedData pointer to feature persistent data
		 */
		void PrinterFeature::onStop(DynamicJsonDocument* savedData) {

			if(LOG_INFO) Serial.println("Info : [PrinterFeature] onStop");
			if(savedData != NULL) (*savedData)["printerIndex"] = printerIndex;
			if(LOG_INFO) Serial.printf("Info : [PrinterFeature] onStop printerIndex = %d\n", printerIndex);
		}

		/**
		 * Main Feature loop
		 * You should developp your main loop here
		 */
		void PrinterFeature::loop() {

			if(viewIndex == 0) {
				fetchPrinterList();
				showPrintersMenu();
			}
			else {
				fetchPrinter();
				showPrinterDetails();
			}
		}

		/**
		 * Fetch printer list from backend
		 */
		void PrinterFeature::fetchPrinterList() {

			if(millis() - lastPrinterListFetch > FETCH_BACKEND_DELAY) {

				if(LOG_INFO) Serial.println("Info : [PrinterFeature] fetchPrinterList");

				DynamicJsonDocument responseBody(MAX_JSON_DOCUMENT_SIZE);
				int status = webClient.sendGET("printer", responseBody);
				if(status != 200) {
					char err[15];
					sprintf(err, "server er:%d", status);
					screen->showError(err, FETCH_BACKEND_DELAY);
					shouldRedrawScreen = false;
					return;
				}

				JsonArray printerDtos = responseBody.as<JsonArray>();
				printerSize = printerDtos.size();

				for(int i = 0; i < printerSize; i++) {

					Printer printer;

					printer.id = printerDtos[i]["id"];
					strncpy(printer.machineName, printerDtos[i]["machineName"] | "\0", 27);
					strncpy(printer.machineIp, printerDtos[i]["machineIp"] | "\0", 27);
					printer.machinePort = printerDtos[i]["machinePort"];
					printers[i] = printer;
				}

				lastPrinterListFetch = millis();
				shouldRedrawScreen = true;
			}
		}

		/**
		 * Fetch single printer from backend
		 */
		void PrinterFeature::fetchPrinter() {

			if(millis() - lastPrinterFetch > FETCH_BACKEND_DELAY) {

				if(LOG_INFO) Serial.println("Info : [PrinterFeature] fetchPrinter");

				lastPrinterFetch = millis();

				DynamicJsonDocument printerDto(MAX_JSON_DOCUMENT_SIZE);
				char uri[16];
				sprintf(uri, "printer/%ld", printers[printerIndex].id);

				int status = webClient.sendGET(uri, printerDto);
				if(status != 200) {
					char err[screen->getMaxXCharacters()];

					if(status == 503) {
						sprintf(err, "server er: printer is off");
					}
					else {
						sprintf(err, "server er:%d", status);
					}

					screen->showError(err, FETCH_BACKEND_DELAY);
					viewIndex = 0;
					shouldRedrawScreen = false;
					screen->clearLCD();
					return;
				}

				printers[printerIndex].id = printerDto["id"];

				strncpy(printers[printerIndex].machineName, printerDto["machineName"] | "\0", 27);
				strncpy(printers[printerIndex].machineIp, printerDto["machineIp"] | "\0", 27);

				printers[printerIndex].machinePort = printerDto["machinePort"];

				strncpy(printers[printerIndex].ledColor, printerDto["ledColor"] | "\0", 27);

				printers[printerIndex].x = printerDto["x"];
				printers[printerIndex].maxX = printerDto["maxX"];

				printers[printerIndex].y = printerDto["y"];
				printers[printerIndex].maxY = printerDto["maxY"];

				printers[printerIndex].z = printerDto["z"];
				printers[printerIndex].maxZ = printerDto["maxZ"];

				printers[printerIndex].temperatureExtruderLeft = printerDto["temperatureExtruderLeft"];
				printers[printerIndex].temperatureExtruderRight = printerDto["temperatureExtruderRight"];
				printers[printerIndex].temperatureBed = printerDto["temperatureBed"];
				printers[printerIndex].printingProgress = printerDto["printingProgress"];

				lastPrinterFetch = millis();
				shouldRedrawScreen = true;
			}
		}

		/**
		 * Shows printer list menu
		 * user can choose between diferent printers clicking home button
		 * user can choose to show printer by double clicking home button
		 */
		void PrinterFeature::showPrintersMenu() {

			if(shouldRedrawScreen) {
				if(LOG_INFO) Serial.println("Info : [PrinterFeature] showPrintersMenu");

				for(int i = 0; i < printerSize; i++) {
					char displayName[screen->getMaxXCharacters()];

					short textColor = foregroundColor;
					short highlightColor = backgroundColor;

					if(i == printerIndex) {
						strncpy(displayName, "> \0", 3);
						textColor = backgroundColor;
						highlightColor = foregroundColor;
					}
					else {
						displayName[0] = '\0';
					}

					if(strcmp(printers[i].machineName, "\0") != 0) {
						strncat(displayName, printers[i].machineName, screen->getMaxXCharacters());
					}
					else if(strcmp(printers[i].machineIp, "\0") != 0) {
						strncat(displayName, printers[i].machineIp, screen->getMaxXCharacters());
					}

					screen->clearText(screen->getMaxXCharacters(), 5, screen->getMinY() + 5 + (10 * i));
					screen->printText(displayName, 5, screen->getMinY() + 5 + (10 * i), textColor, highlightColor);

					if(LOG_DEBUG) Serial.printf("Debug : [PrinterFeature] showPrintersMenu displayName = %s\n", displayName);
				}
				shouldRedrawScreen = false;
			}
		}

		/**
		 * Shows currently selected printer details
		 */
		void PrinterFeature::showPrinterDetails() {
			if(shouldRedrawScreen) {
				if(LOG_INFO) Serial.println("Info : [PrinterFeature] showPrinterDetails");

				char displayName[10];
				if(strcmp(printers[printerIndex].machineName, "\0") != 0) {
					strncpy(displayName, printers[printerIndex].machineName, 9);
				}
				else if(strcmp(printers[printerIndex].machineIp, "\0") != 0) {
					strncpy(displayName, printers[printerIndex].machineIp, 9);
				}
				displayName[9] = '\0';

				screen->clearText(9, 49, 3);
				screen->printText(displayName, 49, 3, foregroundColor);

				char buff[screen->getMaxXCharacters()];

				sprintf(buff, "IP : %s", printers[printerIndex].machineIp);
				screen->clearText(screen->getMaxXCharacters(), 5, screen->getMinY() + 2);
				screen->printText(buff, 5, screen->getMinY() + 2, foregroundColor);

				sprintf(buff, "PORT : %d", printers[printerIndex].machinePort);
				screen->clearText(screen->getMaxXCharacters(), 5, screen->getMinY() + 12);
				screen->printText(buff, 5, screen->getMinY() + 12, foregroundColor);

				if(printers[printerIndex].printingProgress == -1 || printers[printerIndex].printingProgress == 100) {
					sprintf(buff, "LED : %s", printers[printerIndex].ledColor);
					screen->clearText(screen->getMaxXCharacters(), 5, screen->getMinY() + 22);
					screen->printText(buff, 5, screen->getMinY() + 22, foregroundColor);
				}
				else {

					sprintf(buff, "Progress : %.2f%%", printers[printerIndex].printingProgress);
					screen->clearText(screen->getMaxXCharacters(), 5, screen->getMinY() + 22);
					screen->printText(buff, 5, screen->getMinY() + 22, foregroundColor);
				}

				M5.Lcd.drawLine(5, screen->getMinY() + 31, screen->getMaxX() - 5, screen->getMinY() + 31, foregroundColor);

				sprintf(buff, "X : %.0f/%.0f", printers[printerIndex].x, printers[printerIndex].maxX);
				screen->clearText(screen->getMaxXCharacters(), 5, screen->getMinY() + 35);
				screen->printText(buff, 5, screen->getMinY() + 35, foregroundColor);

				sprintf(buff, "Y : %.0f/%.0f", printers[printerIndex].y, printers[printerIndex].maxY);
				screen->clearText(screen->getMaxXCharacters(), 5, screen->getMinY() + 45);
				screen->printText(buff, 5, screen->getMinY() + 45, foregroundColor);

				sprintf(buff, "Z : %.0f/%.0f", printers[printerIndex].z, printers[printerIndex].maxZ);
				screen->clearText(screen->getMaxXCharacters(), 5, screen->getMinY() + 55);
				screen->printText(buff, 5, screen->getMinY() + 55, foregroundColor);

				M5.Lcd.drawLine(80, screen->getMinY() + 31, 80, screen->getMaxY(), foregroundColor);

				sprintf(buff, "TL : %d °", printers[printerIndex].temperatureExtruderLeft);
				screen->printText(buff, 90, screen->getMinY() + 35, foregroundColor);

				sprintf(buff, "TR : %d °", printers[printerIndex].temperatureExtruderRight);
				screen->printText(buff, 90, screen->getMinY() + 45, foregroundColor);

				sprintf(buff, "TB : %d °", printers[printerIndex].temperatureBed);
				screen->printText(buff, 90, screen->getMinY() + 55, foregroundColor);

				shouldRedrawScreen = false;
			}
		}

		/**
		 * Toggles printer led color
		 */
		void PrinterFeature::toggleLedColor() {
			if(LOG_INFO) Serial.println("Info : [PrinterFeature] toggleLedColor");

			unsigned int size = sizeof(COLORS) / sizeof(COLORS[0]);
			int i = 0;
			for(i = 0; i < size; i++) {
				if(strcmp(COLORS[i], printers[printerIndex].ledColor) == 0) {
					break;
				}
			}

			if(LOG_DEBUG) Serial.printf("Debug : [PrinterFeature] toggleLedColor COLOR = %s, i = %d\n", printers[printerIndex].ledColor, i);
			if(++i >= size) i = 0;

			strcpy(printers[printerIndex].ledColor, COLORS[i]);
			if(LOG_DEBUG) Serial.printf("Debug : [PrinterFeature] toggleLedColor COLOR = %s\n", COLORS[i]);

			DynamicJsonDocument printerDto(MAX_JSON_DOCUMENT_SIZE);
			DynamicJsonDocument emptyJson(32);
			char uri[50];
			sprintf(uri, "printer/%ld/color?color=%s", printers[printerIndex].id, printers[printerIndex].ledColor);

			int status = webClient.sendPATCH(uri, emptyJson, printerDto);
			if(status != 202) {
				char err[screen->getMaxXCharacters()];

				if(status == 503) {
					sprintf(err, "server er: printer is off");
				}
				else {
					sprintf(err, "server er:%d", status);
				}

				screen->showError(err, FETCH_BACKEND_DELAY);
			}
		}

		/**
		 * Called when home button is clicked
		 */
		void PrinterFeature::onHomeClick() {

			if(viewIndex == 0) {
				if(++printerIndex >= printerSize) printerIndex = 0;
			}
			else {
				toggleLedColor();
			}
			shouldRedrawScreen = true;
		}

		/**
		 * Called when home button is double clicked
		 */
		void PrinterFeature::onHomeDoubleClick() {
			if(++viewIndex > 1) viewIndex = 0;
			screen->clearLCD();
			shouldRedrawScreen = true;
		}

	} // namespace Feature
} // namespace CrowOs
