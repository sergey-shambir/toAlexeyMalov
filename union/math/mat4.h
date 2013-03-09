#pragma once
#include <string.h>
#include <algorithm>
#include "mat3.h"
#include "vec3.h"
#include "vec4.h"

namespace base {

template<class treal>
class mat4
{
public:
    mat4(void)
    {
        loadIdentity();
    }

    explicit mat4(const treal *src)
    {
        memcpy(data, src, sizeof(data));
    }

    /**
      Direct access
     */
    operator const treal*()const
    {
        return &data[0];
    }

    /** Multiply */

    mat4& operator*=(mat4 const& rhs)
    {
        treal result[4][4];

        result[0][0] = a00*rhs.a00 + a01*rhs.a10 + a02*rhs.a20 + a03*rhs.a30;
        result[0][1] = a10*rhs.a00 + a11*rhs.a10 + a12*rhs.a20 + a13*rhs.a30;
        result[0][2] = a20*rhs.a00 + a21*rhs.a10 + a22*rhs.a20 + a23*rhs.a30;
        result[0][3] = a30*rhs.a00 + a31*rhs.a10 + a32*rhs.a20 + a33*rhs.a30;

        result[1][0] = a00*rhs.a01 + a01*rhs.a11 + a02*rhs.a21 + a03*rhs.a31;
        result[1][1] = a10*rhs.a01 + a11*rhs.a11 + a12*rhs.a21 + a13*rhs.a31;
        result[1][2] = a20*rhs.a01 + a21*rhs.a11 + a22*rhs.a21 + a23*rhs.a31;
        result[1][3] = a30*rhs.a01 + a31*rhs.a11 + a32*rhs.a21 + a33*rhs.a31;

        result[2][0] = a00*rhs.a02 + a01*rhs.a12 + a02*rhs.a22 + a03*rhs.a32;
        result[2][1] = a10*rhs.a02 + a11*rhs.a12 + a12*rhs.a22 + a13*rhs.a32;
        result[2][2] = a20*rhs.a02 + a21*rhs.a12 + a22*rhs.a22 + a23*rhs.a32;
        result[2][3] = a30*rhs.a02 + a31*rhs.a12 + a32*rhs.a22 + a33*rhs.a32;

        result[3][0] = a00*rhs.a03 + a01*rhs.a13 + a02*rhs.a23 + a03*rhs.a33;
        result[3][1] = a10*rhs.a03 + a11*rhs.a13 + a12*rhs.a23 + a13*rhs.a33;
        result[3][2] = a20*rhs.a03 + a21*rhs.a13 + a22*rhs.a23 + a23*rhs.a33;
        result[3][3] = a30*rhs.a03 + a31*rhs.a13 + a32*rhs.a23 + a33*rhs.a33;
        memcpy(data, result, sizeof(data));

        return *this;
    }

    mat4 const operator*(mat4 const& rhs)const
    {
        treal result[4][4];

        result[0][0] = a00*rhs.a00 + a01*rhs.a10 + a02*rhs.a20 + a03*rhs.a30;
        result[0][1] = a10*rhs.a00 + a11*rhs.a10 + a12*rhs.a20 + a13*rhs.a30;
        result[0][2] = a20*rhs.a00 + a21*rhs.a10 + a22*rhs.a20 + a23*rhs.a30;
        result[0][3] = a30*rhs.a00 + a31*rhs.a10 + a32*rhs.a20 + a33*rhs.a30;

        result[1][0] = a00*rhs.a01 + a01*rhs.a11 + a02*rhs.a21 + a03*rhs.a31;
        result[1][1] = a10*rhs.a01 + a11*rhs.a11 + a12*rhs.a21 + a13*rhs.a31;
        result[1][2] = a20*rhs.a01 + a21*rhs.a11 + a22*rhs.a21 + a23*rhs.a31;
        result[1][3] = a30*rhs.a01 + a31*rhs.a11 + a32*rhs.a21 + a33*rhs.a31;

        result[2][0] = a00*rhs.a02 + a01*rhs.a12 + a02*rhs.a22 + a03*rhs.a32;
        result[2][1] = a10*rhs.a02 + a11*rhs.a12 + a12*rhs.a22 + a13*rhs.a32;
        result[2][2] = a20*rhs.a02 + a21*rhs.a12 + a22*rhs.a22 + a23*rhs.a32;
        result[2][3] = a30*rhs.a02 + a31*rhs.a12 + a32*rhs.a22 + a33*rhs.a32;

        result[3][0] = a00*rhs.a03 + a01*rhs.a13 + a02*rhs.a23 + a03*rhs.a33;
        result[3][1] = a10*rhs.a03 + a11*rhs.a13 + a12*rhs.a23 + a13*rhs.a33;
        result[3][2] = a20*rhs.a03 + a21*rhs.a13 + a22*rhs.a23 + a23*rhs.a33;
        result[3][3] = a30*rhs.a03 + a31*rhs.a13 + a32*rhs.a23 + a33*rhs.a33;

        return mat4(&result[0][0]);
    }

