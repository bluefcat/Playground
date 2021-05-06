#include <initializer_list>

namespace linear {
	template <const int R, const int C>
	class Matrix {
		public:
			Matrix<R, C>(const double scalar = 0){
				for(int row = 0; row < R; row ++){
					for(int col = 0; col < C; col ++) data_[row*C+col] = (row == col)? scalar: 0;
				}
			}

			Matrix<R, C>(std::initializer_list<double> data){
				for(int row = 0; row < R; row ++){
					for(int col = 0; col < C; col ++) data_[row*C+col] = *(data.begin()+row*C+col);
				}
			}

			Matrix<R, C>(const Matrix<R, C>& matrix){
				for(int row = 0; row < R; row ++){
					for(int col = 0; col < C; col ++) data_[row*C+col] = matrix.data_[row*C+col];
				}
			}
			
			Matrix<R, C>(Matrix<R, C>&& matrix){
				for(int row = 0; row < R; row ++){
					for(int col = 0; col < C; col ++) data_[row*C+col] = matrix.data_[row*C+col];
				}
			}
			
			~Matrix<R, C>(){
			}

			double* operator[](int row) {
				return data_+row*C;
			}
			
			bool operator==(const Matrix<R, C>& other) {
				bool result = true;
				for(int row = 0; row < R; row ++){
					for(int col = 0; col < C; col ++) result &= (data_[row*C+col] == other.data_[row*C+col]);
				}
				return result;
			}

			bool operator!=(const Matrix<R, C>& other) {
				bool result = false;
				for(int row = 0; row < R; row ++){
					for(int col = 0; col < C; col ++) result |= (data_[row*C+col] != other.data_[row*C+col]);
				}
				return result;
			}
			
			Matrix<R, C>& operator= (const Matrix<R, C>& other) {
				for(int row = 0; row < R; row ++){
					for(int col = 0; col < C; col ++) data_[row*C+col] = other.data_[row*C+col];
				}

				return *this;
			}

			const Matrix<R, C> operator+ (const Matrix<R, C>& other) const noexcept {
				Matrix<R, C> result(0);
				for(int row = 0; row < R; row ++){
					for(int col = 0; col < C; col ++){
						result[row][col] = data_[row*C+col]+other.data_[row*C+col];
					}
				}
				return result;
			}
			
			const Matrix<R, C> operator- (const Matrix<R, C>& other) const noexcept {
				Matrix<R, C> result(0);
				for(int row = 0; row < R; row ++){
					for(int col = 0; col < C; col ++){
						result[row][col] = data_[row*C+col]+other.data_[row*C+col];
					}
				}
				return result;
			}

			Matrix<R, C>& operator+= (const Matrix<R, C>& other) const {
				for(int row = 0; row < R; row ++){
					for(int col = 0; col < C; col ++){
						this->data_[row*C+col] += other.data_[row*C+col];
					}
				}

				return *this;
			}

			Matrix<R, C>& operator-= (const Matrix<R, C>& other) const {
				for(int row = 0; row < R; row ++){
					for(int col = 0; col < C; col ++){
						this->data_[row*C+col] -= other.data_[row*C+col];
					}
				}

				return *this;
			}

			friend const Matrix<R, C> operator*(const double scalar, Matrix<R, C>& matrix){
				Matrix<R, C> result(0);
				for(int row = 0; row < R; row ++){
					for(int col = 0; col < C; col ++){
						result[row][col] = scalar * matrix[row][col];
					}
				}
				return result;
			}

			friend Matrix<R, C>& operator*=(Matrix<R, C>& matrix, const double scalar) {
				for(int row = 0; row < R; row ++){
					for(int col = 0; col < C; col ++){
						matrix[row][col] *= scalar;
					}
				}

				return matrix;
			}
			
			template<const int K>
			Matrix<R, K> operator*(Matrix<C, K>& other) {
				Matrix<R, K> result(0);
				for(int row = 0; row < R; row ++){
					for(int mid = 0; mid < C; mid ++){
						for(int col = 0; col < K; col ++) {
							result[row][col] += data_[row*C+mid] * other[mid][col];
						}
					}
				}
				return result;
			}

		private:
			double data_[R*C];
			using type_ = Matrix<R, C>;
	};
}
