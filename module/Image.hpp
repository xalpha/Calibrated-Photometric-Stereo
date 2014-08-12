#ifndef __IMAGE_H__
#define __IMAGE_H__

/*!
 * \file Image.hpp
 *
 * \author Yuji Oyamada
 * \date 2014/03/31
 * \brief This file contains classes and functions handling image files based on CImg library, which is an open source single header file library.
 *
 */

// STL
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>

// CImg
#include <CImg.h>

// internal headers
#include "DataStructure.hpp"

/*!
 * \class ImageSingle
 *
 * \brief is to load / save / handle image data.
 *
 * \author Yuji Oyamada
 * \date 2014/04/21
 *
 */
template <typename ImageType = unsigned char, typename ImageOutputType = int>
class ImageSingle
{
public:
    //--------------------------------------------------------
    //
    //! \name Public Types
    //@{
    //--------------------------------------------------------
    typedef cimg_library::CImg<ImageType> Image;
    typedef typename ImagePixel<ImageOutputType>::PixelValue PixelValue;
    typedef typename ImagePixel<ImageOutputType>::PixelArray PixelArray;
    //@}

    //--------------------------------------------------------
    //
    //! \name Constructors / Destructor / Instance Management
    //@{
    //--------------------------------------------------------
    //! Destructor.
    ~ImageSingle(){}
    //! Default constructor.
    ImageSingle(
        const int _width = 0,
        const int _height = 0,
        const int _color = 1
    );
    //! Construct a single image from an image file.
    ImageSingle(
        const std::string _filename
    );
    //! Copy constructor.
    ImageSingle(
        const ImageSingle& _img
    );
    ImageSingle(const Image& _img):
        img(_img),
        width(_img.width()),
        height(_img.height()),
        color(_img.spectrum())
    {}
    //@}

    //------------------------------------------
    //
    //! \name Operators
    //@{
    //------------------------------------------
    //! Return an \c ImageSingle object.
    ImageSingle& operator()(const ImageSingle<ImageType,ImageOutputType>& _img);
    //! Return an \c ImageSingle object.
    ImageSingle& operator=(const ImageSingle<ImageType,ImageOutputType>& _img);
    //! Set \c filename of an image and load it.
    void operator()(const std::string _filename);
    //! Return a value of pixel (x,y,0,c).
    ImageOutputType operator()(
        const int x,
        const int y,
        const int c
    );
    //! Return a pixel value of pixel (x,y,0,c). This is either a single value if the image is gray image or RGB values if the image is color image.
    PixelValue operator()(
        const int x,
        const int y
    );

    //@}

    //------------------------------------------
    //
    //! \name Get / Set private member variables
    //@{
    //------------------------------------------
    //! Return the image.
    Image _img() const {return img;}
    //! Return image width.
    int _width() const {return width;}
    //! Return image height.
    int _height() const {return height;}
    //! Return the number of color depth.
    int _color() const {return color;}
    //! Return index of a pixel (x,y).
    int _index(const int x, const int y) const {return y*width+x;}
    //! Return file name of the image.
    std::string _filename() const {return filename;}
    //! Return all pixel values of the image as an array.
    PixelArray GetAsArray();
    //! Return all pixel values of the image as a \c std::string.
    std::string GetAsString(
        const char separator = ',',
        const unsigned int max_size = 0
    );
    //@}

    //------------------------------------------
    //
    //! \name Load / Save the image
    //@{
    //------------------------------------------
    //! Load an image using already given \c filename.
    void Load();
    //! Load an image given \c _filename.
    void Load(
        const std::string _filename,
        const bool flagEqualization = false
    );
    //! save an image as \c _filename.
    void Save(
        const std::string _filename
    );
    //! save assigned pixel value array as \c _filename.
    void Save(
        const PixelArray _data,
        const std::string _filename
    );
    //@}

private:
    //------------------------------------------
    //
    //! \name Private variables
    //@{
    //------------------------------------------
    //! The image.
    Image img;
    //! Image width.
    int width;
    //! Image height.
    int height;
    //! Image color depth.
    int color;
    //! File name of the image.
    std::string filename;
    //@}
};

template <typename ImageType, typename ImageOutputType>
inline ImageSingle<ImageType,ImageOutputType>::ImageSingle(
    const std::string _filename
)
{
    Load(_filename);
}

