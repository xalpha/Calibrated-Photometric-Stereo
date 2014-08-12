#ifndef __PHOTOMETRICSTEREOSOLVER_H__
#define __PHOTOMETRICSTEREOSOLVER_H__

/*!
 * \file PhotometricStereoSolver.hpp
 *
 * \author Yuji Oyamada
 * \date 2014/05/18
 * \brief This file contains solvers for photometric stereo problems.
 *
 */

// STL
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <limits>

// internal headers
#include "utilEigen.hpp"
#include "utilString.hpp"
#include "DataStructure.hpp"
#include "Image.hpp"
#include "CpsConfiguration.hpp"

void showMatrix(
    const Eigen::MatrixXf& mat
)
{
    int cols = mat.cols();
    int rows = mat.rows();
    for(int r = 0; r < rows; ++r)
    {
        for(int c = 0; c < cols; ++c)
        {
            std::cout << mat(r,c) << "  ";
        }
        std::cout << std::endl;
    }
}

void loadAvailablePixels(
    const std::string strImageMask,
    int& width,
    int& height,
    std::vector<int>& indexOfPixels
)
{

    ImageSingle<unsigned char, int> imgMask(strImageMask);

    width = imgMask._width();
    height = imgMask._height();
    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            if( imgMask(x,y,0) == 255 )
            {
                indexOfPixels.push_back( y*width+x );
            }
        }
    }
}

template <typename DataType>
inline Eigen::Matrix<DataType, -1, -1> buildObservationMatrix(
    const std::vector<int>& indexOfPixels,
    const std::vector<CPS::ObservationSingle>& obsSingle,
    const int color,
    const int width
)
{
    int numberOfPixels = indexOfPixels.size();
    int numberOfImages = obsSingle.size();

    Eigen::Matrix<DataType, -1, -1> I = Eigen::Matrix<DataType, -1, -1>::Zero(numberOfPixels*color, numberOfImages);
    typename ImagePixel<DataType>::PixelValue pixelValue;

    std::cout << "build I of " << numberOfPixels*color << "x" << numberOfImages << " matrix" << std::endl;
    for(int f = 0; f < numberOfImages; ++f)
    { // f means "f"rame
        ImageSingle<DataType, DataType> img( obsSingle[f].strImage() );
        for(int p = 0; p < numberOfPixels; ++p)
        { // p means "p"ixel
            pixelValue = img(indexOfPixels[p]%width, indexOfPixels[p]/width);
            for(int c = 0; c < color; ++c)
            { // c means "c"olor
                I(c*numberOfPixels+p,f) = pixelValue[c];
            }
        }
    }

    return I;
}

template <typename DataType>
inline Eigen::Matrix<DataType, -1, -1> buildLightSourceMatrix(
    const std::vector<CPS::ObservationSingle>& obsSingle
)
{
    int numberOfImages = obsSingle.size();

    Eigen::Matrix<DataType, -1, -1> L = Eigen::Matrix<DataType, -1, -1>::Zero(3, numberOfImages);
    std::vector<DataType> vecL(3,(DataType)0);

    std::cout << "build L of " << 3 << "x" << numberOfImages << " matrix" << std::endl;
    for(int f = 0; f < numberOfImages; ++f)
    { // f means "f"rame
        vecL = str2vector<DataType>(obsSingle[f].lightDirection());
        for(int d = 0; d < 3; ++d)
        { // d means "d"imension
            L(d,f) = obsSingle[f].lightIntensity() * vecL[d];
        }
    }

    return L;
}

template <typename DataType>
inline void loadObservation(
    const std::vector<int>& indexOfPixels,
    const std::vector<CPS::ObservationSingle>& obsSingle,
    const int color,
    const int width,
    Eigen::Matrix<DataType, -1, -1>& I,
    Eigen::Matrix<DataType, -1, -1>& L
)
{
    I = buildObservationMatrix<DataType>(
        indexOfPixels,
        obsSingle,
        color,
        width
    );
    L = buildLightSourceMatrix<DataType>(
        obsSingle
    );
    std::cout << "I = " << std::endl;
    showMatrix(I);
    std::cout << "L = " << std::endl;
    showMatrix(L);
}

template <typename DataType>
inline Eigen::Matrix<DataType, -1, -1> estimateSurface(
    const Eigen::Matrix<DataType, -1, -1>& I,
    const Eigen::Matrix<DataType, -1, -1>& L
)
{
    Eigen::Matrix<DataType, -1, -1> Linv = pinv(L);
    Eigen::Matrix<DataType, -1, -1> Shat = I * Linv;

    DataType tol = std::numeric_limits<DataType>::epsilon() * (DataType)255;
    for( int i = 0; i < I.rows(); ++i )
    {
        if( I.row(i).norm() < tol )
        {
            // Pixel intensity is almost zero vector
            // means that the obtained normal vector is unreliable.
            Shat.row(i) = Eigen::Matrix<DataType, 3, 1>::Zero();
        }
    }

    return Shat;
}

