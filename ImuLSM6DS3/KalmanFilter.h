/*------------------------------------------------------------------------------
 *                  __                           ___
 *                 || |             __          //  |
 *       _______   || |   _______  || |__      //   |    _____  ___
 *      ||  ___ \  || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      || |  || | || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |__|| | || | || |__|| | || |     // /_|| |_  || | || | || |
 *      ||  ____/  || |  \\____  | || |__  //_____   _| || | || | || |
 *      || |       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *      || |
 *      ||_|
 *
 * Copyright (c) 2013 Ben Minerd. All rights reserved.
 *
 * GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of this file.
 * Please review the following information to ensure the GNU Lesser General
 * Public License version 2.1 requirements will be met:
 * http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *----------------------------------------------------------------------------*/

///
/// @file KalmanFilter.h
/// @author Ben Minerd
/// @date 3/28/13
/// @brief KalmanFilter class.
///

#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Eigen/Dense>
#include <Eigen/SVD>
#include <float.h>

/*------------------------------------------------------------------------------
 * Classes
 *----------------------------------------------------------------------------*/

namespace Plat4m
{

template<typename ValueType, int nStates, int nObservables, int nControlInputs>
class KalmanFilter
{
public:

	/*--------------------------------------------------------------------------
	 * Public typedefs
	 *------------------------------------------------------------------------*/

	typedef Eigen::Matrix<ValueType, nStates, nStates> MatrixNbyN;

	typedef Eigen::Matrix<ValueType, nStates, nObservables> MatrixNbyM;

	typedef Eigen::Matrix<ValueType, nObservables, nStates> MatrixMbyN;

	typedef Eigen::Matrix<ValueType, nObservables, nObservables> MatrixMbyM;

	typedef Eigen::Matrix<ValueType, nStates, 1> VectorN;

	typedef Eigen::Matrix<ValueType, nObservables, 1> VectorM;

	/*--------------------------------------------------------------------------
	 * Public constructors
	 *------------------------------------------------------------------------*/

	KalmanFilter() :
		myXVector(),
		myFMatrix(),
		myUVector(),
		myBMatrix(),
		myHMatrix(),
		myPMatrix(),
		myQMatrix(),
		myRMatrix()
	{
	}

	/*--------------------------------------------------------------------------
	 * Public methods
	 *------------------------------------------------------------------------*/

	//--------------------------------------------------------------------------
	void predict()
	{
		// State estimate
		// X = (F * X) + (B * U)
		//
		// Where...
		// X = state vector
		// F = state transition matrix
		// B = control matrix
		// U = control input vector
		//
		myXVector = (myFMatrix * myXVector) + (myBMatrix * myUVector);

		// Covariance estimate
		// P = (F * P * F_T) + Q
		myPMatrix = (myFMatrix * myPMatrix * myFMatrix.transpose()) + myQMatrix;
	}

	//--------------------------------------------------------------------------
	void update(const VectorM& measurementVector)
	{
		VectorM yMatrix;
		MatrixMbyM sMatrix;
		MatrixMbyM sMatrixInverse;
		MatrixNbyM kMatrix;

		// Measurement
		yMatrix = measurementVector - (myHMatrix * myXVector);

		// Residual covariance

		sMatrix = (myHMatrix * myPMatrix * myHMatrix.transpose()) + myRMatrix;

		sMatrixInverse = pseudoInverse(sMatrix);

		// Kalman gain
		kMatrix = myPMatrix * myHMatrix.transpose() * sMatrixInverse;

		// Updated state estimate
		myXVector = myXVector + kMatrix * yMatrix;
		// Updated estimate covariance
		myPMatrix = (MatrixNbyN::Identity() - kMatrix * myHMatrix) * myPMatrix;
	}

