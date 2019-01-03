#include "WitchOutMatchingServer.h"

int main(int argc, char* argv[]) {
	std::locale::global(std::locale(""));
	PP::WitchOutMatchingServer Server;
	Server.Init();
	Server.Run();
	return 0;
}