template <typename ImageType, typename ImageOutputType>
inline ImageSingle<ImageType,ImageOutputType>::ImageSingle(
    const int _width,
    const int _height,
    const int _color
):
    width(_width),
    height(_height),
    color(_color)
{
    img.assign(width, height, 1, color);
}

template <typename ImageType, typename ImageOutputType>
inline ImageSingle<ImageType,ImageOutputType>::ImageSingle(
    const ImageSingle& _img
) :
    width(_img.img.width()),
    height(_img.img.height()),
    color(_img.img.spectrum())
{
    img = _img._img();
}

template <typename ImageType, typename ImageOutputType>
inline void ImageSingle<ImageType,ImageOutputType>::operator()(
    const std::string _filename
)
{
    Load(_filename);
}

template <typename ImageType, typename ImageOutputType>
inline ImageSingle<ImageType,ImageOutputType>& ImageSingle<ImageType,ImageOutputType>::operator()(
    const ImageSingle& _img
)
{
    img = _img._img();
    width = _img._width();
    height = _img._height();
    color = _img._color();
    return *this;
}

template <typename ImageType, typename ImageOutputType>
inline ImageSingle<ImageType,ImageOutputType>& ImageSingle<ImageType,ImageOutputType>::operator=(
    const ImageSingle& _img
)
{
    img = _img._img();
    width = _img._width();
    height = _img._height();
    color = _img._color();
    return *this;
}

template <typename ImageType, typename ImageOutputType>
inline ImageOutputType ImageSingle<ImageType,ImageOutputType>::operator()(
    const int x,
    const int y,
    const int c
)
{
    int _color = std::min(c,color-1);
    return (ImageOutputType)img(x, y, 0, _color);
}

template <typename ImageType, typename ImageOutputType>
inline typename ImageSingle<ImageType,ImageOutputType>::PixelValue ImageSingle<ImageType,ImageOutputType>::operator()(
    const int x,
    const int y
)
{
    PixelValue data(color);
    for(int c = 0; c < color; ++c)
    {
        data[c] = (ImageOutputType)img(x, y, 0, c);
    }
    return data;
}

template <typename ImageType, typename ImageOutputType>
inline void ImageSingle<ImageType,ImageOutputType>::Load()
{
    Load(filename);
}

template <typename ImageType, typename ImageOutputType>
inline void ImageSingle<ImageType,ImageOutputType>::Load(
    const std::string _filename,
    const bool flagEqualization
)
{
    filename = _filename;
    if(flagEqualization)
    {
        Image tmp(filename.c_str());
        img = tmp.get_equalize(256);
    }
    else
    {
        img.assign(filename.c_str());
    }
    width = img.width();
    height = img.height();
    color = img.spectrum();
}

template <typename ImageType, typename ImageOutputType>
inline void ImageSingle<ImageType,ImageOutputType>::Save(
    const std::string _filename
)
{
    img.save(_filename.c_str());
}

template <typename ImageType, typename ImageOutputType>
inline void ImageSingle<ImageType,ImageOutputType>::Save(
    const PixelArray _data,
    const std::string _filename
)
{
    assert(
        _data.size() == width*height &&
        "image size is different from the existing image."
    );
    assert(
        _data[0].size() == color &&
        "Color depth is different from the existing image."
    );
    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            for(int c = 0; c < color; ++c)
            {
                img(x,y,0,c) = (ImageType)_data[_index(x,y)][c];
            }
        }
    }
    Save(_filename);
}

template <typename ImageType, typename ImageOutputType>
inline typename ImagePixel<ImageOutputType>::PixelArray
ImageSingle<ImageType,ImageOutputType>::GetAsArray()
{
    PixelArray data;
    PixelValue pixel;
    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            pixel.clear();
            for(int c = 0; c < color; ++c)
            {
                pixel.push_back((ImageOutputType)img(x,y,0,c));
            }
            data.push_back(pixel);
        }
    }

    return data;
}

template <typename ImageType, typename ImageOutputType>
inline std::string ImageSingle<ImageType,ImageOutputType>::GetAsString(
    const char separator,
    const unsigned int max_size
)
{
    return std::string(
        img.value_string(
           separator,
           max_size
       )
    );
}

