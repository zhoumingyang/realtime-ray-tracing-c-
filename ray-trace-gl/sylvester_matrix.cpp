#include "sylvester_matrix.h"
#include "sylvester_vector.h"

Matrix::Matrix() {

}

Matrix::Matrix(int m, int n) {
	elements = vector<vector<float>>(m, vector<float>(n, 0));
}

Matrix::Matrix(const vector<vector<float>>& v) {
	elements = v;
}

Matrix::Matrix(float** v, int m, int n) {
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			elements[i][j] = v[i][j];
		}
	}
}

Matrix::~Matrix() {

}

vector<vector<float>> Matrix::allElements() const {
	return elements;
}

vector<float> Matrix::row(const int& i) const {
	vector<float> def;
	if (i >= elements.size()) { return def; }
	return elements[i];
}

vector<float> Matrix::col(const int& j) const {
	vector<float> def;
	if (j >= elements[0].size()) { return def; }
	vector <float> col;
	int n = elements.size(), k = n, i;
	do {
		i = k - n;
		col.push_back(elements[i][j]);
	} while (--n);
	return col;
}

void Matrix::dimesions(int& rows, int& cols) const {
	rows = elements.size();
	cols = elements[0].size();
}

int Matrix::rows() const { return elements.size(); }

int Matrix::cols() const { return elements[0].size(); }

bool Matrix::eql(const Matrix& m) const {
	vector<vector<float>> M = m.allElements();
	if (m.rows() != elements.size() || m.cols() != elements[0].size()) {
		return false;
	}
	int ni = elements.size(), ki = ni, i, nj, kj = elements[0].size(), j;
	do {
		i = ki - ni;
		nj = kj;
		do {
			j = kj - nj;
			if (abs(elements[i][j] - M[i][j]) > SYLVESTER_RRECISION) {
				return false;
			}
		} while (--nj);
	} while (--ni);
	return true;
}

Matrix Matrix::dup() const {
	return Matrix(elements);
}

Matrix Matrix::map(float(*f)(float e, int i, int j)) {
	int ni = elements.size(), ki = ni, i, nj, kj = elements[0].size(), j;
	vector<vector<float>> els(ni, vector<float>(kj, 0));
	do {
		i = ki - ni;
		nj = kj;
		do {
			j = kj - nj;
			els[i][j] = f(elements[i][j], i + 1, j + 1);
		} while (--nj);
	} while (--ni);
	return Matrix(els);
}

Matrix Matrix::zero(const int& n) const {
	vector<vector<float>> Z(n, vector<float>(n, 0));
	return Matrix(Z);
}

Matrix Matrix::zero(const int& n, const int& m) const {
	vector<vector<float>> Z(n, vector<float>(m, 0));
	return Matrix(Z);
}

Matrix Matrix::identity(const int& n) const {
	Matrix z = zero(n);
	vector<vector<float>> I = z.allElements();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i == j) {
				I[i][j] = 1.0;
			}
		}
	}
	return Matrix(I);
}

Matrix Matrix::Diagonal(const vector<float>& elements) const {
	int n = elements.size(), k = n, i;
	Matrix I = identity(n);
	vector<vector<float>> e = I.allElements();
	do {
		i = k - n;
		e[i][i] = elements[i];
	} while (--n);
	return Matrix(e);
}

bool Matrix::isSameSizeAs(Matrix mat) const {
	vector<vector<float>> M = mat.allElements();
	return (elements.size() == M.size() && elements[0].size() == M[0].size());
}

Matrix Matrix::add(Matrix mat) const {
	if (!this->isSameSizeAs(mat)) {
		return Matrix();
	}
	vector<vector<float>> M = mat.allElements();
	int ni = elements.size(), ki = ni, i, nj, kj = elements[0].size(), j;
	vector<vector<float>> els(ni, vector<float>(kj, 0));
	do {
		i = ki - ni;
		nj = kj;
		do {
			j = kj - nj;
			els[i][j] = elements[i][j] + M[i][j];
		} while (--nj);
	} while (--ni);
	return Matrix(els);
}

