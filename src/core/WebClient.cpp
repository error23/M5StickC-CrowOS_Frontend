/**
 * Web client class implementation
 * @author error23
 */
#include "core/WebClient.hpp"

namespace CrowOs {
	namespace Core {

		/**
		 * Initialise new web client
		 *
		 * @param host     distinct server host
		 * @param port     distinct server port
		 * @param username distinct server username
		 * @param password distinct server password
		 * @param basePath distinct server application base path
		 */
		WebClient::WebClient(const char* host, const uint16_t port, const char* username, const char* password, const char* basePath)
			: host(host)
			, port(port)
			, username(username)
			, password(password)
			, basePath(basePath) {
			if(LOG_INFO) Serial.printf("Info : [WebClient] created host = %s, port = %d, basePath = %s\n", host, port, basePath);
		}

		/**
		 * Sends GET query to distinct server
		 *
		 * @param path         path on distinct server ressource
		 * @param responseBody server response body
		 * @return status      server status response
		 */
		int WebClient::sendGET(const char* path, DynamicJsonDocument& responseBody) {

			char uri[strlen(basePath) + strlen(path)];
			strcpy(uri, basePath);
			strcat(uri, path);
			if(LOG_INFO) Serial.printf("Info : [WebClient] sendGET uri = %s\n", uri);
			if(LOG_DEBUG) Serial.printf("Debug : [WebClient] sendGET basePath = %s, path = %s\n", basePath, path);

			http.begin(host, port, uri);
			addGeneralHeaders();

			int status = http.GET();
			const char* response = http.getString().c_str();
			deserializeJson(responseBody, response);
			http.end();

			if(LOG_DEBUG) Serial.printf("Debug : [WebClient] sendGET response = %s, status = %d\n", response, status);

			return status;
		}

		/**
		 * Send POST query to distinct server
		 *
		 * @param path         path on distinct server ressource
		 * @param payload      payload to send to server
		 * @param responseBody server response body
		 * @return status      server status response
		 */
		int WebClient::sendPOST(const char* path, const DynamicJsonDocument payload, DynamicJsonDocument& responseBody) {

			char uri[strlen(basePath) + strlen(path)];
			strcpy(uri, basePath);
			strcat(uri, path);
			if(LOG_INFO) Serial.printf("Info : [WebClient] sendPOST uri = %s\n", uri);
			if(LOG_DEBUG) Serial.printf("Debug : [WebClient] sendPOST basePath = %s, path = %s\n", basePath, path);

			http.begin(host, port, uri);
			addGeneralHeaders();

			int jsonSize = measureJson(payload) + 1;
			char jsonString[jsonSize];
			serializeJson(payload, jsonString, jsonSize);
			if(LOG_DEBUG) Serial.printf("Debug : [WebClient] sendPOST payload = %s\n", jsonString);

			int status = http.POST(jsonString);
			const char* response = http.getString().c_str();
			deserializeJson(responseBody, response);
			http.end();

			if(LOG_DEBUG) Serial.printf("Debug : [WebClient] sendPOST response = %s, status = %d\n", response, status);

			return status;
		}

		/**
		 * Send PUT query to distinct server
		 *
		 * @param path         path on distinct server ressource
		 * @param payload      payload to send to server
		 * @param responseBody server response body
		 * @return status      server status response
		 */
		int WebClient::sendPUT(const char* path, const DynamicJsonDocument payload, DynamicJsonDocument& responseBody) {

			char uri[strlen(basePath) + strlen(path)];
			strcpy(uri, basePath);
			strcat(uri, path);
			if(LOG_INFO) Serial.printf("Info : [WebClient] sendPUT uri = %s\n", uri);
			if(LOG_DEBUG) Serial.printf("Debug : [WebClient] sendPUT basePath = %s, path = %s\n", basePath, path);

			http.begin(host, port, uri);
			addGeneralHeaders();

			int jsonSize = measureJson(payload) + 1;
			char jsonString[jsonSize];
			serializeJson(payload, jsonString, jsonSize);
			if(LOG_DEBUG) Serial.printf("Debug : [WebClient] sendPUT payload = %s\n", jsonString);

			int status = http.PUT(jsonString);
			const char* response = http.getString().c_str();
			deserializeJson(responseBody, response);
			http.end();

			if(LOG_DEBUG) Serial.printf("Debug : [WebClient] sendPUT response = %s, status = %d\n", response, status);

			return status;
		}

		/**
		 * Send PATCH query to distinct server
		 *
		 * @param path         path on distinct server ressource
		 * @param payload      payload to send to server
		 * @param responseBody server response body
		 * @return status      server status response
		 */
		int WebClient::sendPATCH(const char* path, const DynamicJsonDocument payload, DynamicJsonDocument& responseBody) {

			char uri[strlen(basePath) + strlen(path)];
			strcpy(uri, basePath);
			strcat(uri, path);
			if(LOG_INFO) Serial.printf("Info : [WebClient] sendPATCH uri = %s\n", uri);
			if(LOG_DEBUG) Serial.printf("Debug : [WebClient] sendPATCH basePath = %s, path = %s\n", basePath, path);

			http.begin(host, port, uri);
			addGeneralHeaders();

			int jsonSize = measureJson(payload) + 1;
			char jsonString[jsonSize];
			serializeJson(payload, jsonString, jsonSize);
			if(LOG_DEBUG) Serial.printf("Debug : [WebClient] sendPATCH payload = %s\n", jsonString);

			int status = http.PATCH(jsonString);
			const char* response = http.getString().c_str();
			deserializeJson(responseBody, response);
			http.end();

			if(LOG_DEBUG) Serial.printf("Debug : [WebClient] sendPATCH response = %s, status = %d\n", response, status);

			return status;
		}

		/**
		 * Send DELETE query to distinct server
		 *
		 * @param path         path on distinct server ressource
		 * @param responseBody server response body
		 * @return status      server status response
		 */
		int WebClient::sendDELETE(const char* path, DynamicJsonDocument& responseBody) {

			char uri[strlen(basePath) + strlen(path)];
			strcpy(uri, basePath);
			strcat(uri, path);
			if(LOG_INFO) Serial.printf("Info : [WebClient] sendDELETE uri = %s\n", uri);
			if(LOG_DEBUG) Serial.printf("Debug : [WebClient] sendDELETE basePath = %s, path = %s\n", basePath, path);

			http.begin(host, port, uri);
			addGeneralHeaders();

			int status = http.sendRequest("DELETE");
			const char* response = http.getString().c_str();
			deserializeJson(responseBody, response);
			http.end();

			if(LOG_DEBUG) Serial.printf("Debug : [WebClient] sendDELETE response = %s, status = %d\n", response, status);

			return status;
		}

		/**
		 * Adds general server headers
		 */
		void WebClient::addGeneralHeaders() {

			http.addHeader("Content-Type", "application/json");
			http.setAuthorization(username, password);
		}

	} // namespace Core
} // namespace CrowOs
