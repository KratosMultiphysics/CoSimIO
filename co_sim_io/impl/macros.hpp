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

#ifndef CO_SIM_IO_MACROS_INCLUDED
#define CO_SIM_IO_MACROS_INCLUDED

/* This file defines macros that are used inside the CoSimIO
Note that they are only defined here if they haven't been defined before.
This makes it possible to override them to use macros that are coming from
the code where the CoSimIO is included
*/

#ifndef CO_SIM_IO_ERROR
    #include <iostream>
    #include <string>
    #include <stdexcept>
    #include <sstream>
    #include <vector>
    #include "code_location.h"

    namespace CoSimIO {

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
            buffer << mMessage << std::endl;
            if (mCallStack.empty()) {
                buffer << "in Unknown Location";
            } else {
                buffer << "in " << mCallStack[0] << std::endl;
                for (auto i = mCallStack.begin()+1; i != mCallStack.end(); ++i) {
                    buffer << "   " << *i << std::endl;
                }
            }
            mWhat = buffer.str();
        }
    }; // class Exception

    } // namespace CoSimIO

    #define CO_SIM_IO_ERROR throw CoSimIO::Exception("Error: ", CO_SIM_IO_CODE_LOCATION)
#endif

#ifndef CO_SIM_IO_ERROR_IF
    #define CO_SIM_IO_ERROR_IF(conditional) if (conditional) throw CoSimIO::Exception("Error: ", CO_SIM_IO_CODE_LOCATION)
#endif

#ifndef CO_SIM_IO_ERROR_IF_NOT
    #define CO_SIM_IO_ERROR_IF_NOT(conditional) if (!(conditional)) throw CoSimIO::Exception("Error: ", CO_SIM_IO_CODE_LOCATION)
#endif

#ifndef CO_SIM_IO_INFO
    #include <iostream>
    #define CO_SIM_IO_INFO(label) std::cout << label << ": "
#endif

#ifndef CO_SIM_IO_INFO_IF
    #define CO_SIM_IO_INFO_IF(label, conditional) if (conditional) CO_SIM_IO_INFO(label)
#endif

#endif // CO_SIM_IO_MACROS_INCLUDED