	//--------------------------------------------------------------------------
	static MatrixMbyM pseudoInverse(const MatrixMbyM& matrix)
	{
		// Compute Moore-Penrose pseudo inverse

		ValueType pinvTolerance = FLT_EPSILON;

		Eigen::JacobiSVD< MatrixMbyM > matrixSvd(matrix,
											     Eigen::ComputeFullU |
												 Eigen::ComputeFullV);
		VectorM inverseSvdValues;

		for (int i = 0; i < nObservables; i++)
		{
		    if (matrixSvd.singularValues()(i) > pinvTolerance)
		    {
		        inverseSvdValues(i) = 1.0f / matrixSvd.singularValues()(i);
		    }
		    else
		    {
		        inverseSvdValues(i) = 0.0f;
		    }
		}

		MatrixMbyM matrixInverse;

		matrixInverse =
		    (matrixSvd.matrixV()           *
		     inverseSvdValues.asDiagonal() *
			 matrixSvd.matrixU().transpose());

		return matrixInverse;
	}

	//--------------------------------------------------------------------------
	VectorN& getXVector()
	{
		return myXVector;
	}

	//--------------------------------------------------------------------------
	VectorN& getStateVector()
	{
		return myXVector;
	}

	//--------------------------------------------------------------------------
	MatrixNbyN& getFMatrix()
	{
		return myFMatrix;
	}

	//--------------------------------------------------------------------------
	MatrixNbyN& getStateTransitionMatrix()
	{
		return myFMatrix;
	}

	//--------------------------------------------------------------------------
	Eigen::Matrix<ValueType, nControlInputs, 1>& getUVector()
	{
		return myUVector;
	}

	//--------------------------------------------------------------------------
	Eigen::Matrix<ValueType, nControlInputs, 1>& getControlInputVector()
	{
		return myUVector;
	}

	//--------------------------------------------------------------------------
	Eigen::Matrix<ValueType, nStates, nControlInputs>& getBMatrix()
	{
		return myBMatrix;
	}

	//--------------------------------------------------------------------------
	Eigen::Matrix<ValueType, nStates, nControlInputs>& getControlMatrix()
	{
		return myBMatrix;
	}

	//--------------------------------------------------------------------------
	MatrixMbyN& getHMatrix()
	{
		return myHMatrix;
	}

	//--------------------------------------------------------------------------
	MatrixMbyN& getObservationMatrix()
	{
		return myHMatrix;
	}

	//--------------------------------------------------------------------------
	MatrixNbyN& getPMatrix()
	{
		return myPMatrix;
	}

	//--------------------------------------------------------------------------
	MatrixNbyN& getPredictedCovarianceMatrix()
	{
		return myPMatrix;
	}

	//--------------------------------------------------------------------------
	MatrixNbyN& getQMatrix()
	{
		return myQMatrix;
	}

	//--------------------------------------------------------------------------
	MatrixNbyN& getProcessErrorCovarianceMatrix()
	{
		return myQMatrix;
	}

	//--------------------------------------------------------------------------
	MatrixMbyM& getRMatrix()
	{
		return myRMatrix;
	}

	//--------------------------------------------------------------------------
	MatrixMbyM& getMeasurementErrorCovarianceMatrix()
	{
		return myRMatrix;
	}

private:

	/*--------------------------------------------------------------------------
	 * Private data members
	 *------------------------------------------------------------------------*/

	///
	/// @brief Contains the current values of the filtered state variables. Also
	/// called state vector.
	///
	VectorN myXVector;

	///
	/// @brief Defines how the state variables relate to each other. Also called
	/// state transition matrix.
	///
	MatrixNbyN myFMatrix;

	///
	/// @brief Stores the control input values. Also called control input
	/// vector.
	///
	Eigen::Matrix<ValueType, nControlInputs, 1> myUVector;

	///
	/// @brief Defines how the control inputs relate to the state variables.
	/// Also called control matrix.
	///
	Eigen::Matrix<ValueType, nStates, nControlInputs> myBMatrix;

	///
	/// @brief Defines what state variables are physically observed or measured.
	/// Also called observation matrix.
	///
	MatrixMbyN myHMatrix;

	///
	/// @brief Contains the current covariance estimate for each state variable.
	/// Also called predicted covariance matrix.
	///
	MatrixNbyN myPMatrix;

	///
	/// @brief Defines the process error covariance estimates for each state
	/// variable. Also called process error covariance matrix.
	///
	MatrixNbyN myQMatrix;

	///
	/// @brief Defines the measurement error covariance estimates for each state
	/// variable. Also called measurement error covariance matrix.
	///
	MatrixMbyM myRMatrix;
};

}; // namespace Plat4m

#endif // KALMAN_FILTER_H