///*!
// * \class ObjectData
// *
// * \brief is to handle an object data that contains two values, \c labelValue and \c values.
// *
// * \author Yuji Oyamada
// * \date 2014/04/21
// *
// */
//template <typename ImageType = unsigned char, typename ImageOutputType = int>
//class ObjectData
//{
//public:
//    //--------------------------------------------------------
//    //
//    //! \name Public Types
//    //@{
//    //--------------------------------------------------------
//    typedef typename ImagePixel<ImageOutputType>::PixelValue PixelValue;
//    typedef typename ImagePixel<ImageOutputType>::PixelArray PixelArray;
//    //@}

//    //--------------------------------------------------------
//    //
//    //! \name Constructors / Destructor / Instance Management
//    //@{
//    //--------------------------------------------------------
//    //! Destructor.
//    ~ObjectData(){}
//    //! Default constructor.
//    ObjectData():
//        labelValue(PixelValue()),
//        values(0)
//    {}
//    //! Constructor.
//    ObjectData(
//        PixelValue _labelValue,
//        PixelArray _values
//    ):
//        labelValue(_labelValue),
//        values(_values)
//    {}
//    //! Copy constructor.
//    ObjectData(
//        const ObjectData& _obj
//    ):
//        labelValue(_obj.labelValue),
//        values(_obj.values)
//    {}
//    //@}

//    //--------------------------------------------------------
//    //
//    //! \name Public member variables
//    //@{
//    //--------------------------------------------------------
//    //! Label value of the object.
//    PixelValue labelValue;
//    //! An array of pixel intensities of the object in an image.
//    PixelArray values;
//};

///*!
// * \class AllObjectData
// *
// * \brief is to handle a \c vector of \c ObjectData objects.
// *
// * \author Yuji Oyamada
// * \date 2014/03/30
// *
// */
//template <typename ImageType = unsigned char, typename ImageOutputType = int>
//class AllObjectData
//{
//public:
//    //--------------------------------------------------------
//    //
//    //! \name Constructors / Destructor / Instance Management
//    //@{
//    //--------------------------------------------------------
//    //! Destructor.
//    ~AllObjectData(){}
//    //! Default constructor.
//    AllObjectData():
//        objects( std::vector< ObjectData<ImageType,ImageOutputType> >() )
//    {}
//    //! Constructor.
//    AllObjectData(
//        std::vector< ObjectData<ImageType,ImageOutputType> > _objects
//    ):
//        objects(_objects)
//    {}
//    //! Copy constructor.
//    AllObjectData(
//        const AllObjectData& _objs
//    ):
//        objects(_objs.objects)
//    {}
//    //@}

//    //--------------------------------------------------------
//    //
//    //! \name operators
//    //@{
//    //--------------------------------------------------------
//    //! Return an \c AllObjectData object.
//    AllObjectData& operator()(const AllObjectData<ImageType,ImageOutputType>& _objs);
//    //! Return an \c AllObjectData object.
//    AllObjectData& operator=(const AllObjectData<ImageType,ImageOutputType>& _objs);

//    //@}

//    //--------------------------------------------------------
//    //
//    //! \name Public member variables
//    //@{
//    //--------------------------------------------------------
//    //!
//    std::vector< ObjectData<ImageType,ImageOutputType> > objects;
//};

//template <typename ImageType, typename ImageOutputType>
//inline AllObjectData<ImageType,ImageOutputType>& AllObjectData<ImageType,ImageOutputType>::operator()(
//    const AllObjectData& _objs
//)
//{
//    objects = _objs.objects;
//    return *this;
//}

//template <typename ImageType, typename ImageOutputType>
//inline AllObjectData<ImageType,ImageOutputType>& AllObjectData<ImageType,ImageOutputType>::operator=(
//    const AllObjectData& _objs
//)
//{
//    objects = _objs.objects;
//    return *this;
//}