Matrix Matrix::subtract(const Matrix& mat) const {
	if (!this->isSameSizeAs(mat)) {
		return Matrix();
	}
	vector<vector<float>> M = mat.allElements();
	int ni = elements.size(), ki = ni, i, nj, kj = elements[0].size(), j;
	vector<vector<float>> els(ni, vector<float>(kj, 0));
	do {
		i = ki - ni;
		nj = kj;
		do {
			j = kj - nj;
			els[i][j] = elements[i][j] - M[i][j];
		} while (--nj);
	} while (--ni);
	return Matrix(els);
}

bool Matrix::canMultiplyFromLeft(const Matrix& mat) const {
	vector<vector<float>> M = mat.allElements();
	return elements[0].size() == M.size();
}

Matrix Matrix::multiply(const Matrix& mat) const {
	vector<vector<float>> M = mat.allElements();
	int mCols = mat.cols();
	if (!this->canMultiplyFromLeft(mat)) {
		return Matrix();
	}
	int ni = elements.size(), ki = ni, i, nj, kj = M[0].size(), j;
	int cols = elements[0].size(), nc, c;
	float sum = 0.0;
	vector<vector<float>> newM(ni, vector<float>(mCols, 0.0));
	do {
		i = ki - ni;
		nj = kj;
		do {
			j = kj - nj;
			sum = 0.0;
			nc = cols;
			do {
				c = cols - nc;
				sum += elements[i][c] * M[c][j];

			} while (--nc);
			newM[i][j] = sum;
		} while (--nj);
	} while (--ni);
	return Matrix(newM);
}

Matrix Matrix::multiply(const float& x) const {
	int rows = this->rows();
	int cols = this->cols();
	vector<vector<float>> newM(rows, vector<float>(cols, 0));
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			newM[i][j] = elements[i][j] * x;
		}
	}
	return Matrix(newM);
}

Vector Matrix::multiply(const Vector& vec) const {
	if (this->cols() != vec.dimensions()) {
		return Vector();
	}
	vector<float> result;
	vector<float> ele = vec.allElements();
	for (int i = 0; i < rows(); i++) {
		vector<float> tmp = row(i);
		if (tmp.size() != ele.size()) {
			return Vector();
		}
		float re = 0.0;
		for (int j = 0; j < tmp.size(); j++) {
			re += tmp[j] * ele[j];
		}
		result.push_back(re);
	}
	return Vector(result);
}

Matrix Matrix::minor(const int& startRow, const int& startCol, const int& nrows, const int& ncols) const {
	int rows = this->rows();
	int cols = this->cols();
	int ni = nrows, i, nj, j;
	vector<vector<float>> newM(nrows, vector<float>(ncols, 0));
	do {
		i = nrows - ni;
		nj = ncols;
		do {
			j = ncols - nj;
			newM[i][j] = elements[(startRow + i) % rows][(startCol + j) % cols];
		} while (--nj);
	} while (--ni);
	return Matrix(newM);
}

Matrix Matrix::transpose() const {
	int rows = this->rows();
	int cols = this->cols();
	vector<vector<float>> newM(rows, vector<float>(cols, 0));
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			newM[j][i] = elements[i][j];
		}
	}
	return Matrix(newM);
}

bool Matrix::isSquare() const {
	return elements.size() == elements[0].size();
}

float Matrix::max() const {
	int rows = this->rows();
	float max = elements[0][0];
	for (int i = 0; i < rows; i++) {
		float tmp = *max_element(elements[i].begin(), elements[i].end());
		if (tmp > max) {
			max = tmp;
		}
	}
	return max;
}

void Matrix::indexOf(const float& x, int& i, int& j) const {
	int rows = this->rows();
	i = -1;
	j = -1;
	for (int k = 0; k < rows; k++) {
		auto pos = find(elements[k].begin(), elements[k].end(), x);
		if (pos != elements[k].end()) {
			i = k;
			j = pos - elements[k].begin();
			return;
		}
	}
}

