#ifndef WEB_CLIENT_H
#define WEB_CLIENT_H

// Lib includes
#include "ArduinoJson.h"
#include "HTTPClient.h"

// local Includes
#include "Defines.hpp"

namespace CrowOs {
	namespace Core {

		/**
		 * Simple web client class
		 *
		 * Simple implementation of REST web client
		 */
		class WebClient {

		private:
			/** Http client instance */
			HTTPClient http;

			/** Host that this client has to connect to */
			const char* host;

			/** Port that this client has to connect to */
			const uint16_t port;

			/** Server username */
			const char* username;

			/** Server password */
			const char* password;

			/** Server base path */
			const char* basePath;

			/**
			 * Adds general server headers
			 */
			void addGeneralHeaders();

		public:
			/**
			 * Initialise new web client
			 *
			 * @param host     distinct server host
			 * @param port     distinct server port
			 * @param username distinct server username
			 * @param password distinct server password
			 * @param basePath distinct server application base path
			 */
			WebClient(const char* host, const uint16_t port, const char* username, const char* password, const char* basePath = "/");

			/**
			 * Sends GET query to distinct server
			 *
			 * @param path         path on distinct server ressource
			 * @param responseBody server response body
			 * @return status      server status response
			 */
			int sendGET(const char* path, DynamicJsonDocument& responseBody);

			/**
			 * Send POST query to distinct server
			 *
			 * @param path         path on distinct server ressource
			 * @param payload      payload to send to server
			 * @param responseBody server response body
			 * @return status      server status response
			 */
			int sendPOST(const char* path, const DynamicJsonDocument payload, DynamicJsonDocument& responseBody);

			/**
			 * Send PUT query to distinct server
			 *
			 * @param path         path on distinct server ressource
			 * @param payload      payload to send to server
			 * @param responseBody server response body
			 * @return status      server status response
			 */
			int sendPUT(const char* path, const DynamicJsonDocument payload, DynamicJsonDocument& responseBody);

			/**
			 * Send PATCH query to distinct server
			 *
			 * @param path         path on distinct server ressource
			 * @param payload      payload to send to server
			 * @param responseBody server response body
			 * @return status      server status response
			 */
			int sendPATCH(const char* path, const DynamicJsonDocument payload, DynamicJsonDocument& responseBody);

			/**
			 * Send DELETE query to distinct server
			 *
			 * @param path         path on distinct server ressource
			 * @param responseBody server response body
			 * @return status      server status response
			 */
			int sendDELETE(const char* path, DynamicJsonDocument& responseBody);
		};

	} // namespace Core
} // namespace CrowOs
#endif