///*!
// * \class ImagePair
// *
// * \brief is to handle a pair of image and its ground truth labeling.
// *
// * It is better to avoid defining a \c vector of \c ImagePair objects because each object contains two image data, which is memory consumption.
// * A better way is to define a \c vector of \c NamePair objects, each of which contains a pair of filenames and load each pair in a loop.
// * One of \c ImagePair class constructor constructs its object given NamrPair object for this issue.
// * The code is like:
// * \code
// * std::vector<NamePair> filename;
// * load(filename); // you should implement the load().
// * for(int n = 0; n < filename.size(); ++n)
// * {
// *      ImagePair<unsigned char> imgpair(filename[n]);
// *      func(imgpair); // do whatever you want.
// * }
// * \endcode
// *
// * \author Yuji Oyamada
// * \date 2014/04/21
// *
// */
//template <typename ImageType = unsigned char, typename ImageOutputType = int>
//class ImagePair
//{
//public:
//    //--------------------------------------------------------
//    //
//    //! \name Public Types
//    //@{
//    //--------------------------------------------------------
//    typedef cimg_library::CImg<ImageType> Image;
//    typedef typename ImagePixel<ImageOutputType>::PixelValue PixelValue;
//    typedef typename ImagePixel<ImageOutputType>::PixelArray PixelArray;
//    //@}

//    //--------------------------------------------------------
//    //
//    //! \name Constructors / Destructor / Instance Management
//    //@{
//    //--------------------------------------------------------
//    //! Destructor.
//    ~ImagePair(){}
//    //! Default constructor. Make both of the image and its labeling empty.
//    ImagePair(
//        const int _width = 0,
//        const int _height = 0,
//        const int _color = 1,
//        const int _numberOfLabels = 0,
//        const PixelArray _labelValues = PixelArray()
//    ):
//        width(_width),
//        height(_height),
//        color(_color),
//        numberOfLabels(_numberOfLabels),
//        labelValues(_labelValues),
//        img(ImageSingle<ImageType,ImageOutputType>()),
//        labeling(ImageSingle<ImageType,ImageOutputType>())
//    {}

//    //! Load two image files giving their file name,\c  _filenameImg and \c _filenameLabeling, and set the data to \c img and \c labeling, respectively.
//    ImagePair(
//        const std::string _filenameImg,
//        const std::string _filenameLabeling,
//        const bool flagEqualization = false
//    );
//    //! Load two image files giving a \c NamePair object, and set the data to \c img and \c labeling, respectively.
//    ImagePair(
//        const NamePair names
//    );
//    //@}

//    //------------------------------------------
//    //
//    //! \name Operators
//    //@{
//    //------------------------------------------
//    //! Load two image files giving their file name,\c  _filenameImg and \c _filenameLabeling, and set the data to \c img and \c labeling, respectively.
//    void operator()(
//        const std::string _filenameImg,
//        const std::string _filenameLabeling,
//        const bool flagEqualization = false
//    );
//    //@}

//    //------------------------------------------
//    //
//    //! \name Get / Set private member variables
//    //@{
//    //------------------------------------------
//    //! Load two image files giving their file name,\c  _filenameImg and \c _filenameLabeling, and set the data to \c img and \c labeling, respectively.
//    void Load(
//        const std::string _filenameImg,
//        const std::string _filenameLabeling,
//        const bool flagEqualization = false
//    );
//    //! Return the image \c img.
//    ImageSingle<ImageType,ImageOutputType> _img() const {return img;}
//    ImageSingle<ImageType,ImageOutputType> _labeling() const {return labeling;}
//    //! Return image width \c width.
//    int _width() const {return width;}
//    //! Return image height \c height.
//    int _height() const {return height;}
//    //! Return the number of color depth \c color.
//    int _color() const {return color;}
//    //! Return index of a pixel (x,y).
//    int _index(const int x, const int y) const {return y*width+x;}
//    //! returns \c numberOfLabels.
//    int _numberOfLabels(void) const {return numberOfLabels;}
//    //! returns \c labelValues.
//    PixelArray _labelValues(void) const {return labelValues;}
//    //! returns index-th pixel value of \c labelValues.
//    PixelValue _labelValues(const size_t index) const {return labelValues[ std::min(index,labelValues.size()-1) ];}

//    //! Return a pixel value of pixel (x,y,0,c).
//    ImageOutputType& _pixelValue(
//        const int x,
//        const int y,
//        const int c
//    );

//    //! Return a label value of pixel (x,y).
//    PixelValue& _pixelLabel(
//        const int x,
//        const int y
//    );

//    //! Return all pixel values of the image as an array.
//    PixelArray GetImageAsArray(){return img.GetAsArray();}

//    //! Return all label values of the labeling as an array.
//    PixelArray GetLabelAsArray(){return labeling.GetAsArray();}

