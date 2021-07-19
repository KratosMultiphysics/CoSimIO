//     ______     _____ _           ________
//    / ____/___ / ___/(_)___ ___  /  _/ __ |
//   / /   / __ \\__ \/ / __ `__ \ / // / / /
//  / /___/ /_/ /__/ / / / / / / // // /_/ /
//  \____/\____/____/_/_/ /_/ /_/___/\____/
//  Kratos CoSimulationApplication
//
//  License:         BSD License, see license.txt
//
//  Main authors:    Philipp Bucher (https://github.com/philbucher)
//

#ifndef CO_SIM_IO_EXCEPTION_INCLUDED
#define CO_SIM_IO_EXCEPTION_INCLUDED

// System includes
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <vector>

// Project includes
#include "code_location.h"

namespace CoSimIO {

// Extends std::exception to contain more information about error location
// Simplified version of kratos/includes/exception.h

class Exception : public std::exception
{
  public:
    explicit Exception(const std::string& rWhat)
        : std::exception(),
            mMessage(rWhat),
            mCallStack()
    {
        update_what();
    }

    Exception(const std::string& rWhat, const CodeLocation& rLocation)
        : std::exception(),
            mMessage(rWhat),
            mCallStack()
    {
        add_to_call_stack(rLocation);
        update_what();
    }

    const char* what() const noexcept override
    {
        return mWhat.c_str();
    }

    const std::string& message() const
    {
        return mMessage;
    }

    /// string stream function
    template<class StreamValueType>
    Exception& operator << (StreamValueType const& rValue)
    {
        std::stringstream buffer;
        buffer << rValue;

        append_message(buffer.str());

        return *this;
    }

    Exception& operator << (std::ostream& (*pf)(std::ostream&))
    {
        std::stringstream buffer;
        pf(buffer);

        append_message(buffer.str());

        return *this;
    }

    Exception& operator << (const char* pString)
    {
        append_message(pString);
        return *this;
    }

  private:
    std::string mWhat;
    std::string mMessage;
    std::vector<CodeLocation> mCallStack;

    void append_message(const std::string& rMessage)
    {
        mMessage.append(rMessage);
        update_what();
    }

    void add_to_call_stack(const CodeLocation& rLocation)
    {
        mCallStack.push_back(rLocation);
        update_what();
    }

    void update_what(){
        std::stringstream buffer;
        buffer << mMessage << "\n";
        if (mCallStack.empty()) {
            buffer << "in Unknown Location";
        } else {
            buffer << "in " << mCallStack[0] << "\n";
            for (auto i = mCallStack.begin()+1; i != mCallStack.end(); ++i) {
                buffer << "   " << *i << "\n";
            }
        }
        mWhat = buffer.str();
    }
}; // class Exception

} // namespace CoSimIO

#endif // CO_SIM_IO_EXCEPTION_INCLUDED
