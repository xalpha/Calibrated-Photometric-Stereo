#ifndef __CPSCONFIGURATION_H__
#define __CPSCONFIGURATION_H__

/*!
 * \file CpsConfiguration.hpp
 *
 * \author Yuji Oyamada
 * \date 2014/05/16
 * \brief This file contains configuration loader for calibrated photometric stereo.
 *
 */

// STL
#include <vector>
#include <string>
#include <iostream>
#include <cassert>

// internal headers
#include "DataStructure.hpp"

#include "configurationCPS.hxx"

//! returns configuration of calibrated photometric stereo.
CPS::CpsConfig loadConfiguration(
    const std::string strFile
)
{
    CPS::CpsConfig cpsConfig;
    try
    {
        std::cout << "Load configuration from " << strFile << std::endl;
        std::auto_ptr<CalibratedPhotometricStereoType> _config(CalibratedPhotometricStereo(strFile));
        CalibratedPhotometricStereoType config( *_config );

        // loads directory name for outputs.
        cpsConfig.strDirOutput( config.DirectoryOutput() );
        // loads name of reflection model.
        cpsConfig.strReflection( config.ReflectanceModel() );

        // loads all observation information.
        ObservationType observationAll = config.Observation();
        // loads directory name for inputs.
        cpsConfig.strDirObservation( observationAll.DirectoryObservation() );
        // loads name of image mask.
        if( observationAll.ObservationMask() )
        {
            cpsConfig.strImageMask( cpsConfig.obsAll().strDirObservation() + observationAll.ObservationMask().get() );
        }
        // loads each observation information.
        ObservationType::ObservationSingle_sequence observationSingleSequence = observationAll.ObservationSingle();
        for(int n = 0; n < observationSingleSequence.size(); ++n)
        {
            // loads name of image, light source direction, and light source intensity.
            cpsConfig.addObservation(
                CPS::ObservationSingle(
                    cpsConfig.obsAll().strDirObservation() + observationSingleSequence[n].Image(),
                    observationSingleSequence[n].LightDirection(),
                    observationSingleSequence[n].LightIntensity()
                )
            );
        }
        // loads the number of color channels.
        cpsConfig.color( config.Observation().Color() );

        return cpsConfig;
    }
    catch (const xml_schema::exception& e)
    {
        std::cerr << e << std::endl;
    }

}

//! shows loaded configuration of calibrated photometric stereo.
void showConfiguration(
    const CPS::CpsConfig& cpsConfig
)
{
    std::cout << "The CPS configuration: " << std::endl;
    std::cout << "  Directory for input:  " << cpsConfig.strDirObservation() << std::endl;
    std::cout << "  Directory for output: " << cpsConfig.strDirOutput() << std::endl;
    std::cout << "  Reflectance model: " << cpsConfig.strReflection() << std::endl;
    std::cout << "  Image mask: " << cpsConfig.strImageMask() << std::endl;
    std::cout << "  Total number of images is " << cpsConfig.numberOfObservation() << std::endl;
    std::cout << "  Number of color channel is " << cpsConfig.color() << std::endl;
    for(int n = 0; n < cpsConfig.numberOfObservation(); ++n)
    {
        std::cout << "  Image[" << n << "]:" << std::endl;
        std::cout << "      " << cpsConfig.observationSingle(n).strImage() << std::endl;
        std::cout << "      " << cpsConfig.observationSingle(n).lightDirection() << std::endl;
        std::cout << "      " << cpsConfig.observationSingle(n).lightIntensity() << std::endl;
    }
}

#endif