//    //! Return all pixel values of \c index-th object as an array.
//    ObjectData<ImageType,ImageOutputType> GetObjectAsArray(
//        const PixelValue labelValue
//    );
//    //! Return all image gradient of \c index-th object as an array.
//    ObjectData<ImageType,ImageOutputType> GetGradientAsArray(
//        const PixelValue labelValue,
//        const int flagPrior
//    );
//    //! Return all pixel values of index-th object as an array.
//    AllObjectData<ImageType,ImageOutputType> GetAllObjectsAsArray();
//    //@}

//private:
//    //! The image.
//    ImageSingle<ImageType,ImageOutputType> img;
//    //! Ground truth labeling of the image.
//    ImageSingle<ImageType,ImageOutputType> labeling;
//    //! Image width.
//    int width;
//    //! Image height.
//    int height;
//    //! Image color depth.
//    int color;
//    //! The number of objects.
//    int numberOfLabels;
//    //! A set of label values.
//    PixelArray labelValues;
//};

//template <typename ImageType, typename ImageOutputType>
//inline ImagePair<ImageType,ImageOutputType>::ImagePair(
//    const std::string _filenameImg,
//    const std::string _filenameLabeling,
//    const bool flagEqualization
//)
//{
//    Load(
//        _filenameImg,
//        _filenameLabeling,
//        flagEqualization
//    );
//}

//template <typename ImageType, typename ImageOutputType>
//inline ImagePair<ImageType,ImageOutputType>::ImagePair(const NamePair names)
//{
//    Load(
//        names._img(),
//        names._labeling()
//    );
//}

//template <typename ImageType, typename ImageOutputType>
//inline void ImagePair<ImageType,ImageOutputType>::operator()(
//    const std::string _filenameImg,
//    const std::string _filenameLabeling,
//    const bool flagEqualization
//)
//{
//    Load(
//        _filenameImg,
//        _filenameLabeling,
//        flagEqualization
//    );
//}

//template <typename ImageType, typename ImageOutputType>
//inline void ImagePair<ImageType,ImageOutputType>::Load(
//    const std::string _filenameImg,
//    const std::string _filenameLabeling,
//    const bool flagEqualization
//)
//{
//// load images from file
//    img.Load(_filenameImg, flagEqualization);
//    labeling.Load(_filenameLabeling);
//// check image size
//    assert(
//        (img._width() == labeling._width()) &&
//        (img._height() == labeling._height()) &&
//        "size, width and height, of img and labeling must be same"
//    );
//// set image property
//    width = img._width();
//    height = img._height();
//    color = img._color();
//// check the number of labels in labeling
//    labelValues.clear();
//    PixelValue labelvalue;
//    bool flagExist;
//    for(int y = 0; y < height; ++y)
//    {
//        for(int x = 0; x < width; ++x)
//        {
//            flagExist = false;
//            labelvalue = labeling(x,y);
//            for(int l = 0; l < labelValues.size(); ++l)
//            { // search existing label values
//                if(labelValues[l] == labelvalue)
//                {
//                    flagExist = true;
//                }
//            }
//            if(!flagExist)
//            { // if we don't have current labelvalue in the stack.
//                labelValues.push_back( labelvalue );
//            }
//        }
//    }
//    numberOfLabels = labelValues.size();
//}

//template <typename ImageType, typename ImageOutputType>
//inline ImageOutputType& ImagePair<ImageType,ImageOutputType>::_pixelValue(
//    const int x,
//    const int y,
//    const int c
//)
//{
//    int _color = std::min(c,color-1);
//    return (ImageOutputType)img(x, y, 0, _color);
//}

//template <typename ImageType, typename ImageOutputType>
//inline typename ImagePair<ImageType,ImageOutputType>::PixelValue& ImagePair<ImageType,ImageOutputType>::_pixelLabel(
//    const int x,
//    const int y
//)
//{
//    PixelValue pixelvalue(labeling._img().spectrum());
//    for(int c = 0; c < pixelvalue.size(); ++c)
//    {
//        pixelvalue[c] = (ImageOutputType)labeling(x,y,0,c);
//    }
//    return pixelvalue;
//}

