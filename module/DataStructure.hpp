#ifndef __DATASTRUCTURE_H__
#define __DATASTRUCTURE_H__

/*!
 * \file DataStructure.hpp
 *
 * \author Yuji Oyamada
 * \date 2014/04/02
 * \brief This file contains classes and functions handling some data structures, which contains basic type variables.
 *
 */

// STL
#include <vector>
#include <string>
#include <iostream>
#include <cassert>
#include <algorithm>

// Eigen
#include <Eigen/Core>

template <typename ImageOutputType = int>
struct ImagePixel
{
    typedef std::vector<ImageOutputType> PixelValue;
    typedef std::vector<PixelValue> PixelArray;
};

struct PixelLocation
{
    PixelLocation(const int x_ = 0, const int y_ = 0): x(x_), y(y_){}
    int x;
    int y;
};

namespace CPS
{

/*!
 * \class ObservationSingle
 *
 * \brief represents data of a single observation, i.e., filename of an image and light source information (direction and intensity).
 *
 * \author Yuji Oyamada
 * \date 2014/05/18
 *
 */

class ObservationSingle
{
    public:
    //--------------------------------------------------------
    //
    //! \name Constructors / Destructor / Instance Management
    //@{
    //--------------------------------------------------------
    //! Destructor.
    ~ObservationSingle(){}

    //! Default constructor.
    ObservationSingle(
        const std::string strImage = "",
        const std::string lightDirection = "",
        const float lightIntensity = 0.0f
    ):
        strImage_(strImage),
        lightDirection_(lightDirection),
        lightIntensity_(lightIntensity)
    {}

    //! Copy constructor
    ObservationSingle(
        const ObservationSingle& obj
    ) :
        strImage_(obj.strImage()),
        lightDirection_(obj.lightDirection()),
        lightIntensity_(obj.lightIntensity())
    {}
    //@}

    //------------------------------------------
    //
    //! \name Operators
    //@{
    //------------------------------------------
    ObservationSingle& operator()(const ObservationSingle& obj)
    {
        strImage_ = obj.strImage();
        lightDirection_ = obj.lightDirection();
        lightIntensity_ = obj.lightIntensity();

        return *this;
    }
    ObservationSingle& operator=(const ObservationSingle& obj)
    {
        strImage_ = obj.strImage();
        lightDirection_ = obj.lightDirection();
        lightIntensity_ = obj.lightIntensity();

        return *this;
    }
    //@}
    //------------------------------------------
    //
    //! \name Get / Set private member variables
    //@{
    //------------------------------------------
    //! returns \c strImage_, Filename of the image.
    std::string strImage(void) const {return strImage_;}
    //! returns \c lightDirection_, Light source direction.
    std::string lightDirection(void) const {return lightDirection_;}
    //! returns \c lightIntensity_, Light source intensity.
    float lightIntensity(void) const {return lightIntensity_;}
    //! sets \c strImage_, Filename of the image.
    void strImage(const std::string  strImage){strImage_ = strImage;}
    //! sets \c lightDirection_, Light source direction.
    void lightDirection(const std::string lightDirection){lightDirection_ = lightDirection;}
    //! sets \c lightIntensity_, Light source intensity.
    void lightIntensity(const float lightIntensity){lightIntensity_ = lightIntensity;}
    //@}

private:
    //! Filename of the image.
    std::string strImage_;
    //! Light source direction.
    std::string lightDirection_;
    //! Light source intensity.
    float lightIntensity_;
};

/*!
 * \class ObservationAll
 *
 * \brief represents data of a set of observation, i.e., .
 *
 * \author Yuji Oyamada
 * \date 2014/05/18
 *
 */
class ObservationAll
{
    public:
    //--------------------------------------------------------
    //
    //! \name Constructors / Destructor / Instance Management
    //@{
    //--------------------------------------------------------
    //! Destructor.
    ~ObservationAll(){}

    //! Default constructor.
    ObservationAll(
        const std::string strImageMask = "",
        const std::string strDirObservation = "",
        const std::vector<ObservationSingle> observation = std::vector<ObservationSingle>(),
        const int color = 1
    ):
        observation_( observation ),
        strImageMask_( strImageMask ),
        strDirObservation_( strDirObservation ),
        color_( color )
    {}

    //! Copy constructor
    ObservationAll(
        const ObservationAll& obj
    ) :
        observation_( obj.observation() ),
        strImageMask_( obj.strImageMask() ),
        strDirObservation_( obj.strDirObservation() ),
        color_( obj.color() )
    {}
    //@}

    //------------------------------------------
    //
    //! \name Operators
    //@{
    //------------------------------------------
    ObservationAll& operator()(const ObservationAll& obj)
    {
        observation_ = obj.observation();
        strImageMask_ = obj.strImageMask();
        strDirObservation_ = obj.strDirObservation();
        color_ = obj.color();

        return *this;
    }
    ObservationAll& operator=(const ObservationAll& obj)
    {
        observation_ = obj.observation();
        strImageMask_ = obj.strImageMask();
        strDirObservation_ = obj.strDirObservation();
        color_ = obj.color();

        return *this;
    }
    //@}
    //------------------------------------------
    //
    //! \name Get / Set private member variables
    //@{
    //------------------------------------------
    //! returns \c strImageMask_, Filename of image mask.
    std::string strImageMask(void) const {return strImageMask_;}
    //! returns \c strDirObservation_, Name of a directory, which contains all observation data.
    std::string strDirObservation(void) const {return strDirObservation_;}
    //! returns \c observation_, A set of single observation.
    std::vector<ObservationSingle> observation(void) const {return observation_;}
    //! returns \c index-th observation.
    ObservationSingle observationSingle(const int index) const {return observation_[std::min(index,(int)(observation_.size()-1))];}
    //! returns \c color_, the number of color channels of input images.
    int color(void) const {return color_;}
    //! sets \c strImage_, Filename of the image.
    void strImageMask(const std::string  strImageMask){strImageMask_ = strImageMask;}
    //! sets \c strDirObservation_, Name of a directory, which contains all observation data.
    void strDirObservation(const std::string strDirObservation){strDirObservation_ = strDirObservation;}
    //! sets \c observation_, A set of single observation.
    void observation(const std::vector<ObservationSingle> observation){observation_ = observation;}
    //! sets \c color_, the number of color channels of input images.
    void color(const int color){color_ = color;}
    //! adds a single observation.
    void addObservation(const ObservationSingle obs){observation_.push_back(obs);}
    //! returns the number of observation.
    int numberOfObservation(void) const {observation_.size();}
    //@}

private:
    //! A set of single observation.
    std::vector<ObservationSingle> observation_;
    //! Filename of image mask.
    std::string strImageMask_;
    //! Name of a directory, which contains all observation data.
    std::string strDirObservation_;
    //! The number of color channels of input images.
    int color_;
};

/*!
 * \class CpsConfig
 *
 * \brief combines all configuration of calibrated photometric stereo.
 *
 * \date 2014/05/16
 *
 */
class CpsConfig
{
public:
    //--------------------------------------------------------
    //
    //! \name Constructors / Destructor / Instance Management
    //@{
    //--------------------------------------------------------
    //! Destructor.
    ~CpsConfig(){}
    //! Default Destructor.
    CpsConfig(
        const ObservationAll obsAll = ObservationAll(),
        const std::string strDirOutput = "",
        const std::string strReflection = ""
    ):
        obsAll_(obsAll),
        strDirOutput_(strDirOutput),
        strReflection_(strReflection)
    {}
    //! Copy constructor.
    CpsConfig(
        const CpsConfig& config
    ):
        obsAll_(config.obsAll()),
        strDirOutput_(config.strDirOutput()),
        strReflection_(config.strReflection())
    {}
    //@}