    inline vec3<treal> transform3(vec3<treal> const& rhs) const
    {
        vec4<treal> temp;
        temp.x = a00 * rhs.x + a01 * rhs.y + a02 * rhs.z;
        temp.y = a10 * rhs.x + a11 * rhs.y + a12 * rhs.z;
        temp.z = a20 * rhs.x + a21 * rhs.y + a22 * rhs.z;
        return vec3<treal>(temp.x, temp.y, temp.z);
    }

    inline vec4<treal> transform4(vec4<treal> const& rhs) const
    {
        vec4<treal> ret;
        ret.x = a00 * rhs.x + a01 * rhs.y + a02 * rhs.z + a03 * rhs.w;
        ret.y = a10 * rhs.x + a11 * rhs.y + a12 * rhs.z + a13 * rhs.w;
        ret.z = a20 * rhs.x + a21 * rhs.y + a22 * rhs.z + a23 * rhs.w;
        ret.w = a30 * rhs.x + a31 * rhs.y + a32 * rhs.z + a33 * rhs.w;
        // FIXME: there i have seen such code:
//         ret.w = a30 * rhs.x + a31 * rhs.y + a32 * rhs.z + a03 * rhs.w;

        return ret;
    }

    inline vec4<treal> operator*(vec4<treal> const& rhs) const
    {
        return transform4(rhs);
    }

    inline vec4<treal> row(size_t index) const
    {
        return base::vec4<treal>(mat[index]);
    }

    inline vec4<treal> column(size_t index) const
    {
        return vec4<treal>(
                    mat[0][index],
                    mat[1][index],
                    mat[2][index],
                    mat[3][index]
                );
    }

    inline mat4<treal> &setRow(size_t index, const vec3<treal> &value, treal w = 0.0)
    {
        mat[0][index] = value.x;
        mat[1][index] = value.y;
        mat[2][index] = value.z;
        mat[3][index] = w;
        return *this;
    }

    inline mat4<treal> &setColumn(size_t index, const vec3<treal> &value, treal w = 0.0)
    {
        mat[index][0] = value.x;
        mat[index][1] = value.y;
        mat[index][2] = value.z;
        mat[index][3] = w;
        return *this;
    }

    inline float at(size_t row, size_t column)
    {
        return data[4 * row + column];
    }

    inline mat4<treal> &translate(treal dx = 0.0, treal dy = 0.0, treal dz = 0.0)
    {
        mat4 temp;
        temp.x = dx;
        temp.y = dy;
        temp.z = dz;
        return *this *= temp;
    }

    inline mat4<treal> &translate(const vec3<treal> &direction)
    {
        return translate(direction.x, direction.y, direction.z);
    }

    inline mat4<treal> &scaleAllSides(treal scaleCoeff = 1.0)
    {
        return scale(scaleCoeff, scaleCoeff, scaleCoeff);
    }

    inline mat4<treal> &scale(treal sx = 1.0, treal sy = 1.0, treal sz = 1.0)
    {
        mat4 temp;
        temp.sx = sx;
        temp.sy = sy;
        temp.sz = sz;
        return *this *= temp;
    }