Matrix Matrix::toRightTriangular() const {
	vector<vector<float>> M = this->dup().allElements();
	int n = elements.size(), k = n, i, np, kp = elements[0].size(), p, j;
	do {
		i = k - n;
		if (M[i][i] == 0) {
			for (j = i + 1; j < k; j++) {
				if (M[j][i] != 0) {
					vector<float> els;
					np = kp;
					do {
						p = kp - np;
						els.push_back(M[i][p] + M[j][p]);
					} while (--np);
					M[i] = els;
					break;
				}
			}
		}
		if (M[i][i] != 0) {
			for (j = i + 1; j < k; j++) {
				float multiplier = elements[j][i] / elements[i][i];
				vector<float> els;
				np = kp;
				do {
					p = kp - np;
					els.push_back(p <= i ? 0 : M[j][p] - M[i][p] * multiplier);
				} while (--np);
				M[j] = els;
			}
		}
	} while (--n);
	return Matrix(M);
}

Matrix Matrix::toUpperTriangular() const {
	return this->toRightTriangular();
}

float Matrix::determinant() const {
	if (!this->isSquare()) {
		return NULL;
	}
	Matrix mat = this->toRightTriangular();
	vector<vector<float>> M = mat.allElements();
	float det = M[0][0];
	int n = M.size() - 1, k = n, i;
	do {
		i = k - n + 1;
		det = det * M[i][i];
	} while (--n);
	return det;
}

float Matrix::det() const { return this->determinant(); }

bool Matrix::isSingular() const {
	return (this->isSquare() && this->determinant() < SYLVESTER_RRECISION);
}

float Matrix::trace() const {
	if (!this->isSquare()) { return NULL; }
	float tr = elements[0][0];
	int n = elements.size() - 1, k = n, i;
	do {
		i = k - n + 1;
		tr += elements[i][i];
	} while (--n);
	return tr;
}

float Matrix::tr() const {
	return this->trace();
}

int Matrix::rank() const {
	Matrix mat = this->toRightTriangular();
	vector<vector<float>> M = mat.allElements();
	int rank = 0;
	int rows = this->rows();
	int cols = this->cols();
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (M[i][j] > SYLVESTER_RRECISION) {
				rank++;
				break;
			}
		}
	}
	return rank;
}

int Matrix::rk() const {
	return this->rank();
}

Matrix Matrix::augment(const Matrix& mat) const {
	vector<vector<float>> M = mat.allElements();
	Matrix T = this->dup();
	vector<vector<float>> TM = T.allElements();
	int cols = T.cols(), ni = T.rows(), ki = ni, i, nj, kj = M[0].size(), j;
	if (ni != M.size()) {
		return Matrix();
	}
	vector<vector<float>> newEle(T.rows(), vector<float>(T.cols() * 2, 0));
	for (int i = 0; i < T.rows(); i++) {
		for (int j = 0; j < T.cols(); j++) {
			newEle[i][j] = TM[i][j];
		}
	}
	do {
		i = ki - ni;
		nj = kj;
		do {
			j = kj - nj;
			newEle[i][cols + j] = M[i][j];
		} while (--nj);
	} while (--ni);
	return Matrix(newEle);
}

Matrix Matrix::inverse() const {
	if (!this->isSquare() || !this->isSingular()) {
		return Matrix();
	}

	int ni = elements.size(), ki = ni, i, j;
	Matrix mat = this->augment(identity(ni)).toRightTriangular();
	vector<vector<float>> M = mat.allElements();
	int np, kp = M[0].size(), p;
	float divisor, new_element;
	//vector<vector<float>> inverse_elements(mat.rows(), vector<float>(mat.cols(), 0));
	vector<vector<float>> inverse_elements(mat.rows());
	do {
		i = ni - 1;
		vector<float> els;
		np = kp;
		divisor = M[i][i];
		do {
			p = kp - np;
			new_element = M[i][p] / divisor;
			els.push_back(new_element);
			if (p >= ki) { inverse_elements[i].push_back(new_element); }
		} while (--np);
		M[i] = els;
		for (j = 0; j < i; j++) {
			vector<float> els;
			np = kp;
			do {
				p = kp - np;
				els.push_back(M[j][p] - M[i][p] * M[j][i]);
			} while (--np);
			M[j] = els;
		}
	} while (--ni);
	return Matrix(inverse_elements);
}