    //------------------------------------------
    //
    //! \name Get / Set private member variables
    //@{
    //------------------------------------------
    //! returns \c strDirOutput_, Name of a directory, which contains all output data.
    std::string strDirOutput(void) const {return strDirOutput_;}
    //! sets \c strDirOutput_, Name of a directory, which contains all output data.
    void strDirOutput(const std::string  strDirOutput){strDirOutput_ = strDirOutput;}

    //! returns \c strReflection_, Name of reflectance model.
    std::string strReflection(void) const {return strReflection_;}
    //! sets \c strReflection_, Name of reflectance model.
    void strReflection(const std::string strReflection){strReflection_ = strReflection;}

    //! returns \c observation_, A set of single observation.
    ObservationAll obsAll(void) const {return obsAll_;}
    //! sets \c observation_, A set of single observation.
    void obsAll(const ObservationAll obsAll){obsAll_ = obsAll;}
    //! returns \c index-th observation.
    ObservationSingle observationSingle(const int index) const {return obsAll_.observation()[std::min(index,(int)(obsAll_.observation().size()-1))];}

    //! returns \c strDirObservation.
    std::string strDirObservation(void) const {return obsAll_.strDirObservation();}
    //! sets \c obsAll_.strDirObservation_, Name of a directory, which contains all observation data.
    void strDirObservation(const std::string strDirObservation){obsAll_.strDirObservation(strDirObservation);}

    //! returns \c obsAll_.strImageMask_, Filename of the image mask.
    std::string strImageMask(void) const {return obsAll_.strImageMask();}
    //! sets \c obsAll_.strImageMask_, Filename of the image mask.
    void strImageMask(const std::string  strImageMask){obsAll_.strImageMask(strImageMask);}

    //! sets \c observation_, A set of single observation.
    void observation(const std::vector<ObservationSingle> observation){obsAll_.observation(observation);}
    //! adds a single observation.
    void addObservation(const ObservationSingle obs){obsAll_.addObservation(obs);}

    //! returns the number of observation.
    int numberOfObservation(void) const {return obsAll_.numberOfObservation();}

    //! returns \c obsAll_.color_, the number of color channels of input images.
    int color(void) const {return obsAll_.color();}
    //! sets \c obsAll_.color_, the number of color channels of input images.
    void color(const int color){obsAll_.color(color);}
    //@}
private:
    //------------------------------------------
    //
    //! \name Private variables
    //@{
    //------------------------------------------
    //! All observation data.
    ObservationAll obsAll_;
    //! Name of a directory, which contains all output data.
    std::string strDirOutput_;
    //! Name of reflectance model.
    std::string strReflection_;
    //@}
};

/*!
 * \class CalibratedPhotometricStereo
 *
 * \brief combines all configuration of calibrated photometric stereo.
 *
 * \date 2014/05/18
 *
 */

template <typename DataType = float>
class CalibratedPhotometricStereo
{
public:
    //--------------------------------------------------------
    //
    //! \name Constructors / Destructor / Instance Management
    //@{
    //--------------------------------------------------------
    //! Destructor.
    ~CalibratedPhotometricStereo(){}
    //! Default Destructor.
    CalibratedPhotometricStereo(
        const CpsConfig& config = CpsConfig()
    ):
        config_(config)
    {}
    //! Copy constructor.
    CalibratedPhotometricStereo(
        const CalibratedPhotometricStereo& cps
    ):
        config_( cps.config() )
    {}
    //@}

    //------------------------------------------
    //
    //! \name Get / Set private member variables
    //@{
    //------------------------------------------
    //! returns \c config_, Configuration of calibrated photometric stereo.
    CpsConfig config(void) const {return config_;}
    //! sets \c config_, Configuration of calibrated photometric stereo.
    void config(const CpsConfig& config){config_ = config;}

    //! returns \c width_, Image width.
    int width(void) const {return width_;}
    //! sets \c width_, Image width.
    void width(const int width){width_ = width;}

    //! returns \c height_, Image width.
    int height(void) const {return height_;}
    //! sets \c height_, Image width.
    void height(const int height){height_ = height;}

    //! returns \c color_, The number of color channels.
    int color(void) const {return color_;}
    //! sets \c color_, The number of color channels.
    void color(const int color){color_ = color;}

    //! returns \c indexOfPixels_, The indices of available pixels.
    std::vector<int> indexOfPixels(void) const {return indexOfPixels_;}
    //! sets \c indexOfPixels_, The indices of available pixels.
    void indexOfPixels(const std::vector<int>& indexOfPixels){indexOfPixels_ = indexOfPixels; numberOfPixels_ = indexOfPixels_.size();}
    //! returns \c numberOfPixels_, The number of available pixels.
    int indexOfPixel(const int n) const {return indexOfPixels_[std::min(n,numberOfPixels_)];}
    //! returns \c numberOfPixels_, The number of available pixels.
    PixelLocation LocationOfPixel(const int n){return computePixelLocation(indexOfPixel(n));}
    //! returns \c numberOfPixels_, The number of available pixels.
    int numberOfPixels(void) const {return numberOfPixels_;}