    inline mat4<treal> &scale(const vec3<treal> &v)
    {
        return scale(v.x, v.y, v.z);
    }

    inline mat4<treal> &rotate(treal angle, treal x, treal y, treal z)
    {
        mat4 temp;
        temp.loadRotation(angle, x, y, z);
        return *this *= temp;
    }

    /**
     * @param angle - use degrees, not radians
     */
    inline mat4<treal> &rotate(treal angle, vec3<treal> const& v)
    {
        return rotate(angle, v.x, v.y, v.z);
    }

    inline mat4<treal> &lookAtRh(vec3<treal> const& eye,
                                 vec3<treal> const& at,
                                 vec3<treal> const& up)
    {
        mat4 temp;
        temp.loadLookAtRH(eye, at, up);
        return *this *= temp;
    }

    inline mat4<treal> &transpose()
    {
        std::swap(data[1], data[4]);
        std::swap(data[2], data[8]);
        std::swap(data[3], data[12]);
        std::swap(data[6], data[9]);
        std::swap(data[7], data[13]);
        std::swap(data[11], data[14]);
        return *this;
    }

    /** Determinant and inverse matrix */

    // FIXME: probably incorrect, see commented code
    treal getDeterminant() const
    {
        mat3<treal> m0(getMinorMatrix(0, 0));
        mat3<treal> m1(getMinorMatrix(0, 1));
        mat3<treal> m2(getMinorMatrix(0, 2));
       // mat3<treal> m3(getMinorMatrix(0, 3));
        return
            a00 * m0.getDeterminant() -
            a01 * m1.getDeterminant() +
            a02 * m1.getDeterminant() -
            a03 * m2.getDeterminant();
    }

    /**
       \brief Calculates inverse matrix
     */
    mat4<treal> getInverseMatrix()const
    {
        const treal det = getDeterminant();
        const treal invDet = 1.0 / det;
        return mat3<treal>(
            // column 0
            +getMinorMatrix(0, 0).getDeterminant() * invDet,
            -getMinorMatrix(0, 1).getDeterminant() * invDet,
            +getMinorMatrix(0, 2).getDeterminant() * invDet,
            -getMinorMatrix(0, 3).getDeterminant() * invDet,
            // column 1
            -getMinorMatrix(1, 0).getDeterminant() * invDet,
            +getMinorMatrix(1, 1).getDeterminant() * invDet,
            -getMinorMatrix(1, 2).getDeterminant() * invDet,
            +getMinorMatrix(1, 3).getDeterminant() * invDet,
            // column 2
            +getMinorMatrix(2, 0).getDeterminant() * invDet,
            -getMinorMatrix(2, 1).getDeterminant() * invDet,
            +getMinorMatrix(2, 2).getDeterminant() * invDet,
            -getMinorMatrix(2, 3).getDeterminant() * invDet,
            // column 3
            -getMinorMatrix(3, 0).getDeterminant() * invDet,
            +getMinorMatrix(3, 1).getDeterminant() * invDet,
            -getMinorMatrix(3, 2).getDeterminant() * invDet,
            +getMinorMatrix(3, 3).getDeterminant() * invDet
            );
    }

    /*
    Вычисление минорной матрицы 3x3, получающейся путем исключения строки
    с индексом row и столбца с индексом col
    */
    mat3<treal> getMinorMatrix(unsigned row, unsigned column)const
    {
        /*
        Данная lookup-таблица указывает какие номера столбцов или строк
        следует брать для получения матрицы 3x3.
        Позволяет избежат излишних ветвлений и циклов
        */
        static const unsigned indices[4][3] =
        {
            {1, 2, 3},	//	Строки (столбцы), получающиеся при исключении 0-й строки (столбца)
            {0, 2, 3},	//	Строки (столбцы), получающиеся при исключении 1-й строки (столбца)
            {0, 1, 3},	//	Строки (столбцы), получающиеся при исключении 2-й строки (столбца)
            {0, 1, 2},	//	Строки (столбцы), получающиеся при исключении 3-й строки (столбца)
        };
        /*
        Формируем индексы строк и столбцов, выбираемых ля построения матрицы 3*3
        */
        unsigned c0 = indices[column][0];
        unsigned c1 = indices[column][1];
        unsigned c2 = indices[column][2];
        unsigned r0 = indices[row][0];
        unsigned r1 = indices[row][1];
        unsigned r2 = indices[row][2];

        /*
        Возвращаем результат
        */
        return mat3<treal>(
            mat[c0][r0], mat[c0][r1], mat[c0][r2],	// столбец 0
            mat[c1][r0], mat[c1][r1], mat[c1][r2],	// столбец 1
            mat[c2][r0], mat[c2][r1], mat[c2][r2]	// столбец 2
        );
    }

