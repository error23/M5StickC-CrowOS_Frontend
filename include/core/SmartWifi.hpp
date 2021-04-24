#ifndef SMART_WIFI_H
#define SMART_WIFI_H

// Lib includes
#include "M5StickC.h"
#include "WiFi.h"

// local Includes
#include "Defines.hpp"

// Resource includes
#include "resources/r_wifi.hpp"

namespace CrowOs {
	namespace Core {

		/**
		 * Smart wifi connector
		 *
		 * Helper used to manage wifi connections using ESP32 smartConfig to get credentials
		 */
		class SmartWifi {

		private:
			/** Time between to reconnection attrempts */
			unsigned long reconnectionTimeOut;

			/** Last reconnection try time */
			unsigned long lastReconnectionTime;

			/**
			 * Do smart config and wait until is done
			 */
			void configure();

		public:
			/**
			 * Initialise SmartWifi
			 */
			SmartWifi();

			/**
			 * Set up Smart wifi
			 */
			void setUp();

			/**
			 * Opens wifi connection
			 */
			void connect();

			/**
			 * Disconnect turning wifi off
			 */
			void disconnect();

			/**
			 * Try to reconnect wifi
			 */
			void reconnect();

			/**
			 * Checks wifi status and shows wifi icon on the screen
			 *
			 * @return true if wifi is connected
			 */
			const bool checkStatus();
		};

	} // namespace Core
} // namespace CrowOs

#endif