    //! returns \c I_.
    Eigen::Matrix<DataType, -1, -1> I(void) const {return I_;}
    //! sets \c I_.
    void I(const Eigen::Matrix<DataType, -1, -1>& I){I_ = I;}
    //! returns \c S_.
    Eigen::Matrix<DataType, -1, -1> S(void) const {return S_;}
    //! sets \c S_.
    void S(const Eigen::Matrix<DataType, -1, -1>& S){S_ = S;}
    //! returns \c R_.
    Eigen::Matrix<DataType, -1, -1> R(void) const {return R_;}
    //! sets \c R_.
    void R(const Eigen::Matrix<DataType, -1, -1>& R){R_ = R;}
    //! returns \c N_.
    Eigen::Matrix<DataType, -1, -1> N(void) const {return N_;}
    //! sets \c N_.
    void N(const Eigen::Matrix<DataType, -1, -1>& N){N_ = N;}
    //! returns \c L_.
    Eigen::Matrix<DataType, -1, -1> L(void) const {return L_;}
    //! sets \c L_.
    void L(const Eigen::Matrix<DataType, -1, -1>& L){L_ = L;}
    //! returns \c Idiff_.
    Eigen::Matrix<DataType, -1, -1> Idiff(void) const {return Idiff_;}
    //! sets \c Idiff_.
    void Idiff(const Eigen::Matrix<DataType, -1, -1>& Idiff){Idiff_ = Idiff;}
    //@}
private:
    //------------------------------------------
    //
    //! \name Private variables
    //@{
    //------------------------------------------
    //! computes the index of a pixel (x,y).
    int computeIndex(const PixelLocation& pixel){return (pixel.y)*width_+(pixel.x);}
    //! computes the index of a pixel (x,y).
    PixelLocation computePixelLocation(const int& index){return PixelLocation(index%width_,index/width_);}
    //! Configuration of calibrated photometric stereo.
    CpsConfig config_;
    //! Image width.
    int width_;
    //! Image height.
    int height_;
    //! The number of color channels.
    int color_;
    //! The number of available pixels.
    int numberOfPixels_;
    //! The indices of available pixels.
    std::vector<int> indexOfPixels_;
    //! The number of available pixels.
    int numberOfImages_;
    //! The observation matrix \c I = pxf matrix, which satisfies \c I = SL.
    Eigen::Matrix<DataType, -1, -1> I_;
    //! The surface matrix \c S = px3 matrix, which satisfies \c I = SL.
    Eigen::Matrix<DataType, -1, -1> S_;
    //! The surface albedo matrix \c R = pxc vector, which satisfies \c S = RN.
    Eigen::Matrix<DataType, -1, -1> R_;
    //! The surface normal matrix \c N = px3 matrix, which satisfies \c S = RN.
    Eigen::Matrix<DataType, -1, -1> N_;
    //! The light source matrix \c L = 3xf matrix, which satisfies \c I = SL.
    Eigen::Matrix<DataType, -1, -1> L_;
    //! The reprojection error matrix \c Idiff = pxf matrix, which satisfies \c Idiff = I - SL.
    Eigen::Matrix<DataType, -1, -1> Idiff_;
    //@}
};


} // end of namespace CPS


///*!
// * \class InferenceAlgorithm
// *
// * \brief contains inference algorithm information
// *
// * \date 2014/05/16
// *
// */
//struct InferenceAlgorithm
//{
//    /*!
//     * \struct MinStCut
//     * \brief contains MinStCut information. see OpenGM API documentation for more detail.
//     */
//    struct MinStCut{
//        std::string implementation_;
//        bool on_;
//        //--------------------------------------------------------
//        //
//        //! \name Constructors / Destructor / Instance Management
//        //@{
//        //--------------------------------------------------------
//        //! Destructor.
//        ~MinStCut(){}
//        //! Default constructor.
//        MinStCut(const bool on = false) :
//            on_(on),
//            implementation_("Kolmogorov")
//        {}
//        MinStCut(const std::string implementation):
//            implementation_(implementation)
//        {
//            if(!on_){on_ = true;}
//        }
//        //@}

//        //------------------------------------------
//        //
//        //! \name Operators
//        //@{
//        //------------------------------------------
//        void operator()(
//            const bool on,
//            const std::string implementation
//        )
//        {
//            on_ = on;
//            implementation_ = implementation;
//        }
//        void operator=(
//            const MinStCut _minstcut
//        )
//        {
//            on_ = _minstcut.on();
//            implementation_ = _minstcut.implementation_;
//        }
//        //@}
//        void on(bool on){on_ = on;}
//        bool on(void) const {return on_;}
//    };

//    /*!
//     * \struct QPBO
//     * \brief contains QPBO information. see OpenGM API documentation for more detail.
//     */
//    struct QPBO{
//        bool strongPersistency_;
//        bool useImproveing_;
//        bool useProbeing_;
//        bool on_;
//        //--------------------------------------------------------
//        //
//        //! \name Constructors / Destructor / Instance Management
//        //@{
//        //--------------------------------------------------------
//        //! Destructor.
//        ~QPBO(){}
//        //! Default constructor.
//        QPBO(const bool on = false) :
//            on_(on),
//            strongPersistency_(true),
//            useImproveing_(false),
//            useProbeing_(false)
//        {}
//        //! constructs with input arguments.
//        QPBO(
//            const bool strongPersistency,
//            const bool useImproveing,
//            const bool useProbeing
//        )
//        {
//            strongPersistency_ = strongPersistency;
//            useImproveing_ = useImproveing;
//            useProbeing_ = useProbeing;
//            if(!on_){on_ = true;}
//        }
//        //@}

//        //------------------------------------------
//        //
//        //! \name Operators
//        //@{
//        //------------------------------------------
//        void operator()(
//            const bool on = false,
//            const bool strongPersistency = true,
//            const bool useImproveing = false,
//            const bool useProbeing = false
//        )
//        {
//            on_ = on;
//            strongPersistency_ = strongPersistency;
//            useImproveing_ = useImproveing;
//            useProbeing_ = useProbeing;
//        }
//        void operator=(
//            const QPBO _qpbo
//        )
//        {
//            useProbeing_ = _qpbo.useProbeing_;
//            strongPersistency_ = _qpbo.strongPersistency_;
//            useImproveing_ = _qpbo.useImproveing_;
//            on_ = _qpbo.on_;
//        }
//        //@}
//        void on(bool on){on_ = on;}
//        bool on(void) const {return on_;}
//    };

//    /*!
//     * \struct AlphaBetaSwap
//     * \brief contains AlphaBetaSwap information. see OpenGM API documentation for more detail.
//     */
//    struct AlphaBetaSwap{
//        int maxNumberOfIterations_;
//        bool on_;
//        //--------------------------------------------------------
//        //
//        //! \name Constructors / Destructor / Instance Management
//        //@{
//        //--------------------------------------------------------
//        //! Destructor.
//        ~AlphaBetaSwap(){}
//        //! Default constructor.
//        AlphaBetaSwap(
//            const bool on = false
//        ) :
//            on_(on),
//            maxNumberOfIterations_(1000)
//        {}
//        //! constructs with input arguments.
//        AlphaBetaSwap(
//            const int maxNumberOfIterations
//        )
//        {
//            maxNumberOfIterations_ = maxNumberOfIterations;
//            if(!on_){on_ = true;}
//        }
//        //@}

