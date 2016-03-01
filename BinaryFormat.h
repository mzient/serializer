/* 
 * File:   BinaryFormat.h
 * Author: michal
 *
 * Created on March 1, 2016, 2:40 AM
 */

#ifndef BINARYFORMAT_H
#define	BINARYFORMAT_H

#include "Stream.h"

namespace Serialize
{
    struct BinaryFormat {};
    
    typedef FormattedIStream<BinaryFormat> BinaryInput;
    typedef FormattedOStream<BinaryFormat> BinaryOutput;
    
    inline BinaryInput binary(IInputStream &input)
    {
        return BinaryInput(input);
    }
    inline BinaryOutput binary(IOutputStream &output)
    {
        return BinaryOutput(output);
    }
};

#endif	/* BINARYFORMAT_H */

