#include "matrix.h"
#include <iostream>
#include <functional>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>

using std::vector;
using linear::Matrix;

void MoveConstant(Matrix<2, 1>* position){
	double theta = M_PI / 30.0;
	Matrix<2, 2> movement( {cos(theta), -sin(theta),
						    sin(theta),  cos(theta)} );

	(*position) = movement.operator*(*position);
}	

template <const int W, const int H>
class Display {
	public:
		Display<W, H>() {
			row_basis_ = H/2;
			col_basis_ = W/2;
			char characters[13] = {' ', '.', ',', '-', '~', ':', ';', '=', '!', '*', '#', '$', '@'};
			for(int i = 0; i < 13; i ++) characters_[i] = characters[i];
			InitDisplay();
		}

		void InitDisplay() {
			for(int i = 0; i < H; i ++){
				for(int j = 0; j < W; j ++) display_[i][j] = characters_[1];
			}
		}

		void SetMovement(std::function<void(Matrix<2, 1>*)> movement){
			movement_ = movement;	
		}

		void AppendObject(Matrix<2, 1> object){
			trajectory_.push_back(object);
		}


		void UpdateObject(){
			auto it = trajectory_.begin();
			Matrix<2, 1> object = *(it);

			movement_(&object);

			trajectory_.insert(it, object);
			if(trajectory_.size() > 12) trajectory_.resize(12);
		}

		void SetObjectOnDisplay() {
			display_[row_basis_][col_basis_] = '@';

			for(int i = 0; i < trajectory_.size(); i ++){
				Matrix<2, 1> position = trajectory_[i];

				int row = (int)(position[0][0]) + row_basis_;
				int col = (int)(position[1][0]*((double)W/H)) + col_basis_;
				
				display_[row][col] = characters_[12-i];
			}
		}


		void DrawAll() {
			system("clear");
			UpdateObject();
			SetObjectOnDisplay();
			for(int i = 0; i < H; i ++){
				for(int j = 0; j < W; j ++) std::cout << display_[i][j];
				std::cout << "\n";
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(167));
		}

	private:
		int row_basis_;
		int col_basis_;

		vector<Matrix<2, 1>> trajectory_;
		char display_[H][W];
		//characters_ length is 13
		char characters_[13];

		std::function<void(Matrix<2, 1>*)> movement_;
};

int main(){
	Matrix<2, 1> a({18, 0});

	Display<100, 50> display;
	
	display.SetMovement(MoveConstant);
	display.AppendObject(a);
	while(true){
		display.DrawAll();
	}

	return 0;
}