//        //------------------------------------------
//        //
//        //! \name Operators
//        //@{
//        //------------------------------------------
//        void operator()(
//            const bool on = false,
//            const int maxNumberOfIterations = 1000
//        )
//        {
//            on_ = on;
//            maxNumberOfIterations_ = maxNumberOfIterations;
//        }
//        void operator=(
//            const AlphaBetaSwap _swap
//        )
//        {
//            on_ = _swap.on_;
//            maxNumberOfIterations_ = _swap.maxNumberOfIterations_;
//        }
//        //@}
//        void on(bool on){on_ = on;}
//        bool on(void) const {return on_;}
//    };

//    /*!
//     * \struct AlphaExpansion
//     * \brief contains AlphaExpansion information. see OpenGM API documentation for more detail.
//     */
//    struct AlphaExpansion{
//        int maxNumberOfSteps_;
//        enum LabelingIntitialType {DEFAULT_LABEL, RANDOM_LABEL, LOCALOPT_LABEL, EXPLICIT_LABEL};
//        enum OrderType {DEFAULT_ORDER, RANDOM_ORDER, EXPLICIT_ORDER};
//        LabelingIntitialType labelInitialType_;
//        OrderType orderType_;
//        bool on_;
//        //--------------------------------------------------------
//        //
//        //! \name Constructors / Destructor / Instance Management
//        //@{
//        //--------------------------------------------------------
//        //! Destructor.
//        ~AlphaExpansion(){}
//        //! Default constructor.
//        AlphaExpansion(const bool on = false) :
//            on_(on),
//            maxNumberOfSteps_(1000),
//            labelInitialType_(DEFAULT_LABEL),
//            orderType_(DEFAULT_ORDER)
//        {}
//        //! constructs with input arguments.
//        AlphaExpansion(
//            const int maxNumberOfSteps,
//            const LabelingIntitialType labelInitialType,
//            const OrderType orderType
//        )
//        {
//            maxNumberOfSteps_ = maxNumberOfSteps;
//            labelInitialType_ = labelInitialType;
//            orderType_ = orderType;
//            if(!on_){on_ = true;}
//        }
//        //@}

//        //------------------------------------------
//        //
//        //! \name Operators
//        //@{
//        //------------------------------------------
//        void operator()(
//            const bool on = false,
//            const int maxNumberOfSteps = 1000,
//            const LabelingIntitialType labelInitialType = DEFAULT_LABEL,
//            const OrderType orderType = DEFAULT_ORDER
//        )
//        {
//            on_ = on;
//            maxNumberOfSteps_ = maxNumberOfSteps;
//            labelInitialType_ = labelInitialType;
//            orderType_ = orderType;
//        }
//        void operator=(
//            const AlphaExpansion _expansion
//        )
//        {
//            on_ = _expansion.on_;
//            maxNumberOfSteps_ = _expansion.maxNumberOfSteps_;
//            labelInitialType_ = _expansion.labelInitialType_;
//            orderType_ = _expansion.orderType_;
//        }
//        //@}
//        void on(bool on){on_ = on;}
//        bool on(void) const {return on_;}
//    };

//    /*!
//     * \struct FusionMoveFix
//     * \brief contains FusionMoveFix information. see OpenGM API documentation for more detail.
//     */
//    struct FusionMoveFix{
//        size_t maxNumberOfSteps_;
//        enum LabelingIntitialType {DEFAULT_LABEL, RANDOM_LABEL, LOCALOPT_LABEL, EXPLICIT_LABEL};
//        enum OrderType {DEFAULT_ORDER, RANDOM_ORDER, EXPLICIT_ORDER};
//        LabelingIntitialType labelInitialType_;
//        OrderType orderType_;
//        bool on_;
//        //--------------------------------------------------------
//        //
//        //! \name Constructors / Destructor / Instance Management
//        //@{
//        //--------------------------------------------------------
//        //! Destructor.
//        ~FusionMoveFix(){}
//        //! Default constructor.
//        FusionMoveFix(const bool on = false) :
//            on_(on),
//            maxNumberOfSteps_(1000),
//            labelInitialType_(DEFAULT_LABEL),
//            orderType_(DEFAULT_ORDER)
//        {}
//        //! constructs with input arguments.
//        FusionMoveFix(
//            const int maxNumberOfSteps,
//            const LabelingIntitialType labelInitialType,
//            const OrderType orderType
//        )
//        {
//            maxNumberOfSteps_ = maxNumberOfSteps;
//            labelInitialType_ = labelInitialType;
//            orderType_ = orderType;
//            if(!on_){on_ = true;}
//        }
//        //@}

//        //------------------------------------------
//        //
//        //! \name Operators
//        //@{
//        //------------------------------------------
//        void operator()(
//            const bool on = false,
//            const int maxNumberOfSteps = 1000,
//            const LabelingIntitialType labelInitialType = DEFAULT_LABEL,
//            const OrderType orderType = DEFAULT_ORDER
//        )
//        {
//            on_ = on;
//            maxNumberOfSteps_ = maxNumberOfSteps;
//            labelInitialType_ = labelInitialType;
//            orderType_ = orderType;
//        }
//        void operator=(
//            const FusionMoveFix _fusion
//        )
//        {
//            on_ = _fusion.on_;
//            maxNumberOfSteps_ = _fusion.maxNumberOfSteps_;
//            labelInitialType_ = _fusion.labelInitialType_;
//            orderType_ = _fusion.orderType_;
//        }
//        //@}
//        void on(bool on){on_ = on;}
//        bool on(void) const {return on_;}
//    };

//    //--------------------------------------------------------
//    //
//    //! \name Constructors / Destructor / Instance Management
//    //@{
//    //--------------------------------------------------------
//    //! Destructor.
//    ~InferenceAlgorithm(){}
//    //! Default constructor.
//    InferenceAlgorithm():
//        mincut(MinStCut()),
//        qpbo(QPBO()),
//        moveSwap(AlphaBetaSwap()),
//        moveExpansion(AlphaExpansion()),
//        moveFusion(FusionMoveFix())
//    {}
//    //! Construct a single image from an image file.
//    InferenceAlgorithm(
//        const InferenceAlgorithm& _infAlg
//    ):
//        mincut(_infAlg.mincut),
//        qpbo(_infAlg.qpbo),
//        moveSwap(_infAlg.moveSwap),
//        moveExpansion(_infAlg.moveExpansion),
//        moveFusion(_infAlg.moveFusion)
//    {}
//    //@}

//    //------------------------------------------
//    //
//    //! \name Operators
//    //@{
//    //------------------------------------------
//    InferenceAlgorithm& operator()(
//        const InferenceAlgorithm& infAlg
//    );
//    InferenceAlgorithm& operator=(
//        const InferenceAlgorithm& infAlg
//    );
//    //@}

