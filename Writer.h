#ifndef WRITER_H
#define WRITER_H

#include <string>
#include <vector>
#include "Token.h"
#include "Types.h"

using namespace std;

struct LogData {
	ID id;
	Time startTime;
	Time endTime;
};

//Writer is writing every file, when given right input. Singleton.
class Writer {
public:
	//Singleton - get the only instance
	static Writer& getInstance();
	Writer(Writer&) = delete;
	void operator=(Writer&) = delete;

	//Adding different types of lines to writer
	void addImfLine(const Token* result, const Token* operand1, const Token* operand2 = nullptr); //line in intermediate form
	void addMemLine(const string& variableName, const double variableValue); //line in memory file

	//Writing everything that was sent
	void writeFile(const string& outputFileName);

	//Log file - adding data first, sorting and translating to file
	void saveLogData(ID id, const Time startTime, const Time endTime); //remember in writer
	void sortLogData(); //first by start times, then end times, then id
	void addLogLine(ID id, const Time startTime, const Time endTime); //remember formated data as line of text
	void writeLogFile(const string& outputFileName); //writing log data in file outputFileName

	//Clearing everything remembered for writing
	void clear();

private:
	//Singleton - private constructor
	Writer() :numberOfLines(0) {};
	~Writer(); //before deleting clears any left lines

	//Remembering all lines sent for writing until it is written
	vector<string> linesForWriting;
	int numberOfLines;

	//Remember data for log file (sorting before writing)
	vector<LogData> logData;
};

#endif