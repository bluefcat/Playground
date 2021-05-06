#include <iostream>
#include <string>
#include <thread>
#include <chrono>

int main(){
	char display[50][100];

	for(int i = 0; i < 50; i ++){
		for(int j = 0; j < 100; j ++) display[i][j] = '.';
	}

	display[1][1] = '@';

	std::string buffer2;
	for(int i = 0; i < 50; i ++){
		for(int j = 0; j < 100; j ++) buffer2 += display[i][j];
		buffer2 += "\n";
	}


	while(true){
		system("clear");

		std::cout << buffer2;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}
