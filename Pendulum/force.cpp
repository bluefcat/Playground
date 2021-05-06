#include "matrix.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

using linear::Matrix;

int main(){
	//object info
	Matrix<2, 1> pos({.4, 0.1});
	Matrix<2, 1> v({0, 0});

	double m = 5.0;
	double l = sqrt(pow(pos[0][0], 2) + pow(pos[1][0], 2));


	//env info
	double g = -4.8;
	double dt = 0.01;

	char display[32][62];

	for(int i = 0; i < 32; i ++) for(int j = 0; j < 62; j ++) display[i][j] = ' ';
	display[15][31] = '@';

	
	while(true){
		system("clear");
		for(int i = 0; i < 32; i ++){
			for(int j = 0; j < 62; j ++) putchar(display[i][j]);
			putchar('\n');
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
		int prow = (int)(25*pos[0][0]+30);
		int pcol = (int)(15*pos[0][1]+15);
		display[pcol][prow] = ' ';

		v[0][0] += m*g*(pos[0][0]/l)*dt;
		pos[0][0] += v[0][0]*dt;
		pos[1][0] = sqrt(l*l-pos[0][0]*pos[0][0]);

		int row = (int)(25*pos[0][0]+30);
		int col = (int)(15*pos[1][0]+15);
		display[col][row] = '@';

	}

	return 0;
}