//    //------------------------------------------
//    //
//    //! \name Public variables
//    //@{
//    //------------------------------------------
//    //! MinStCut information
//    MinStCut mincut;
//    //! QPBO information
//    QPBO qpbo;
//    //! AlphaBetaSwap information
//    AlphaBetaSwap moveSwap;
//    //! AlphaExpansion information
//    AlphaExpansion moveExpansion;
//    //! FusionMoveFix information
//    FusionMoveFix moveFusion;
//    //@}
//};

//InferenceAlgorithm& InferenceAlgorithm::operator()(
//    const InferenceAlgorithm& infAlg
//)
//{
//    mincut = infAlg.mincut;
//    qpbo = infAlg.qpbo;
//    moveSwap = infAlg.moveSwap;
//    moveExpansion = infAlg.moveExpansion;
//    moveFusion = infAlg.moveFusion;

//    return *this;
//}

//InferenceAlgorithm& InferenceAlgorithm::operator=(
//    const InferenceAlgorithm& infAlg
//)
//{
//    mincut = infAlg.mincut;
//    qpbo = infAlg.qpbo;
//    moveSwap = infAlg.moveSwap;
//    moveExpansion = infAlg.moveExpansion;
//    moveFusion = infAlg.moveFusion;

//    return *this;
//}

///*!
// * \class PotentialGmm
// *
// * \brief contains unary GMM potential function settings.
// *
// * \date 2014/04/07
// *
// */
//template <typename ImageOutputType = int>
//class PotentialGmm
//{
//public:
//    typedef typename SegmentationObject::SingleObject<ImageOutputType> Object;
//    /*!
//     * \struct UnaryGmm
//     * \brief contains FusionMoveFix information. see OpenGM API documentation for more detail.
//     */
//    struct UnaryGmm
//    {
//        //--------------------------------------------------------
//        //
//        //! \name Constructors / Destructor / Instance Management
//        //@{
//        //--------------------------------------------------------
//        //! Destructor.
//        ~UnaryGmm(){}
//        //! Default constructor.
//        UnaryGmm(
//            const Object _obj = Object(),
//            const Prior::GMM _gmm = Prior::GMM()
//        ) :
//            obj(_obj),
//            gmm(_gmm)
//        {}
//        //! Copy constructor.
//        UnaryGmm(
//            const UnaryGmm& _pairGmm
//        ) :
//            obj(_pairGmm.obj),
//            gmm(_pairGmm.gmm)
//        {}
//        //@}

//        //------------------------------------------
//        //
//        //! \name Operators
//        //@{
//        //------------------------------------------
//        UnaryGmm& operator()(
//            const Object _obj,
//            const Prior::GMM _gmm
//        );
//        UnaryGmm& operator()(
//            const UnaryGmm& _pairGmm = UnaryGmm()
//        );
//        UnaryGmm& operator=(
//            const UnaryGmm& _pairGmm
//        );
//        //@}

//        //------------------------------------------
//        //
//        //! \name Public variables
//        //@{
//        //------------------------------------------
//        Object obj;
//        Prior::GMM gmm;
//    };
//    //--------------------------------------------------------
//    //
//    //! \name Constructors / Destructor / Instance Management
//    //@{
//    //--------------------------------------------------------
//    //! Destructor.
//    ~PotentialGmm(){}
//    //! Default constructor.
//    PotentialGmm(
//        const std::vector<UnaryGmm> _pairGmm = std::vector<UnaryGmm>(),
//        const std::vector<double> _weight = std::vector<double>()
//    ) :
//        pairGmm(_pairGmm),
//        weight(_weight)
//    {}
//    //! Copy constructor.
//    PotentialGmm(
//        const PotentialGmm& _potentialGmm
//    ) :
//        pairGmm(_potentialGmm._pairGmm()),
//        weight(_potentialGmm._weight())
//    {}
//    //@}

//    //------------------------------------------
//    //
//    //! \name Operators
//    //@{
//    //------------------------------------------
//    PotentialGmm& operator()(
//        const std::vector<UnaryGmm> pairGmm,
//        const std::vector<double> weight
//    );
//    PotentialGmm& operator()(
//        const PotentialGmm& _potentialGmm = PotentialGmm()
//    );
//    PotentialGmm& operator=(
//        const PotentialGmm& _potentialGmm
//    );
//    //@}

//    //------------------------------------------
//    //
//    //! \name Get / Set private member variables
//    //@{
//    //------------------------------------------
//    //! returns \c pairGmm.
//    std::vector<UnaryGmm> _pairGmm(void) const {return pairGmm;}
//    //! sets \c pairGmm.
//    void _pairGmm(const std::vector<UnaryGmm> _pairGmm){pairGmm = _pairGmm;}
//    //! returns \c weight.
//    std::vector<double> _weight(void) const {return weight;}
//    //! sets \c weight.
//    void _weight(const std::vector<double> _weight){weight = _weight;}
//    //! adds a pair of a segmentation object and a GMM \c _pairGmm to the unary term \c unaryGmm.
//    void addPotentialGmm(
//        const UnaryGmm _potentialGmm
//    );
//    // adds a pair of a segmentation object \c _obj and a GMM \c _gmm to the unary term \c unaryGmm.
//    void addPotentialGmm(
//        const PotentialGmm<ImageOutputType>::Object _obj,
//        const Prior::GMM _gmm
//    );
//    //! returns the number of target objects.
//    bool size(void){return (pairGmm.size()>0);}

//    //@}
//    //! computes energy given an obesrvation data.
//    double computeEnergy(
//        const arma::vec& observation,
//        const int _index
//    );

//private:
//    //------------------------------------------
//    //
//    //! \name Private variables
//    //@{
//    //------------------------------------------
//    //! a set of GMM and object information, each of which is for a single segmentation object.
//    std::vector<UnaryGmm> pairGmm;
//    //! a set of weight parameters.
//    std::vector<double> weight;
//    //@}
//};

//template <typename ImageOutputType>
//inline typename PotentialGmm<ImageOutputType>::UnaryGmm& PotentialGmm<ImageOutputType>::UnaryGmm::operator()(
//    const Object _obj,
//    const Prior::GMM _gmm
//)
//{
//    obj = _obj;
//    gmm = _gmm;

//    return *this;
//}

//template <typename ImageOutputType>
//inline typename PotentialGmm<ImageOutputType>::UnaryGmm& PotentialGmm<ImageOutputType>::UnaryGmm::operator()(
//    const UnaryGmm& _pairGmm
//)
//{
//    obj = _pairGmm.obj;
//    gmm = _pairGmm.gmm;

//    return *this;
//}

//template <typename ImageOutputType>
//inline typename PotentialGmm<ImageOutputType>::UnaryGmm& PotentialGmm<ImageOutputType>::UnaryGmm::operator=(
//    const UnaryGmm& _pairGmm
//)
//{
//    obj = _pairGmm.obj;
//    gmm = _pairGmm.gmm;

//    return *this;
//}