template <typename DataType>
inline Eigen::Matrix<DataType, -1, -1> estimateSurfaceAlbedo(
    const Eigen::Matrix<DataType, -1, -1>& S
)
{
    Eigen::Matrix<DataType, -1, -1> ST = S.transpose();

    return ST.colwise().norm();
}

template <typename DataType>
inline Eigen::Matrix<DataType, -1, -1> estimateSurfaceNormal(
    const Eigen::Matrix<DataType, -1, -1>& S,
    const Eigen::Matrix<DataType, -1, -1>& R,
    const int numberOfPixels,
    const int color
)
{
    Eigen::Matrix<DataType, -1, -1> N = Eigen::Matrix<DataType, -1, -1>::Zero( numberOfPixels, 3 );
    Eigen::Matrix<DataType, 3, 3> Ssub = Eigen::Matrix<DataType, 3, 3>::Zero();
    Eigen::Matrix<DataType, 1, 3> RsubInv = Eigen::Matrix<DataType, 1, 3>::Zero();

    // the following loop can be done by 1 line of code using Eigen trick.
    for( size_t p = 0; p < numberOfPixels; ++p )
    {
        if( R( p ) > 0.0 )
        {
            for(int c = 0; c < color; ++c)
            {
                Ssub.row(c) = S.row(c*numberOfPixels+p);
                RsubInv(c) = 1.0/R(c*numberOfPixels+p);
            }
            N.row( p ) = RsubInv * Ssub / 3.0;
        }
    }

    return N;
}

template <typename DataType>
inline cimg_library::CImg<DataType> saveSurfaceNormalToImage(
    const Eigen::Matrix<DataType, -1, -1>& N,
    const std::vector<int>& indexOfPixels,
    const int width,
    const int height,
    const std::string strSave
)
{
    cimg_library::CImg<DataType> img(width, height, 1, 3, (DataType)0);
    int numberOfPixels = indexOfPixels.size();

    int x, y;
    for(int p = 0; p < numberOfPixels; ++p)
    {
        x = indexOfPixels[p] % width;
        y = indexOfPixels[p] / width;
        for(int c = 0; c < 3; ++c)
        {
            img(x, y, 0, c) = 255*(N(p,c)+1)/2;
        }
    }
    img.save( strSave.c_str() );

    return img;
}

template <typename DataType>
inline cimg_library::CImg<DataType> saveSurfaceAlbedoToImage(
    const Eigen::Matrix<DataType, -1, -1>& R,
    const std::vector<int>& indexOfPixels,
    const int width,
    const int height,
    const int color,
    const std::string strSave
)
{
    cimg_library::CImg<DataType> img(width, height, 1, color, (DataType)0);
    int numberOfPixels = indexOfPixels.size();

    int x, y;
    for(int p = 0; p < numberOfPixels; ++p)
    {
        x = indexOfPixels[p] % width;
        y = indexOfPixels[p] / width;
        for(int c = 0; c < color; ++c)
        {
            img(x, y, 0, c) = 255*(R(c*numberOfPixels+p)+1)/2;
        }
    }
    img.save( strSave.c_str() );

    return img;
}

template <typename DataType>
inline Eigen::Matrix<DataType, -1, -1> computeErrorLambertian(
    const Eigen::Matrix<DataType, -1, -1>& I,
    const Eigen::Matrix<DataType, -1, -1>& Shat,
    const Eigen::Matrix<DataType, -1, -1>& L
)
{
    return I - Shat*L;
}

template <typename DataType>
inline cimg_library::CImg<DataType> saveReprojectionError(
    const Eigen::Matrix<DataType, -1, -1>& Idiff,
    const std::vector<int>& indexOfPixels,
    const int width,
    const int height,
    const int color,
    const std::string strSave
)
{
    int numberOfPixels = indexOfPixels.size();
    cimg_library::CImg<DataType> img(width, height, 1, color, (DataType)0);

    int x, y;
    for(int p = 0; p < numberOfPixels; ++p)
    {
        x = indexOfPixels[p] % width;
        y = indexOfPixels[p] / width;
        for(int c = 0; c < color; ++c)
        {
            img(x, y, 0, c) = std::abs(Idiff(c*numberOfPixels+p,c));
        }
    }
    img.save( strSave.c_str() );

    return img;
}



#endif
