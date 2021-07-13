#ifndef PRINTER_FEATURE_H
#define PRINTER_FEATURE_H

// local Includes
#include "core/Defines.hpp"
#include "core/Feature.hpp"
#include "core/WebClient.hpp"

namespace CrowOs {
	namespace Feature {

		/**
		 * 3D Printer class
		 */
		class Printer {
		public:
			/** Printer id in backend database */
			long id = -1;

			/** Printer machine name */
			char machineName[27];

			/** Printer ip */
			char machineIp[27];

			/** Printer port */
			int machinePort;

			/** Printer led color */
			char ledColor[27];

			/** Printer x position */
			double x;

			/** Printer max x position */
			double maxX;

			/** Printer y position */
			double y;

			/** Printer max y position */
			double maxY;

			/** Printer z position */
			double z;

			/** Printer max z position */
			double maxZ;

			/** Printer left extruder temperature */
			int temperatureExtruderLeft;

			/** Printer right extruder temperature */
			int temperatureExtruderRight;

			/** Printer bed temperature */
			int temperatureBed;

			Printer()
				: id(-1)
				, machineName()
				, machineIp()
				, machinePort(-1)
				, ledColor()
				, x(-1)
				, maxX(-1)
				, y(-1)
				, maxY(-1)
				, z(-1)
				, maxZ(-1)
				, temperatureExtruderLeft(-1)
				, temperatureExtruderRight(-1)
				, temperatureBed(-1) {
			}
		};

		/**
		 * Printer feature class
		 */
		class PrinterFeature : public Core::Feature {

		private:
			/** Delay to wait betweend two backend calls */
			const int FETCH_BACKEND_DELAY;

			/** List of led colors */
			const char* COLORS[5] = {"WHITE", "RED", "GREEN", "BLUE", "PURPLE"};

			/** Pointer to screen helper */
			Core::Screen* screen;

			/** Web client instance */
			Core::WebClient webClient;

			/** Indicates if screen should be redrawen */
			boolean shouldRedrawScreen;

			/** Last time printers list was fetched */
			int lastPrinterListFetch;

			/** Last time printer was fetched */
			int lastPrinterFetch;

			/** foreground color */
			const uint16_t foregroundColor;

			/** background color */
			const uint16_t backgroundColor;

			/** Size of discovered printers */
			int printerSize;

			/** Current printer index */
			short printerIndex;

			/** Current view index 0 = printersMenu; 1 = printerDetails */
			short viewIndex;

			/** List of fetched printers */
			Printer printers[5];

			/**
			 * Fetch printer list from backend
			 */
			void fetchPrinterList();

			/**
			 * Fetch single printer from backend
			 */
			void fetchPrinter();

			/**
			 * Shows printer list menu
			 * user can choose between diferent printers clicking home button
			 * user can choose to show printer by double clicking home button
			 */
			void showPrintersMenu();

			/**
			 * Shows currently selected printer details
			 */
			void showPrinterDetails();

			/**
			 * Toggles printer led color
			 */
			void toggleLedColor();

		public:
			/**
			 * Initialise new PrinterFeature
			 */
			PrinterFeature();

			/**
			 * Called after Feature creation before loop when state changes to this feature
			 * You should initialise all your variables here and restore savedData to your class if savedData is not null
			 *
			 * @param screenHelper Screen helper used to manage screen
			 * @param timeHelper   Time helper used to manage upTime and sleepTime
			 * @param ledHelper    Led helper used to manage Led state
			 * @param savedData    pointer to feature persistent data
			 */
			void onStart(Core::Screen* screenHelper, Core::Time* timeHelper, Core::Led* ledHelper, DynamicJsonDocument* savedData) override;

			/**
			 * Called before Feature destroying after loop when state changes from this feature to another
			 * You should destroy all your variables here and save the persistent one into savedData pointer
			 *
			 * @param savedData pointer to feature persistent data
			 */
			void onStop(DynamicJsonDocument* savedData) override;

			/**
			 * Main Feature loop
			 * You should developp your main loop here
			 */
			void loop() override;

			/**
			 * Called when home button is clicked
			 */
			void onHomeClick() override;

			/**
			 * Called when home button is double clicked
			 */
			void onHomeDoubleClick() override;
		};

	} // namespace Feature
} // namespace CrowOs

#endif