//template <typename ImageOutputType>
//inline PotentialGmm<ImageOutputType>& PotentialGmm<ImageOutputType>::operator()(
//    const std::vector<UnaryGmm> _pairGmm,
//    const std::vector<double> _weight
//)
//{
//    pairGmm = _pairGmm;
//    weight = _weight;

//    return *this;
//}

//template <typename ImageOutputType>
//inline PotentialGmm<ImageOutputType>& PotentialGmm<ImageOutputType>::operator()(
//    const PotentialGmm<ImageOutputType>& _potentialGmm
//)
//{
//    pairGmm = _potentialGmm._pairGmm();
//    weight = _potentialGmm._weight();

//    return *this;
//}

//template <typename ImageOutputType>
//inline PotentialGmm<ImageOutputType>& PotentialGmm<ImageOutputType>::operator=(
//    const PotentialGmm<ImageOutputType>& _potentialGmm
//)
//{
//    pairGmm = _potentialGmm._pairGmm();
//    weight = _potentialGmm._weight();

//    return *this;
//}

//template <typename ImageOutputType>
//inline void PotentialGmm<ImageOutputType>::addPotentialGmm(
//    const UnaryGmm _potentialGmm
//)
//{
//    pairGmm.push_back(_potentialGmm);
//}


//template <typename ImageOutputType>
//inline void PotentialGmm<ImageOutputType>::addPotentialGmm(
//    const PotentialGmm<ImageOutputType>::Object _obj,
//    const Prior::GMM _gmm
//)
//{
//    pairGmm.push_back(
//        PotentialGmm<ImageOutputType>::UnaryGmm(
//            _obj,
//            _gmm
//        )
//    );
//}

//template <typename ImageOutputType>
//inline double PotentialGmm<ImageOutputType>:: computeEnergy(
//    const arma::vec& observation,
//    const int _index
//)
//{
//    int index = std::min(_index, (int)(pairGmm.size()-1));
//    return std::max( pairGmm[index].gmm.computeEnergy(observation), std::numeric_limits<double>::epsilon() );
//}

///*!
// * \class PotentialPairwisePotts
// *
// * \brief contains pairwise Potts function settings.
// *
// * \date 2014/04/08
// *
// */
//class PotentialPairwisePotts
//{
//public:
//    //--------------------------------------------------------
//    //
//    //! \name Constructors / Destructor / Instance Management
//    //@{
//    //--------------------------------------------------------
//    //! Destructor.
//    ~PotentialPairwisePotts(){}
//    //! Default constructor.
//    PotentialPairwisePotts(
//        const double _valueEqual = 0.0,
//        const double _valueNotEqual = 1.0,
//        const std::vector<double> _weight = std::vector<double>()
//    ) :
//        valueEqual(_valueEqual),
//        valueNotEqual(_valueNotEqual),
//        weight(_weight)
//    {}
//    //! Copy constructor.
//    PotentialPairwisePotts(
//        const PotentialPairwisePotts& _pairwisePotts
//    ) :
//        valueEqual(_pairwisePotts._valueEqual()),
//        valueNotEqual(_pairwisePotts._valueNotEqual()),
//        weight(_pairwisePotts._weight())
//    {}
//    //@}

//    //------------------------------------------
//    //
//    //! \name Operators
//    //@{
//    //------------------------------------------
//    PotentialPairwisePotts& operator()(
//        const double _valueEqual = 0.0,
//        const double _valueNotEqual = 1.0,
//        const std::vector<double> _weight = std::vector<double>()
//    );
//    PotentialPairwisePotts& operator()(
//        const PotentialPairwisePotts& _pairwisePotts
//    );
//    PotentialPairwisePotts& operator=(
//        const PotentialPairwisePotts& _pairwisePotts
//    );
//    //@}

//    //------------------------------------------
//    //
//    //! \name Get / Set private member variables
//    //@{
//    //------------------------------------------
//    //! returns \c valueEqual.
//    double _valueEqual(void) const {return valueEqual;}
//    //! returns \c valueNotEqual.
//    double _valueNotEqual(void) const {return valueNotEqual;}
//    //! returns \c weight.
//    std::vector<double> _weight(void) const {return weight;}
//    //! sets \c valueEqual.
//    void _valueEqual(const double _valueEqual){valueEqual = _valueEqual;}
//    //! returns \c valueNotEqual.
//    void _valueNotEqual(const double _valueNotEqual){valueNotEqual = _valueNotEqual;}
//    //! sets \c weight.
//    void _weight(const std::vector<double> _weight){weight = _weight;}
//    //@}
//    bool size(void){return (weight.size() > 0);}

//private:
//    //------------------------------------------
//    //
//    //! \name Private variables
//    //@{
//    //------------------------------------------
//    //! a set of weight parameters
//    std::vector<double> weight;
//    //! energy for the case the labels of two nodes pair are same.
//    double valueEqual;
//    //! energy for the case the labels of two nodes pair are different.
//    double valueNotEqual;
//    //@}
//};

//PotentialPairwisePotts& PotentialPairwisePotts::operator()(
//    const double _valueEqual,
//    const double _valueNotEqual,
//    const std::vector<double> _weight
//)
//{
//    valueEqual = _valueEqual;
//    valueNotEqual = _valueNotEqual;
//    weight = _weight;

//    return *this;
//}

//PotentialPairwisePotts& PotentialPairwisePotts::operator()(
//    const PotentialPairwisePotts& _pairwisePotts
//)
//{
//    valueEqual = _pairwisePotts._valueEqual();
//    valueNotEqual = _pairwisePotts._valueNotEqual();
//    weight = _pairwisePotts._weight();

//    return *this;
//}

//PotentialPairwisePotts& PotentialPairwisePotts::operator=(
//    const PotentialPairwisePotts& _pairwisePotts
//)
//{
//    valueEqual = _pairwisePotts._valueEqual();
//    valueNotEqual = _pairwisePotts._valueNotEqual();
//    weight = _pairwisePotts._weight();

//    return *this;
//}

///*!
// * \class EnergyFunction
// *
// * \brief contains energy function settings.
// *
// * \date 2014/04/06
// *
// */
//template <typename ImageOutputType>
//class PotentialFunction
//{
//public:

