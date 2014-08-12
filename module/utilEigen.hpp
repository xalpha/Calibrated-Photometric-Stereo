#ifndef _UTILEIGEN_H_
#define _UTILEIGEN_H_

/*!
 * \file utilEigen.hpp
 *
 * \author Yuji Oyamada
 * \date 2014/04/20
 * \brief This file is utility to use Eigen.
 *
 */
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
// for Eigen
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/StdVector>

//! @brief computes Moore Penrose pseudo inverse matrix.

//! case1: invA = A^T * (A * A^T)^{-1} <br>
//! case2: invA = (A^T * A)^{-1} * A^T <br>
//! @param[in]	A	the original matrix
template <typename DataType>
Eigen::Matrix<DataType, -1, -1> pinvMoorePenrose(
    const Eigen::Matrix<DataType, -1, -1> &A
)
{
    int m = A.rows(), n = A.cols();
    Eigen::Matrix<DataType, -1, -1> AT, Ainv;

    if( m == n )
    {
        Ainv = A.inverse();
    }
    else
    {
        AT = A.transpose();
        if( m > n )
        {
            Ainv.noalias() = (AT * A).colPivHouseholderQr().inverse() * AT;
        }
        else // m < n
        {
            Ainv.noalias() = (AT * A).colPivHouseholderQr().inverse() * AT;
        }
    }

    return Ainv;
}


//! @brief computes inverse matrix of sigma matrix.

//! @param[in]	svd		object for SVD
//! @param[in]	epsilon	torelance
//! @return		computed pseudo inverse matrix
template <typename DataType>
Eigen::Matrix<DataType, -1, -1> getInverseSigma(
    const Eigen::JacobiSVD< Eigen::Matrix<DataType, -1, -1> > &svd,
    double epsilon = 1e-6
)
{
    Eigen::Matrix<DataType, -1, -1> vecSV = svd.singularValues();
    Eigen::Matrix<DataType, -1, -1> Sinv;
    int n = vecSV.size();

    Sinv = Eigen::Matrix<DataType, -1, -1>::Zero( svd.matrixV().cols(), svd.matrixU().cols() );
    for( int i = 0; i < n; ++i )
    {
        if( vecSV(i) > epsilon )
        {
            Sinv(i,i) = 1.0 / vecSV(i);
        }
    }

    return Sinv;
}

//! @brief computes pseudo inverse matrix by using Singular Value Decomposition (SVD).

//! @param[in]	A	the original matrix
template <typename DataType>
Eigen::Matrix<DataType, -1, -1> pinvSvdThin(
    const Eigen::Matrix<DataType, -1, -1>&A
)
{
    int m = A.rows(), n = A.cols();
    Eigen::Matrix<DataType, -1, -1> Sinv, Ainv;

    Eigen::JacobiSVD< Eigen::Matrix<DataType, -1, -1> > svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Sinv = getInverseSigma( svd );
    Ainv = svd.matrixV() * Sinv * svd.matrixU().transpose();

    return Ainv;
}

//! computes pseudo inverse matrix by using Singular Value Decomposition (SVD).

//! @param[in]	A	the original matrix
//! @return			computed pseudo inverse matrix
template <typename DataType>
Eigen::Matrix<DataType, -1, -1> pinvSvdFull(
    const Eigen::Matrix<DataType, -1, -1>&A
)
{
    int m = A.rows(), n = A.cols();
    Eigen::Matrix<DataType, -1, -1> Sinv, Ainv;

    Eigen::JacobiSVD< Eigen::Matrix<DataType, -1, -1> > svd(A, Eigen::ComputeFullU | Eigen::ComputeFullV);
    Sinv = getInverseSigma( svd );
    Ainv = svd.matrixV() * Sinv * svd.matrixU().transpose();

    return Ainv;
}

//! @brief computes inverse matrix.

//! @param[in]	A	the original matrix
//! @param[in]	idx	index for specifying algorithm computing pseudo inverse matrix
//! @return			computed pseudo inverse matrix
template <typename DataType>
Eigen::Matrix<DataType, -1, -1> pinv(
    const Eigen::Matrix<DataType, -1, -1>& A,
    const int idx = 0
)
{
    Eigen::Matrix<DataType, -1, -1> Ainv;
    switch( idx )
    {
    case 1:
        Ainv = pinvMoorePenrose( A );
        break;
    case 2:
        Ainv = pinvSvdThin( A );
        break;
    default:
        Ainv = pinvSvdFull( A );
        break;
    }

    return Ainv;
}
#endif