Matrix Matrix::inv() const {
	return this->inverse();
}

Matrix Matrix::round() const {
	int rows = this->rows();
	int cols = this->cols();
	vector<vector<float>> newM(rows, vector<float>(cols, 0));
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			newM[i][j] = std::round(elements[i][j]);
		}
	}
	return Matrix(newM);
}

Matrix Matrix::snapTo(float x) const {
	int rows = this->rows();
	int cols = this->cols();
	vector<vector<float>> newM(rows, vector<float>(cols, 0));
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			newM[i][j] = (abs(elements[i][j] - x) <= SYLVESTER_RRECISION) ? x : elements[i][j];
		}
	}
	return Matrix(newM);
}

void Matrix::setElements(Matrix mat) {
	vector<vector<float>> M = mat.allElements();
	elements = M;
}

void Matrix::setElements(vector<vector<float>> mat) {
	elements = mat;
}

Matrix Matrix::I(int n) const {
	vector<vector<float>> newM(n, vector<float>(n, 0));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			newM[i][j] = (i == j) ? 1 : 0;
		}
	}
	return Matrix(newM);
}

Matrix Matrix::rotation(const float& theta, const Vector& _axis) const {
	vector<float> axis = _axis.allElements();
	if (axis.size() == 0) {
		vector<vector<float>> newM = {
			{cos(theta), -sin(theta)},
			{sin(theta), cos(theta)}
		};
		return Matrix(newM);
	}
	if (axis.size() != 3) {
		return Matrix();
	}
	float mod = axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2];
	float x = axis[0] / mod;
	float y = axis[1] / mod;
	float z = axis[2] / mod;
	float s = sin(theta), c = cos(theta), t = 1.0 - c;
	vector<vector<float>> newM = {
		{ t*x*x + c, t*x*y - s * z, t*x*z + s * y },
		{ t*x*y + s * z, t*y*y + c, t*y*z - s * x },
		{ t*x*z - s * y, t*y*z + s * x, t*z*z + c }
	};
	return Matrix(newM);
}

Matrix Matrix::rotationX(const float& theta) const {
	float c = cos(theta), s = sin(theta);
	vector<vector<float>> newM = {
		{ 1.0,  0.0,  0.0 },
		{ 0.0, c, -s},
		{ 0.0, s, c}
	};
	return Matrix(newM);
}

Matrix Matrix::rotationY(const float& theta) const {
	float c = cos(theta), s = sin(theta);
	vector<vector<float>> newM = {
		{ c,  0.0,  s },
		{ 0.0, 1.0, 0.0 },
		{ -s, 0.0, c }
	};
	return Matrix(newM);
}

Matrix Matrix::rotationZ(const float& theta) const {
	float c = cos(theta), s = sin(theta);
	vector<vector<float>> newM = {
		{ c,  -s,  0.0 },
		{ s,   c,  0.0 },
		{ 0.0, 0.0, 1.0}
	};
	return Matrix(newM);
}

Matrix Matrix::translation(const Vector& v) const {
	vector<float> ele = v.allElements();
	if (ele.size() == 2) {
		vector<vector<float>> r = Matrix().I(3).allElements();
		r[2][0] = ele[0];
		r[2][1] = ele[1];
		return Matrix(r);
	}
	if (ele.size() == 3) {
		vector<vector<float>> r = Matrix().I(4).allElements();
		r[0][3] = ele[0];
		r[1][3] = ele[1];
		r[2][3] = ele[2];
		return Matrix(r);
	}
	return Matrix();
}

vector<float> Matrix::flatten() const {
	vector<float> result;
	if (elements.size() == 0) {
		return result;
	}

	for (int j = 0; j < elements[0].size(); j++) {
		for (int i = 0; i < elements.size(); i++) {
			result.push_back(elements[i][j]);
		}
	}
	return result;
}

Matrix Matrix::ensure4x4() const {
	if (elements.size() == 4 &&
		elements[0].size() == 4) {
		return Matrix(elements);
	}

	if (elements.size() > 4 || elements[0].size() > 4) {
		return Matrix();
	}
}