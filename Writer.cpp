#include <fstream>
#include <iomanip>
#include <sstream>
#include "Writer.h"

Writer& Writer::getInstance() {
	static Writer instance; //Instance made on first use
	return instance;
}

//Remembering a line for intermediate form file
void Writer::addImfLine(const Token * result, const Token * operand1, const Token * operand2) {
	numberOfLines++;
	string newLine = "[" + to_string(numberOfLines) + "]";
	if (result) {
		newLine += ' ' + result->getOperationSymbol();
		if (!(result->getOperationSymbol() == "="))
			newLine += ' ' + result->getName();
	}
	if (operand1) newLine += ' ' + operand1->getName();
	if (operand2) newLine += ' ' + operand2->getName();
	linesForWriting.push_back(newLine);
}

//Remembering a line for memory file
void Writer::addMemLine(const string & variableName, const double variableValue) {
	numberOfLines++;

	std::stringstream stream;
	stream << std::noshowpoint << variableValue;
	string strVal = stream.str();
	string newLine = variableName + " = " + strVal;

	linesForWriting.push_back(newLine);
}

//Write all remembered lines
void Writer::writeFile(const string& outputFileName) {
	fstream outputFile(outputFileName, ios::out);
	for (auto it : linesForWriting) {
		outputFile << it << endl;
	}
	outputFile.close();

	clear(); //after writing, clear remembered lines
}

//.log file
void Writer::saveLogData(ID id, const Time startTime, const Time endTime) {
	LogData field;
	field.id = id;
	field.startTime = startTime;
	field.endTime = endTime;
	this->logData.push_back(field);
}

// A utility function to swap two elements  
void swap(LogData* a, LogData* b) {
	LogData t = *a;
	*a = *b;
	*b = t;
}

//puts pivot on right place and returns pivot position
int partition(vector<LogData>& arr, int low, int high) {
	LogData pivot = arr[(high + low)/2]; // pivot  
	int i = low, j = high - 1;
	
	while (1) {
		//while arr[j] should stay on the right side of pivot element
		while (arr[j].startTime > pivot.startTime ||
			(arr[j].startTime == pivot.startTime && arr[j].endTime > pivot.endTime) ||
			(arr[j].startTime == pivot.startTime && arr[j].endTime == pivot.endTime && arr[j].id > pivot.id))
			j--;
		//while arr[i] should stay on the left side of pivot element
		while (arr[i].startTime < pivot.startTime ||
			(arr[i].startTime == pivot.startTime && arr[i].endTime < pivot.endTime) ||
			(arr[i].startTime == pivot.startTime && arr[i].endTime == pivot.endTime && arr[i].id < pivot.id))
			i++;
		//swap
		if (i < j) swap(&arr[i], &arr[j]);
		else return j + 1;
	}	
}

void quickSort(vector<LogData>& arr, int low, int high) {
	if (low < high) {
		// p is index of element which is now on right place
		int pi = partition(arr, low, high);

		// sorting subarrays 
		quickSort(arr, low, pi - 1);
		quickSort(arr, pi , high);
	}
}

// sort log data elements in writer
void Writer::sortLogData() {
	quickSort(logData, 0, logData.size());
}

//Remembering a line for execution file
void Writer::addLogLine(ID id, const Time startTime, const Time endTime) {
	numberOfLines++;
	string newLine = "[" + to_string(id) + "]" + '\t';

	std::stringstream stream1;
	stream1 << std::noshowpoint << startTime;
	std::string s = stream1.str();
	newLine += "(" + s;

	std::stringstream stream2;
	stream2 << std::noshowpoint << endTime;
	s = stream2.str();
	newLine += "-" + s + ")ns";

	linesForWriting.push_back(newLine);
}

void Writer::writeLogFile(const string & outputFileName) {
	//sort database
	sortLogData();
	//add database to lines for writing
	for (auto it : logData) {
		addLogLine(it.id, it.startTime, it.endTime);
	}
	//write like any other file
	writeFile(outputFileName);
}

Writer::~Writer() {
	this->clear(); //clear any left (not written) lines
}

void Writer::clear() {
	logData.clear();
	linesForWriting.clear();
	numberOfLines = 0;
}