    /** Loading functions */

    inline mat4<treal> &loadIdentity()
    {
        static const treal identityMatrix[4][4] =
        {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1},
        };
        memcpy(data, identityMatrix, sizeof(identityMatrix));
        return *this;
    }

    inline mat4<treal> &loadTranslation(treal dx, treal dy, treal dz)
    {
        loadIdentity();
        x = dx;
        y = dy;
        z = dz;
        return *this;
    }

    inline mat4<treal> &loadScale(treal sx0, treal sy0, treal sz0)
    {
        loadIdentity();
        sx = sx0;
        sy = sy0;
        sz = sz0;
        return *this;
    }

    /**
     * @param angle - Use degrees, not radians
     */
    inline mat4<treal> &loadRotation(treal angle, treal ux, treal uy, treal uz)
    {
        angle = (treal)(angle * 3.14159265358979323846 / 180.0);

        treal inv_len = (treal)(1.0 / sqrt(ux * ux + uy * uy + uz * uz));
        ux *= inv_len;
        uy *= inv_len;
        uz *= inv_len;

        treal c = (treal)cos(angle);
        treal s = (treal)sin(angle);
        treal one_minus_c = (treal)1 - c;

        treal uxuy = ux * uy;
        treal uxuz = ux * uz;
        treal uyuz = uy * uz;

        treal sux = s * ux;
        treal suy = s * uy;
        treal suz = s * uz;

        a00 = c + one_minus_c * ux * ux;
        a10 = one_minus_c * uxuy + suz;
        a20 = one_minus_c * uxuz - suy;

        a01 = one_minus_c * uxuy - suz;
        a11 = c + one_minus_c * uy * uy;
        a21 = one_minus_c * uyuz + sux;

        a02 = one_minus_c * uxuz + suy;
        a12 = one_minus_c * uyuz - sux;
        a22 = c + one_minus_c * uz * uz;

        a30 = a31 = a32 = a03 = a13 = a23 = 0;
        a33 = 1;

        return *this;
    }

    inline mat4<treal> &loadRotation(treal angle, const vec3<treal> &vec)
    {
        loadRotation(angle, vec.x, vec.y, vec.z);
        return *this;
    }

    inline mat4<treal> &loadFrustum(treal left, treal right,
                                    treal bottom, treal top,
                                    treal zNear, treal zFar)
    {
        const treal n2 = zNear + zNear;
        const treal invRminusL = 1 / (right - left);
        const treal invFminusN = 1 / (zFar - zNear);
        const treal invTminusB = 1 / (top - bottom);

        a00 = n2 * invRminusL;	// 2*near/(right-left);
        a10 = a20 = a30 = a01 = 0;
        a11 = n2 * invTminusB;	// 2*near/(top-bottom)
        a21 = a31 = 0;
        a02 = (right + left) * invRminusL;	// (right+left)/(right-left);
        a12 = (top + bottom) * invTminusB;	// (top+bottom)/(top-bottom)
        a22 = -(zFar + zNear) * invFminusN;	// -(far+near)/(far-near)
        a32 = -1;
        a03 = a13 = 0;
        a23 = -n2 * zFar * invFminusN;		// -2*far*near/(far-near)
        a33 = 0;
        return *this;
    }

