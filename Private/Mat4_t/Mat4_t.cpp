#include "../../Public/Mat4_t/Mat4_t.h"

Mat4_t::Mat4_t(){
    mat.clear();
    mat.resize(4,std::vector<double>(4,0));
}

Mat4_t::Mat4_t(double _mat[4][4]){
    for(int i = 0;i < 4; i++) {
        for(int j = 0;j < 4; j++) {
            mat[i][j] = _mat[i][j];
        }
    }
}

Mat4_t::Mat4_t(const Mat4_t& _mat){
    mat = _mat.mat;
}

Mat4_t::~Mat4_t()= default;
Mat4_t& Mat4_t::operator=(const Mat4_t& _mat)= default;

Mat4_t Mat4_t::operator+(const Mat4_t& _mat) const {
    Mat4_t answer;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            answer.mat[i][j] = this->mat[i][j] + _mat.mat[i][j];
        }
    }
    return answer;
}
Mat4_t Mat4_t::operator-(const Mat4_t& _mat) const {
    Mat4_t answer;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            answer.mat[i][j] = this->mat[i][j] - _mat.mat[i][j];
        }
    }
    return answer;
}
Mat4_t Mat4_t::operator*(const Mat4_t& _mat) const {
    Mat4_t answer;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            for(int k = 0; k < 4; k++) {
                answer.mat[i][j] += this->mat[i][k] * _mat.mat[k][j];
            }
        }
    }
    return answer;
}
Vec4_t Mat4_t::operator*(const Vec4_t& _vec) const {
    const std::vector<double> aux = {_vec.getX(), _vec.getY(), _vec.getZ(), _vec.getW()};
    std::vector<double> answer(4, 0);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++) {
            answer[i] += mat[i][j] * aux[j];
        }
    }
    return Vec4_t(answer);
}

Mat4_t Mat4_t::identity() {
    auto answer = Mat4_t();
    for(int i = 0; i < 4; i++) {
        answer.mat[i][i] = 1;
    }
    return answer;
}

Mat4_t Mat4_t::isometric() {
    Mat4_t answer;
    answer.mat[0][0] = answer.mat[0][2] = sqrt(2)/2;
    answer.mat[1][0] = sqrt(6)/6;
    answer.mat[1][1] = answer.mat[1][0]*2;
    answer.mat[1][2] = -answer.mat[1][0];
    answer.mat[2][1] = answer.mat[2][2] = sqrt(3)/3;
    answer.mat[2][0] = -answer.mat[2][1];
    answer.mat[3][3] = 1;
    return answer;
}

Mat4_t Mat4_t::transpose() const {
    Mat4_t answer;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            answer.mat[i][j] = this->mat[j][i];
        }
    }
    return answer;
}

Mat4_t Mat4_t::make_scale(const double _sx, const double _sy, const double _sz){
    Mat4_t answer;
    answer.mat[0][0] = _sx;
    answer.mat[1][1] = _sy;
    answer.mat[2][2] = _sz;
    answer.mat[3][3] = 1;
    return answer;
}

Mat4_t Mat4_t::make_translate(const double _tx, const double _ty, const double _tz) {
    Mat4_t answer = Mat4_t::identity();
    answer.mat[0][3] = _tx;
    answer.mat[1][3] = _ty;
    answer.mat[2][3] = _tz;
    return answer;
}

Mat4_t Mat4_t::make_rx(const double angle) {
    Mat4_t answer;
    answer.mat[0][0] = 1;
    answer.mat[1][1] = cos(angle);
    answer.mat[2][1] = sin(angle);
    answer.mat[1][2] = -sin(angle);
    answer.mat[2][2] = cos(angle);
    answer.mat[3][3] = 1;
    return answer;
}

Mat4_t Mat4_t::make_ry(const double angle) {
    Mat4_t answer;
    answer.mat[0][0] = cos(angle);
    answer.mat[1][1] = 1;
    answer.mat[2][2] = cos(angle);
    answer.mat[3][3] = 1;
    answer.mat[2][0] = sin(angle);
    answer.mat[0][2] = -sin(angle);
    return answer;
}
Mat4_t Mat4_t::make_rz(const double angle) {
    Mat4_t answer;
    answer.mat[0][0] = cos(angle);
    answer.mat[1][1] = cos(angle);
    answer.mat[1][0] = sin(angle);
    answer.mat[0][1] = -sin(angle);
    answer.mat[2][2] = 1;
    answer.mat[3][3] = 1;
    return answer;
}

Mat4_t Mat4_t::make_rotate(const double _rx, const double _ry, const double _rz) {
    Mat4_t answer;
    return make_rx(_rx) * make_ry(_ry) * make_rz(_rz);
}