//    //--------------------------------------------------------
//    //
//    //! \name Constructors / Destructor / Instance Management
//    //@{
//    //--------------------------------------------------------
//    //! Destructor.
//    ~PotentialFunction(){}
//    //! Default constructor.
//    PotentialFunction(
//        const PotentialGmm<ImageOutputType>& _unaryGmm = PotentialGmm<ImageOutputType>(),
//        const PotentialGmm<ImageOutputType>& _unaryGradientXGmm = PotentialGmm<ImageOutputType>(),
//        const PotentialGmm<ImageOutputType>& _unaryGradientYGmm = PotentialGmm<ImageOutputType>(),
//        const PotentialPairwisePotts& _pairwisePotts = PotentialPairwisePotts()
//    ) :
//        unaryGmm(_unaryGmm),
//        unaryGradientXGmm(_unaryGradientXGmm),
//        unaryGradientYGmm(_unaryGradientYGmm),
//        pairwisePotts(_pairwisePotts)
//    {}
//    //! Copy constructor.
//    PotentialFunction(
//        const PotentialFunction& _potential
//    ) :
//        unaryGmm(_potential._unaryGmm()),
//        unaryGradientXGmm(_potential._unaryGradientXGmm()),
//        unaryGradientYGmm(_potential._unaryGradientYGmm()),
//        pairwisePotts(_potential._pairwisePotts())
//    {}
//    //@}

//    //------------------------------------------
//    //
//    //! \name Operators
//    //@{
//    //------------------------------------------
//    PotentialFunction& operator()(
//        const PotentialFunction& _potential
//    );
//    PotentialFunction& operator=(
//        const PotentialFunction& _potential
//    );
//    //@}

//    //------------------------------------------
//    //
//    //! \name Get / Set private member variables
//    //@{
//    //------------------------------------------
//    //! returns \c gmm, which is GMM for unary GMM term.
//    PotentialGmm<ImageOutputType> _unaryGmm(void) const {return unaryGmm;}
//    //! returns \c gmm, which is GMM for pairwise gradient GMM term.
//    PotentialGmm<ImageOutputType> _unaryGradientXGmm(void) const {return unaryGradientXGmm;}
//    //! returns \c gmm, which is GMM for pairwise gradient GMM term.
//    PotentialGmm<ImageOutputType> _unaryGradientYGmm(void) const {return unaryGradientYGmm;}
//    //! returns \c gmm, which is GMM for unary GMM term.
//    PotentialPairwisePotts _pairwisePotts(void) const {return pairwisePotts;}
//    //! sets \c unaryGmm, which is GMM for unary GMM term.
//    void  _unaryGmm(const PotentialGmm<ImageOutputType>& _unaryGmm){unaryGmm = _unaryGmm;}
//    //! sets \c unaryGradientXGmm, which is GMM for pairwise gradient GMM term.
//    void  _unaryGradientXGmm(const PotentialGmm<ImageOutputType>& _unaryGradientXGmm){unaryGradientXGmm = _unaryGradientXGmm;}
//    //! sets \c unaryGradientYGmm, which is GMM for pairwise gradient GMM term.
//    void  _unaryGradientYGmm(const PotentialGmm<ImageOutputType>& _unaryGradientYGmm){unaryGradientYGmm = _unaryGradientYGmm;}
//    //! sets \c unaryGmm, which is GMM for unary GMM term.
//    void  _pairwisePotts(const PotentialPairwisePotts& _pairwisePotts){pairwisePotts = _pairwisePotts;}

//    //! adds a pair of a segmentation object \c _obj and a GMM \c _gmm to the unary term \c unaryGmm.
//    void addPotentialUnaryGmm(
//        const typename PotentialGmm<ImageOutputType>::Object _obj,
//        const Prior::GMM _gmm
//    );
//    //! adds a pair of a segmentation object \c _obj and a GMM \c _gmm to the unary term \c unaryGmm.
//    void addPotentialUnaryGradientGmm(
//        const typename PotentialGmm<ImageOutputType>::Object _obj,
//        const Prior::GMM _gmm,
//        const int flagAxis
//    );

//    bool onUnaryGmm(void){return unaryGmm.size();}
//    bool onUnaryGradientXGmm(void){return unaryGradientXGmm.size();}
//    bool onUnaryGradientYGmm(void){return unaryGradientYGmm.size();}
//    bool onPairwisePotts(void){return pairwisePotts.size();}
//    //@}

//private:
//    //------------------------------------------
//    //
//    //! \name Private variables
//    //@{
//    //------------------------------------------
//    //! potential settings for unary GMM term.
//    PotentialGmm<ImageOutputType> unaryGmm;
//    //! potential settings for pairwise gradient GMM term.
//    PotentialGmm<ImageOutputType> unaryGradientXGmm;
//    //! potential settings for pairwise gradient GMM term.
//    PotentialGmm<ImageOutputType> unaryGradientYGmm;
//    //! potential settings for pairwise Potts term.
//    PotentialPairwisePotts pairwisePotts;
//    //@}

//};

//template <typename ImageOutputType>
//inline void PotentialFunction<ImageOutputType>::addPotentialUnaryGmm(
//    const typename PotentialGmm<ImageOutputType>::Object _obj,
//    const Prior::GMM _gmm
//)
//{
//    unaryGmm.push_back(
//        PotentialFunction<ImageOutputType>::UnaryGmm(
//            _obj,
//            _gmm
//        )
//    );
//}

//template <typename ImageOutputType>
//inline void PotentialFunction<ImageOutputType>::addPotentialUnaryGradientGmm(
//    const typename PotentialGmm<ImageOutputType>::Object _obj,
//    const Prior::GMM _gmm,
//    const int flagAxis
//)
//{
//    if(flagAxis == 0)
//    {
//        unaryGradientXGmm.push_back(
//            PotentialFunction<ImageOutputType>::UnaryGmm(
//                _obj,
//                _gmm
//            )
//        );
//    }
//    else
//    {
//        unaryGradientYGmm.push_back(
//            PotentialFunction<ImageOutputType>::UnaryGmm(
//                _obj,
//                _gmm
//            )
//        );
//    }
//}

//template <typename ImageOutputType>
//inline PotentialFunction<ImageOutputType>& PotentialFunction<ImageOutputType>::operator()(
//    const PotentialFunction& _potential
//)
//{
//    unaryGmm = _potential._unaryGmm();
//    unaryGradientXGmm = _potential._unaryGradientXGmm();
//    unaryGradientYGmm = _potential._unaryGradientYGmm();
//    pairwisePotts = _potential._pairwisePotts();

//    return *this;
//}

//template <typename ImageOutputType>
//inline PotentialFunction<ImageOutputType>& PotentialFunction<ImageOutputType>::operator=(
//    const PotentialFunction& _potential
//)
//{
//    unaryGmm = _potential._unaryGmm();
//    unaryGradientXGmm = _potential._unaryGradientXGmm();
//    unaryGradientYGmm = _potential._unaryGradientYGmm();
//    pairwisePotts = _potential._pairwisePotts();

//    return *this;
//}