    inline mat4<treal> &loadFrustum(const vec4<treal> &viewport, treal zNear, treal zFar)
    {
        loadFrustum(viewport.x, viewport.y, viewport.z, viewport.w, zNear, zFar);
        return *this;
    }

    inline mat4<treal> &loadPerspective(treal fovY, treal aspect, treal zNear, treal zFar)
    {
        fovY = (treal)(fovY * 3.14159265358979323846 / 180);
        treal top = zNear * tan(fovY * 0.5);
        treal right = aspect * top;
        return loadFrustum(-right, right, -top, top, zNear, zFar);
    }

    /** gluLookAt clone */

    mat4<treal> & loadLookAtRH(treal eyeX, treal eyeY, treal eyeZ,
                               treal atX, treal atY, treal atZ,
                               treal upX, treal upY, treal upZ)
    {
        vec3<treal> eye(eyeX, eyeY, eyeZ);
        vec3<treal> at(atX, atY, atZ);
        vec3<treal> up(upX, upY, upZ);

        return loadLookAtRH(eye, at, up);
    }

    mat4<treal> & loadLookAtRH(vec3<treal> const& eye,
                               vec3<treal> const& at,
                               vec3<treal> const& up)
    {
        vec3<treal> zaxis = (eye - at);
        zaxis.normalize();
        vec3<treal> xaxis = cross(up, zaxis);
        xaxis.normalize();
        vec3<treal> yaxis = cross(zaxis, xaxis);
        yaxis.normalize();

        setRow(0, xaxis);
        setRow(1, yaxis);
        setRow(2, zaxis);
        clearRow(3);
        x = -dot(xaxis, eye);
        y = -dot(yaxis, eye);
        z = -dot(zaxis, eye);
        w = 1.0;

        return *this;
    }

    void clearColumn(size_t i)
    {
        mat[i][0] = 0;
        mat[i][1] = 0;
        mat[i][2] = 0;
        mat[i][3] = 0;
    }

    void clearRow(size_t i)
    {
        mat[0][i] = 0;
        mat[1][i] = 0;
        mat[2][i] = 0;
        mat[3][i] = 0;
    }

    void normalize()
    {
        /*
        Ортонормирование - приведение координатных осей к единичной длине (нормирование)
        и взаимной перпендикулярности (ортогонализация)
        Достичь этого можно при помощи нормализации координатных осей
        и векторного произведения
        */
        vec3<treal> xAxis(data[0], data[4], data[8]);
        xAxis.normalize();
        vec3<treal> yAxis(data[1], data[5], data[9]);
        yAxis.normalize();

        // Ось Z вычисляем через векторное произведение X и Y
        // Z будет перпендикулярна плоскости векторов X и Y
        vec3<treal> zAxis = cross(xAxis, yAxis);
        // И иметь единичную длину
        zAxis.normalize();
        // То же самое проделываем с осями x и y
        xAxis = cross(yAxis, zAxis);
        xAxis.normalize();

        yAxis = cross(zAxis, xAxis);
        yAxis.normalize();

        // Сохраняем вектора координатных осей обратно в матрицу
        setRow(0, xAxis, x);
        setRow(1, yAxis, y);
        setRow(2, zAxis, z);
    }

    /** Data */
    union
    {
        struct
        {
            union
            {
                treal a00;
                treal sx;
            };
            treal a10, a20;
            union
            {
                treal a30;
                treal p;
            };

            treal a01;
            union
            {
                treal a11;
                treal sy;
            };
            treal a21;
            union
            {
                treal a31;
                treal q;
            };

            treal a02;
            treal a12;
            union
            {
                treal a22;
                treal sz;
            };
            union
            {
                treal a32;
                treal r;
            };

            union
            {
                treal a03;
                treal x;
            };
            union
            {
                treal a13;
                treal y;
            };
            union
            {
                treal a23;
                treal z;
            };
            union
            {
                treal a33;
                treal w;
            };
        };

        treal mat[4][4];
        treal data[16];
    };
};

} // namespace base

typedef base::mat4<float> mat4;
typedef base::mat4<double> mat4d;
