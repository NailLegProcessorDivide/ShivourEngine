#pragma once

#include "JMem.h"
#include <memory>
#include <time.h>
#include <chrono>


#define hClock std::chrono::high_resolution_clock

#ifdef LOGGING_ON
#define ADDLOG(VAR, NAME, MFILE) VAR = jstl::logger.addLogFile(NAME, MFILE);
#define JLOG( KEY, FMT, ... ) {jstl::logger.writeLogFile(KEY, FMT, __VA_ARGS__);}
#define JLOG_ENABLE(NAME) jstl::logger.enable(NAME);
#define JLOG_DISABLE(NAME) jstl::logger.disable(NAME);
#else
#define ADDLOG(VAR, NAME, FILE)
#define JLOG(...)
#define JLOG_ENABLE(NAME)
#define JLOG_DISABLE(NAME)
#endif

namespace jstl{

	class LogMaster;

	extern int timerLogID;

	class pathDesc {
	public:
		int numParts;
		char* parts[10];
		char* description;
		pathDesc(const char* desc) : numParts(0), description(new char[strlen(desc) + 1]){
			strcpy(description, desc);
			parts[numParts++] = description;
			for (int i = 0; i < strlen(desc) && numParts < 9; ++i) {
				if (description[i] == '.') {
					parts[numParts++] = description + i + 1;
				}
			}
		}
		bool matches(pathDesc& pd) {

			if (strlen(description) > strlen(pd.description)) {
				if (memcmp(description, pd.description, strlen(pd.description)) == 0 && description[strlen(pd.description)] == '.') {
					return true;
				}
			}
			if (strlen(description) == strlen(pd.description)) {
				if (strcmp(description, description) == 0) {
					return true;
				}
			}
			return false;
		}
		~pathDesc() {
			delete[] description;
		}
	};

	class logFileInfo {
	public:
		FILE* file;
		pathDesc logDesc;
		bool enabled;
		logFileInfo(const char* desc) : logDesc(desc), file(nullptr), enabled(true){}
		~logFileInfo() {}
	};

	class LogMaster
	{
		Ticketer<std::shared_ptr<logFileInfo>> files;
	public:
		LogMaster() {
			timerLogID = addLogFile("timer", stdout);
		}
		~LogMaster() {}
	public:
		int addLogFile(const char* fileInfo, FILE* file) {
			//int descriptorLength = strlen(fileInfo);
			std::shared_ptr<logFileInfo> record = std::make_shared<logFileInfo>(fileInfo);
			record->file = file;
			return files.add(record);

		}

		void writeLogFile(int fileID, const char* data, ...) {
#ifdef LOGGING_ON
			std::shared_ptr<logFileInfo> file = files.get(fileID);
			if (file->file != nullptr){
				if (file->enabled) {
					va_list args;
					va_start(args, data);
					char sTime[30];
					time_t now;
					time(&now);
					strcpy(sTime, ctime(&now));
					sTime[strlen(sTime) - 1] = 0;
					fprintf(file->file, "%s | %-40s | ", sTime, file->logDesc.description);
					vfprintf(file->file, data, args);
					va_end(args);
				}
			}
			else {
				printf("%s file is null\n", file->logDesc.description);
			}
#endif // LOGGING_ON
		}

		void enable(const char* fileID) {
			pathDesc pd(fileID);
			int i = 0;
			int objectRef;
			while ((objectRef = files.getIt(i++)) != -1) {
				if (files.get(objectRef)->logDesc.matches(pd)) {
					files.get(objectRef)->enabled = true;
				}
			}
		}

		void disable(const char* fileID) {
			pathDesc pd(fileID);
			int i = 0;
			int objectRef;
			while ((objectRef = files.getIt(i++)) != -1) {
				if (files.get(objectRef)->logDesc.matches(pd)) {
					files.get(objectRef)->enabled = false;
				}
			}
		}
	};

	extern LogMaster logger;


	//https://www.youtube.com/watch?v=YG4jexlSAjc heavily based on
	class Timer {
	public:
	private:
		std::chrono::time_point<hClock> startTime;
		const char* outputString;
	public:
		Timer(const char* str) {
			outputString = str;
			startTime = hClock::now();
		}
		~Timer() {
			std::chrono::time_point<hClock> endTime = hClock::now();
			auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startTime).time_since_epoch().count();
			auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();
			long long dif = end - start;
			JLOG(timerLogID, outputString, dif * 0.001f);
		}
	};

}