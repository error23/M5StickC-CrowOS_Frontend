/**
 * SmartWifi class implementation
 * @author error23
 */
#include "core/SmartWifi.hpp"

namespace CrowOs {
	namespace Core {

		/**
		 * Initialise SmartWifi
		 */
		SmartWifi::SmartWifi()
			: reconnectionTimeOut(30000)
			, lastReconnectionTime(0) {
			if(LOG_INFO) Serial.println("Info : [SmartWifi] created with reconnectionTimeOut = 30000");
		}

		/**
		 * Set up Smart wifi
		 */
		void SmartWifi::setUp() {

			if(LOG_INFO) Serial.println("Info : [SmartWifi] Setup ...");

			if(M5.BtnA.isPressed()) configure();
			connect();

			if(LOG_INFO) Serial.println("Info : [SmartWifi] Setup Done");
		}

		/**
		 * Opens wifi connection
		 */
		void SmartWifi::connect() {

			if(LOG_INFO) Serial.println("Info : [SmartWifi] connect ...");

			M5.Lcd.pushImage(2, 2, 16, 10, res_wifi_connecting);
			WiFi.begin();
			checkStatus();
			lastReconnectionTime = millis();

			if(LOG_INFO) Serial.println("Info : [SmartWifi] connect Done");
		}

		/**
		 * Disconnect turning wifi off
		 */
		void SmartWifi::disconnect() {

			if(LOG_INFO) Serial.println("Info : [SmartWifi] disconnect ...");

			WiFi.disconnect(true);
			checkStatus();

			if(LOG_INFO) Serial.println("Info : [SmartWifi] disconnect Done");
		}

		/**
		 * Try to reconnect wifi
		 */
		void SmartWifi::reconnect() {

			if(millis() - lastReconnectionTime >= reconnectionTimeOut) {
				if(LOG_INFO) Serial.println("Info : [SmartWifi] reconnect ...");

				M5.Lcd.pushImage(2, 2, 16, 10, res_wifi_connecting);
				WiFi.disconnect();
				WiFi.reconnect();

				lastReconnectionTime = millis();
				if(LOG_INFO) Serial.println("Info : [SmartWifi] reconnect Done");
			}
		}

		/**
		 * Wait for connection for 100000 ms than fail
		 */
		const bool SmartWifi::waitUntilReconnect() {

			unsigned long now = millis();

			while(!checkStatus()) {

				reconnect();
				if(millis() - now >= 100000) {
					if(LOG_INFO) Serial.println("Info : [SmartWifi] waitUntilReconnect failed");
					return false;
				}
			}

			if(LOG_INFO) Serial.println("Info : [SmartWifi] waitUntilReconnect OK");
			return true;
		}

		/**
		 * Checks wifi status and shows wifi icon on the screen
		 *
		 * @return true if wifi is connected
		 */
		const bool SmartWifi::checkStatus() {

			if(LOG_DEBUG) {
				Serial.print("Debug : [SmartWifi] checkStatus SSID = ");
				Serial.print(WiFi.SSID());
				Serial.print(" localIpAdress = ");
				Serial.print(WiFi.localIP());
				Serial.print(" status = ");
				Serial.println(WiFi.status());
			}

			switch(WiFi.status()) {

			case WL_IDLE_STATUS:
			case WL_NO_SSID_AVAIL:
			case WL_SCAN_COMPLETED:
				M5.Lcd.pushImage(2, 2, 16, 10, res_wifi_connecting);
				return false;
				break;

			case WL_CONNECTED:
				M5.Lcd.pushImage(2, 2, 16, 10, res_wifi_connected);
				return true;
				break;

			case WL_DISCONNECTED:
				M5.Lcd.pushImage(2, 2, 16, 10, res_wifi_disconnected);
				return false;
				break;

			default:
				M5.Lcd.pushImage(2, 2, 16, 10, res_wifi_error);
				return false;
				break;
			}
		}

		/**
		 * Do smart config and wait until is done
		 */
		void SmartWifi::configure() {

			if(LOG_INFO) Serial.println("Info : [SmartWifi] configure ...");

			M5.Lcd.pushImage(2, 2, 16, 10, res_wifi_connfiguring);

			WiFi.mode(WIFI_STA);
			WiFi.beginSmartConfig();

			while(!WiFi.smartConfigDone()) {
				delay(500);
				if(LOG_DEBUG) Serial.println("Debug : [SmartWifi] configure sleeped for 500ms");
			}

			checkStatus();
			if(LOG_INFO) Serial.println("Info : [SmartWifi] configure Done");
		}

	} // namespace Core
} // namespace CrowOs