///*!
// * \class PropertySegmentationImage
// *
// * \brief combines all information for segmentation image(s).
// *
// * \date 2014/04/05
// *
// */
//template <typename ImageOutputType = int>
//class PropertySegmentationImage
//{
//public:
//    //--------------------------------------------------------
//    //
//    //! \name Constructors / Destructor / Instance Management
//    //@{
//    //--------------------------------------------------------
//    //! Destructor.
//    ~PropertySegmentationImage(){}
//    //! Default constructor.
//    PropertySegmentationImage(
//        const std::vector<NameDatasetPair> _datasetPairAll = std::vector<NameDatasetPair>(),
//        const std::vector<NameDatasetSingle> _datasetSingleAll = std::vector<NameDatasetSingle>(),
//        const std::string _strDirOutput = std::string(),
//        const InferenceAlgorithm _infAlg = InferenceAlgorithm(),
//        const PotentialFunction<ImageOutputType> _potential = PotentialFunction<ImageOutputType>(),
//        const bool _flagEqualization = false
//    ):
//        datasetPairAll(_datasetPairAll),
//        datasetSingleAll(_datasetSingleAll),
//        strDirOutput(_strDirOutput),
//        infAlg(_infAlg),
//        potential(_potential),
//        flagEqualization(_flagEqualization)
//    {}
//    //! Copy constructor.
//    PropertySegmentationImage(
//        const PropertySegmentationImage& _prop
//    ):
//        datasetPairAll(_prop._datasetPairAll()),
//        datasetSingleAll(_prop._datasetSingleAll()),
//        strDirOutput(_prop._strDirOutput()),
//        infAlg(_prop._infAlg()),
//        potential(_prop._potential()),
//        flagEqualization(_prop._flagEqualization())
//    {}
//    //@}

//    //------------------------------------------
//    //
//    //! \name Operators
//    //@{
//    //------------------------------------------
//    //! Sets filenames \c img to \c _img and \c labeling to \c _labeling.
//    void operator()(
//        const std::vector<NameDatasetPair> _datasetPairAll = std::vector<NameDatasetPair>(),
//        const std::vector<NameDatasetSingle> _datasetSingleAll = std::vector<NameDatasetSingle>(),
//        const std::string _strDirOutput = std::string(),
//        const InferenceAlgorithm _infAlg = InferenceAlgorithm(),
//        const PotentialFunction<ImageOutputType> _potential = PotentialFunction<ImageOutputType>(),
//        const bool _flagEqualization = false
//   );
//    PropertySegmentationImage& operator=(
//        const PropertySegmentationImage& _info
//    );
//    PropertySegmentationImage& operator()(
//        const PropertySegmentationImage& _info
//    );
//    //@}

//    //------------------------------------------
//    //
//    //! \name Get / Set private member variables
//    //@{
//    //------------------------------------------
//    //! returns \c datasetPairAll.
//    std::vector<NameDatasetPair> _datasetPairAll(void) const {return datasetPairAll;}
//    //! returns \c datasetSingleAll.
//    std::vector<NameDatasetSingle> _datasetSingleAll(void) const {return datasetSingleAll;}
//    //! returns \c strDirOutput.
//    std::string _strDirOutput(void) const {return strDirOutput;}
//    //! returns \c infAlg.
//    InferenceAlgorithm _infAlg(void) const {return infAlg;}
//    //! returns \c potential.
//    PotentialFunction<ImageOutputType> _potential(void) const {return potential;}
//    //! returns \c flagEqualization
//    bool _flagEqualization(void) const {return flagEqualization;}

//    //! sets \c datasetPairAll.
//    void _datasetPairAll(const std::vector<NameDatasetPair> _datasetPairAll){datasetPairAll = _datasetPairAll;}
//    //! sets \c datasetSingleAll.
//    void _datasetSingleAll(const std::vector<NameDatasetSingle> _datasetSingleAll){datasetSingleAll = _datasetSingleAll;}
//    ///! sets \c strDirOutput.
//    void _strDirOutput(const std::string _strDirOutput){strDirOutput = _strDirOutput;}
//    //! sets \c infAlg.
//    void _infAlg(InferenceAlgorithm  _infAlg){infAlg = _infAlg;}
//    //! sets \c potential.
//    void _potential(const PotentialFunction<ImageOutputType> _potential){potential = _potential;}
//    //! sets \c flagEqualization.
//    void _flagEqualization(const bool _flagEqualization){flagEqualization = _flagEqualization;}

//    //! adds \c datasetPair object.
//    void addDatasetPair(const NameDatasetPair _datasetPair){datasetPairAll.push_back(_datasetPair);}
//    //! adds \c datasetSingle object.
//    void addDatasetSingle(const NameDatasetSingle _datasetSingle){datasetSingleAll.push_back(_datasetSingle);}
//    //@}

//private:
//    //------------------------------------------
//    //
//    //! \name Private variables
//    //@{
//    //------------------------------------------
//    //! a set of image-labeling pair dataset.
//    std::vector<NameDatasetPair> datasetPairAll;
//    //! a set of single images dataset.
//    std::vector<NameDatasetSingle> datasetSingleAll;
//    //! a directory for saving output data.
//    std::string strDirOutput;
//    //! a list of inference algorithm to be used.
//    InferenceAlgorithm infAlg;
//    //! a list of potential functions
//    PotentialFunction<ImageOutputType> potential;
//    //! flag indicating whether color of images are normalized by histogram equalization or not.
//    bool flagEqualization;
//    //@}
//};

//template <typename ImageOutputType>
//inline void PropertySegmentationImage<ImageOutputType>::operator()(
//    const std::vector<NameDatasetPair> _datasetPairAll,
//    const std::vector<NameDatasetSingle> _datasetSingleAll,
//    const std::string _strDirOutput,
//    const InferenceAlgorithm _infAlg,
//    const PotentialFunction<ImageOutputType> _potential,
//    const bool _flagEqualization
//)
//{
//    datasetPairAll = _datasetPairAll;
//    datasetSingleAll = _datasetSingleAll;
//    strDirOutput = _strDirOutput;
//    infAlg = _infAlg;
//    potential = _potential;
//    flagEqualization = _flagEqualization;
//}

//template <typename ImageOutputType>
//inline PropertySegmentationImage<ImageOutputType>& PropertySegmentationImage<ImageOutputType>::operator()(
//    const PropertySegmentationImage& _info
//)
//{
//    datasetPairAll = _info._datasetPairAll();
//    datasetSingleAll = _info._datasetSingleAll();
//    strDirOutput = _info._strDirOutput();
//    infAlg = _info._infAlg();
//    potential = _info._potential();
//    flagEqualization = _info._flagEqualization();

//    return *this;
//}

//template <typename ImageOutputType>
//inline PropertySegmentationImage<ImageOutputType>& PropertySegmentationImage<ImageOutputType>::operator=(
//    const PropertySegmentationImage& _info
//)
//{
//    datasetPairAll = _info._datasetPairAll();
//    datasetSingleAll = _info._datasetSingleAll();
//    strDirOutput = _info._strDirOutput();
//    infAlg = _info._infAlg();
//    potential = _info._potential();
//    flagEqualization = _info._flagEqualization();

//    return *this;
//}



#endif
