////////////////////////////////////////////////////////////////
// header files


// OpenMP
#ifdef _OPENMP
#include <omp.h>
#endif

// Internal header files (utilities for handling data)
#include "utilString.hpp"
#include "utilFile.hpp"

// Internal header files (modules for processing)
#include "CpsConfiguration.hpp"
#include "PhotometricStereoSolver.hpp"

//! checks input arguments and returns the filename of configuration file as \c std::string.
std::string checkInputArguments(
    int argc,
    char* argv[]
)
{
    assert(
        (argc > 1) &&
        "\n [Main] \n The input argument for this program must be larger than 1.\n The second argument is supposed to specify an xml file, which contains all configuration."
    );
    std::string str = argv[1];
    assert(
        UtilFile::checkFileExist(str) &&
        "Specified xml file does not exist!"
    );

    return str;
}

int main(int argc, char* argv[])
{
    typedef float DataType;
    srand(time(NULL));

    std::string strFileConfig = checkInputArguments(argc, argv);

    CPS::CalibratedPhotometricStereo<DataType> cps(
        loadConfiguration(
             strFileConfig
         )
    );
    showConfiguration( cps.config() );

    int width;
    int height;
    std::vector<int> indexOfPixels;
    loadAvailablePixels(
        cps.config().strImageMask(),
        width,
        height,
        indexOfPixels
    );

    cps.width(width);
    cps.height(height);
    cps.color(cps.config().color());
    cps.indexOfPixels(indexOfPixels);

    // build observation matrix and light source matrix.
    cps.I(
        buildObservationMatrix<DataType>(
            cps.indexOfPixels(),
            cps.config().obsAll().observation(),
            cps.color(),
            cps.width()
        )
    );
    cps.L(
        buildLightSourceMatrix<DataType>(
            cps.config().obsAll().observation()
        )
    );

    // solve S given I and L.
    cps.S(
        estimateSurface(
            cps.I(),
            cps.L()
        )
    );

    // solve R given S.
    cps.R(
        estimateSurfaceAlbedo(
            cps.S()
        )
    );
    cimg_library::CImg<DataType> imgR = saveSurfaceAlbedoToImage(
        cps.R(),
        cps.indexOfPixels(),
        cps.width(),
        cps.height(),
        cps.color(),
        cps.config().strDirOutput() + "surfaceAlbedo.png"
    );

    // solve N given S and R.
    cps.N(
        estimateSurfaceNormal(
            cps.S(),
            cps.R(),
            cps.numberOfPixels(),
            cps.color()
        )
    );
    cimg_library::CImg<DataType> imgN = saveSurfaceNormalToImage(
        cps.N(),
        cps.indexOfPixels(),
        cps.width(),
        cps.height(),
        cps.config().strDirOutput() + "surfaceNormal.png"
    );

    // compute reprojection error.
    cps.Idiff(
        computeErrorLambertian(
            cps.I(),
            cps.S(),
            cps.L()
        )
    );
    cimg_library::CImg<DataType> imgDiff = saveReprojectionError(
        cps.Idiff(),
        cps.indexOfPixels(),
        cps.width(),
        cps.height(),
        cps.color(),
        cps.config().strDirOutput() + "reprojectionError.png"
    );

    (imgR, imgN, imgDiff).display("Surface albedo, surface normal, and reprojection error");

    return 0;
}
