#include <sstream>
#include <iostream>
#include <string>
using namespace std;

class Logger {
public:
    explicit Logger(ostream& output_stream) : os(output_stream) {
    }

    void SetLogLine(bool value) { log_line = value; }
    void SetLogFile(bool value) { log_file= value; }
    void SetFile(string new_file) { file = new_file; }
    void SetLine(int new_line) { line = new_line; }

    void Log(const string& message) {
        if (log_file && log_line)
            os << file << ":" << line << " ";
        else if (log_file)
            os << file << " ";
        else if (log_line)
            os << "Line " << line << " ";
        os << message << "\n";
    }

private:
    ostream& os;
    bool log_line = false;
    bool log_file = false;
    string file;
    int line;
};

#define LOG(logger, message) \
    logger.SetFile(__FILE__);\
    logger.SetLine(__LINE__);\
    logger.Log(message);


int main() {
    Logger l(cout);
    LOG(l, "hello");

    l.SetLogFile(true);
    LOG(l, "hello");

    l.SetLogLine(true);
    LOG(l, "hello");

    l.SetLogFile(false);
    LOG(l, "hello");
}