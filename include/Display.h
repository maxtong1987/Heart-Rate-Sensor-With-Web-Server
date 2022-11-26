namespace Display {

enum class Code { OK, FAIL };

Code setup();

void println(const char* c);

void printHeartBeat(int heartBeat);
};  // namespace Display