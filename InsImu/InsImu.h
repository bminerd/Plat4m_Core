//------------------------------------------------------------------------------
//       _______    __                           ___
//      ||  ___ \  || |             __          //  |
//      || |  || | || |   _______  || |__      //   |    _____  ___
//      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
//      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
//      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
//      || |       || |  \\____  | || |__  //_____   _| || | || | || |
//      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
//
//
// The MIT License (MIT)
//
// Copyright (c) 2017 Benjamin Minerd
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

///
/// @file InsImu.h
/// @author Ben Minerd
/// @date 4/20/2017
/// @brief InsImu class header file.
///

#ifndef PLAT4M_INS_IMU_H
#define PLAT4M_INS_IMU_H

//------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------

#include <Ins.h>
#include <ErrorTemplate.h>
#include <Callback.h>
#include <Imu.h>
#include <Matrix.h>
#include <Quaternion.h>

//------------------------------------------------------------------------------
// Namespaces
//------------------------------------------------------------------------------

namespace Plat4m
{

//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

class InsImu : public Ins
{
public:
    
    //--------------------------------------------------------------------------
    // Public types
    //--------------------------------------------------------------------------
    
    enum ErrorCode
    {
        ERROR_CODE_NONE
    };

    typedef ErrorTemplate<ErrorCode> Error;
    
    //--------------------------------------------------------------------------
    // Public constructors
    //--------------------------------------------------------------------------
    
    InsImu(Imu& imu);
    
    //--------------------------------------------------------------------------
    // Public virtual destructors
    //--------------------------------------------------------------------------
    
    virtual ~InsImu();
    
    //--------------------------------------------------------------------------
    // Public methods
    //--------------------------------------------------------------------------

    void imuMeasurementReadyCallback();

private:

    //--------------------------------------------------------------------------
    // Private static data members
    //--------------------------------------------------------------------------

    static const RealNumber myXRotationAngle;
    static const RealNumber myYRotationAngle;
    static const RealNumber myZRotationAngle;

    static const RealNumber myXRotationMatrixValues[3][3];
    static const RealNumber myYRotationMatrixValues[3][3];
    static const RealNumber myZRotationMatrixValues[3][3];
    
    static const Math::Matrix<RealNumber, 3, 3> myXRotationMatrix;
    static const Math::Matrix<RealNumber, 3, 3> myYRotationMatrix;
    static const Math::Matrix<RealNumber, 3, 3> myZRotationMatrix;

    static const Math::Matrix<RealNumber, 3, 3> myRotationMatrix;

    //--------------------------------------------------------------------------
    // Private data members
    //--------------------------------------------------------------------------

    Imu& myImu;

    AngleDegrees myIntegratedYawAngleDegrees;
    AngleDegrees myIntegratedPitchAngleDegrees;
    AngleDegrees myIntegratedRollAngleDegrees;

    AngleDegrees myYawAngleDegrees;
    AngleDegrees myPitchAngleDegrees;
    AngleDegrees myRollAngleDegrees;

    AngularVelocityDps myYawAngularVelocityDps;
    AngularVelocityDps myPitchAngularVelocityDps;
    AngularVelocityDps myRollAngularVelocityDps;

    Math::Vector<RealNumber, 3> myEulerAnglesVector;

    Math::Quaternion<RealNumber> myQuaternion;

    //--------------------------------------------------------------------------
    // Private methods implemented from Ins
    //--------------------------------------------------------------------------

    Ins::Error driverSetConfig(const Config& config);

    Ins::Error driverGetMeasurement(Measurement& measurement);

    Ins::Error driverGetEulerAnglesDegrees(AngleDegrees& yawAngleDegrees,
                                           AngleDegrees& pitchAngleDegrees,
                                           AngleDegrees& rollAngleDegrees);
};

}; // namespace Plat4m

#endif // PLAT4M_INS_IMU_H