//template <typename ImageType, typename ImageOutputType>
//inline ObjectData<ImageType,ImageOutputType> ImagePair<ImageType,ImageOutputType>::GetObjectAsArray(
//    const PixelValue labelValue
//)
//{
//    assert(
//        labelValue.size() == labeling._img().spectrum() &&
//        "The dimension of the followings is different. (1) specified label value of target object and (2) ground truth labeling."
//    );
//    PixelArray data;
//    PixelValue pixel;
//    bool flagSize;
//    for(int y = 0; y < height; ++y)
//    {
//        for(int x = 0; x < width; ++x)
//        {
//            flagSize = true;
//            for(int c = 0; c < labelValue.size(); ++c)
//            {
//                if( (ImageOutputType)labeling(x,y,c) != labelValue[c])
//                {
//                    flagSize = false;
//                }
//            }
//            if( flagSize )
//            { // for each pixel
//                pixel.clear();
//                for(int c = 0; c < color; ++c)
//                { // add each color channel
//                    pixel.push_back( (ImageOutputType)img(x,y,c) );
//                }
//                data.push_back(pixel);
//            }
//        }
//    }

//    return ObjectData<ImageType,ImageOutputType>(labelValue, data);
//}

//template <typename ImageType, typename ImageOutputType>
//inline ObjectData<ImageType,ImageOutputType> ImagePair<ImageType,ImageOutputType>::GetGradientAsArray(
//    const PixelValue labelValue,
//    const int flagPrior
//)
//{
//    assert(
//        labelValue.size() == labeling._img().spectrum() &&
//        "The dimension of the followings is different. (1) specified label value of target object and (2) ground truth labeling."
//    );
//    PixelArray data;
//    PixelValue pixel;
//    bool flagSize;
//    int height_ = height-1;
//    int width_ = width-1;
//    for(int y = 0; y < height_; ++y)
//    {
//        for(int x = 0; x < width_; ++x)
//        {
//            flagSize = true;
//            // check a pixel of interest labeling(x,y)
//            for(int c = 0; c < labelValue.size(); ++c)
//            {
//                if( (ImageOutputType)labeling(x,y,c) != labelValue[c] )
//                {
//                    flagSize = false;
//                }
//            }
//            if( flagSize )
//            { // for each pixel
//                if(flagPrior==0)
//                { // check a pixel at (x+1,y)
//                    if((x+1) < width)
//                    {
//                        for(int c = 0; c < labelValue.size(); ++c)
//                        {
//                            if( (ImageOutputType)labeling(x+1,y,c) != labelValue[c] )
//                            {
//                                flagSize = false;
//                            }
//                        }
//                        if( flagSize )
//                        { // if label(x,y) == label(x+1,y)
//                            pixel.clear();
//                            for(int c = 0; c < color; ++c)
//                            { // add each color channel
//                                pixel.push_back( (ImageOutputType)img(x,y,c)-(ImageOutputType)img(x+1,y,c) );
//                            }
//                            data.push_back(pixel);
//                        }
//                    }
//                }
//                else if(flagPrior==1)
//                { // check a pixel at (x,y+1)
//                    if((y+1) < height)
//                    {
//                        for(int c = 0; c < labelValue.size(); ++c)
//                        {
//                            if( (ImageOutputType)labeling(x,y+1,c) != labelValue[c] )
//                            {
//                                flagSize = false;
//                            }
//                        }
//                        if( flagSize )
//                        { // if label(x,y) == label(x,y+1)
//                            pixel.clear();
//                            for(int c = 0; c < color; ++c)
//                            { // add each color channel
//                                pixel.push_back( (ImageOutputType)img(x,y,c)-(ImageOutputType)img(x,y+1,c) );
//                            }
//                            data.push_back(pixel);
//                        }
//                    }
//                }
//            }
//        }
//    }

//    return ObjectData<ImageType,ImageOutputType>(labelValue, data);
//}

//template <typename ImageType, typename ImageOutputType>
//inline AllObjectData<ImageType,ImageOutputType> ImagePair<ImageType,ImageOutputType>::GetAllObjectsAsArray()
//{
//    AllObjectData<ImageType,ImageOutputType> objs;
//    for(int l = 0; l < numberOfLabels; ++l)
//    {
//        objs.objects.push_back(GetObjectAsArray( labelValues[l] ));
//    }
//    return objs;
//}


#endif
