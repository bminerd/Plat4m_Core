/*------------------------------------------------------------------------------
 *       _______    __                           ___
 *      ||  ___ \  || |             __          //  |
 *      || |  || | || |   _______  || |__      //   |    _____  ___
 *      || |__|| | || |  // ___  | ||  __|    // _  |   ||  _ \/ _ \
 *      ||  ____/  || | || |  || | || |      // /|| |   || |\\  /\\ \
 *      || |       || | || |__|| | || |     // /_|| |_  || | || | || |
 *      || |       || |  \\____  | || |__  //_____   _| || | || | || |
 *      ||_|       ||_|       ||_|  \\___|       ||_|   ||_| ||_| ||_|
 *
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Benjamin Minerd
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *----------------------------------------------------------------------------*/

/**
 * @file Sequencer.cpp
 * @author Ben Minerd
 * @date 2/5/2014
 * @brief Sequencer class.
 */

/*------------------------------------------------------------------------------
 * Include files
 *----------------------------------------------------------------------------*/

#include <Sequencer.h>

/*------------------------------------------------------------------------------
 * Public constructors and destructors
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
Sequencer::Sequencer(void):
    myIsEnabled(false),
    myMaxTicks(0),
    mySequence(0),
    myPosition(0)
{
}

/*------------------------------------------------------------------------------
 * Public methods
 *----------------------------------------------------------------------------*/

//------------------------------------------------------------------------------
void Sequencer::loadSequence(const uint32_t sequence, const uint8_t maxTicks)
{
   mySequence = sequence;
   myMaxTicks = maxTicks;
   resetSequence();
   enableSeq(true);
}


//------------------------------------------------------------------------------
bool Sequencer::isEnabled(void)
{
  return(myIsEnabled);
}

//------------------------------------------------------------------------------
void Sequencer::next(void)
{
   if(!myIsEnabled)
   {
      return;
   }

   if(myPosition < myMaxTicks-1)
   { 
      myPosition++;
   }
   else
   {
      myPosition = 0;
   }
}
//------------------------------------------------------------------------------
void Sequencer::resetSequence(void)
{
  myPosition = 0;
}
//------------------------------------------------------------------------------
bool Sequencer::getSequenceVal(bool& val)
{
   if(!myIsEnabled)
   {
      return(false);
   }
   val = (uint8_t)((mySequence >> myPosition) & 0x1);
   return(true);
}

//------------------------------------------------------------------------------
void Sequencer::getSequencePosition(uint8_t& pos)
{
   if(!myIsEnabled)
   {
      return;
   }
   pos = myPosition;
}

/*--------------------------------------------------------------------------
 * Private methods
 *------------------------------------------------------------------------*/
//------------------------------------------------------------------------------
void Sequencer::enableSeq(const bool enable)
{
    if (enable == myIsEnabled)
    {
        return;
    }   
    myIsEnabled = enable;
}
