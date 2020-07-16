#ifndef APPLICATION_LAYER_LOWPASSFILTER_IRR_H_
#define APPLICATION_LAYER_LOWPASSFILTER_IRR_H_


/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/
#include <stdint.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


/*------------------------------------------------------------------------------
 * Templates
 *----------------------------------------------------------------------------*/
template <uint32_t numberOfPoles> class LowPassFilter_IRR
{
public:
    /*--------------------------------------------------------------------------
     * Public constructors
     *------------------------------------------------------------------------*/
    LowPassFilter_IRR(float cutOffFreq_Hz,
                      float updateFreq_Hz);

    /*--------------------------------------------------------------------------
     * Public methods
     *------------------------------------------------------------------------*/
    void initFilterValue(float initVal);

    void updateFilteredVal(float newSample);

    float getFilteredVal(void);

private:
    /*--------------------------------------------------------------------------
     * Private methods
     *------------------------------------------------------------------------*/
    void IIRfilterCalc_In_Out(float signalIn, float& signalOut);

    /*--------------------------------------------------------------------------
     * Private data members
     *------------------------------------------------------------------------*/
    float myFilterCoef;
    float myFilteredVal[(numberOfPoles + 1)];
};


/*------------------------------------------------------------------------------
 * Public constructors
 *----------------------------------------------------------------------------*/
template <uint32_t numberOfPoles>
LowPassFilter_IRR<numberOfPoles>::LowPassFilter_IRR(float cutOffFreq_Hz,
                                                    float updateFreq_Hz)
{
    if (numberOfPoles < 1) {
        while(1); //error - must have at least 1 pole
    }

    float cutOff_to_Update_Ratio = (cutOffFreq_Hz / updateFreq_Hz);

    if (cutOff_to_Update_Ratio > 0.5) {
        while(1); //error - the cutoff frequency must be <= half the update frequency
    }

    myFilterCoef = exp(-2 * M_PI * cutOff_to_Update_Ratio);

    initFilterValue(0.0);
}


/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/
template <uint32_t numberOfPoles>
void LowPassFilter_IRR<numberOfPoles>::initFilterValue(float initVal)
{
    for (uint32_t i = 0; i <= numberOfPoles; i++) {
        myFilteredVal[i] = initVal;
    }
}

template <uint32_t numberOfPoles>
void LowPassFilter_IRR<numberOfPoles>::updateFilteredVal(float newSample)
{
    myFilteredVal[0] = newSample;

    for (uint32_t i = 0; i < numberOfPoles; i++) {
        IIRfilterCalc_In_Out(myFilteredVal[i],  myFilteredVal[(i + 1)]);
    }
}

template <uint32_t numberOfPoles>
float LowPassFilter_IRR<numberOfPoles>::getFilteredVal(void)
{
    return myFilteredVal[numberOfPoles];
}


/*------------------------------------------------------------------------------
 * Private methods
 *----------------------------------------------------------------------------*/
template <uint32_t numberOfPoles>
void LowPassFilter_IRR<numberOfPoles>::IIRfilterCalc_In_Out(float signalIn,
                                                            float& signalOut)
{
    signalOut = signalIn + ((signalOut - signalIn) * myFilterCoef);
}


#endif /* APPLICATION_LAYER_LOWPASSFILTER_IRR_H_ */

