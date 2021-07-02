//     ______     _____ _           ________
//    / ____/___ / ___/(_)___ ___  /  _/ __ |
//   / /   / __ \\__ \/ / __ `__ \ / // / / /
//  / /___/ /_/ /__/ / / / / / / // // /_/ /
//  \____/\____/____/_/_/ /_/ /_/___/\____/
//  Kratos CoSimulationApplication
//
//  License:         BSD License, see license.txt
//
//  Main authors:    Pooyan Dadvand
//                   Philipp Bucher (https://github.com/philbucher)
//

#ifndef CO_SIM_IO_FILE_SERIALIZER_INCLUDED
#define CO_SIM_IO_FILE_SERIALIZER_INCLUDED

// System includes
#include <iomanip>

// Project includes
#include "serializer.h"

namespace CoSimIO {

// This class provides a simpler interface for serialization to a file
// Note that you may not override any load or save method of the Serializer. They are not virtual
class FileSerializer : public Serializer
{
  public:
    ///this constructor simply wraps the standard Serializer and defines output to basic_iostream
    ///@param rTrace type of serialization to be employed
    FileSerializer(std::string const& Filename, Serializer::TraceType const& rTrace=SERIALIZER_NO_TRACE)
        : Serializer(nullptr, rTrace)
    {
        std::fstream* p_file = new std::fstream(std::string(Filename+".rest").c_str(), std::ios::binary|std::ios::in|std::ios::out);
        if(!(*p_file))
        {
            delete p_file;
            p_file = new std::fstream(std::string(Filename+".rest").c_str(), std::ios::binary|std::ios::out);
        }
        SetBuffer( p_file );
        CO_SIM_IO_ERROR_IF_NOT(*pGetBuffer()) << "Error opening input file : "
                                    << std::string(Filename+".rest") << std::endl;

        // increase precision when using ascii
        if (rTrace != SERIALIZER_NO_TRACE) {
            *pGetBuffer() << std::setprecision(14);
        }
    }

    /// Assignment operator.
    FileSerializer& operator=(FileSerializer const& rOther) = delete;

    /// Copy constructor.
    FileSerializer(FileSerializer const& rOther) = delete;
};

} // namespace CoSimIO

#endif // CO_SIM_IO_FILE_SERIALIZER_INCLUDED
