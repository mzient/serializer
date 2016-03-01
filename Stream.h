/* 
 * File:   Stream.h
 * Author: michal
 *
 * Created on March 1, 2016, 2:25 AM
 */

#ifndef STREAM_H
#define	STREAM_H

#include <exception>

namespace Serialize
{

    struct EndOfStream : std::exception
    {
        const char *what() const noexcept
        {
            return "End of stream";
        }
    };

    struct IOutputStream
    {
        virtual void write(const void *data, std::size_t size)=0;
    };

    struct IInputStream
    {
        virtual std::size_t read_some(void *data, std::size_t size)=0;
        inline void read(void *data, std::size_t size)
        {
            if (read_some(data, size) != size)
                throw EndOfStream();
        }
    };


    template <class Format>
    struct FormattedOStream
    {
        explicit FormattedOStream(IOutputStream &os) : stream(os) {}
        inline void write(const void *data, std::size_t size) const
        {
            stream.write(data, size);
        }
        operator IOutputStream &() const { return stream; }
        IOutputStream &stream;
    };
    template <class Format>
    struct FormattedIStream
    {
        explicit FormattedIStream(IInputStream &is) : stream(is) {}
        inline void read(void *data, std::size_t size) const
        {
            stream.read(data, size);
        }
        operator IInputStream &() const { return stream; }
        IInputStream &stream;
    };
    
}

#endif	/* STREAM_H */
