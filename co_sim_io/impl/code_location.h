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

#if !defined(CO_SIM_IO_CODE_LOCATION_H_INCLUDED)
#define CO_SIM_IO_CODE_LOCATION_H_INCLUDED

// System includes
#include <string>
#include <iostream>

namespace CoSimIO
{
///@addtogroup KratosCore
///@{

/// This class keeps a code location consist of filename, function name and line number.
/// It also provides methods to get cleaned version of filename and function name.
/// Adapted from Kratos (https://github.com/KratosMultiphysics/Kratos/blob/master/kratos/includes/code_location.h)
class  CodeLocation
{
public:

	CodeLocation(std::string const& FileName,
                 std::string const& FunctionName,
                 std::size_t LineNumber) :
		mFileName(FileName),
        mFunctionName(FunctionName),
        mLineNumber(LineNumber) {}

    CodeLocation() : CodeLocation("Unknown", "Unknown", 0) {}

	CodeLocation(CodeLocation const & Other) :
		mFileName(Other.mFileName),
        mFunctionName(Other.mFunctionName),
        mLineNumber(Other.mLineNumber) {}


    ///@}
    ///@name Private Operators
    ///@{

    CodeLocation& operator=(CodeLocation const& Other) {
        mFileName = Other.mFileName;
        mFunctionName = Other.mFunctionName;
        mLineNumber = Other.mLineNumber;

        return *this;
    }

    ///@name Operations
    ///@{

    /// This function removes the path before the Kratos root
    std::string CleanFileName() const;

    /// This method cleans many template arguments and namespaces from the function name gives by compiler
    std::string CleanFunctionName() const;


    ///@}
    ///@name Access
    ///@{

    const std::string& GetFileName() const
    {
        return mFileName;
    }

    const std::string& GetFunctionName() const
    {
        return mFunctionName;
    }

    int GetLineNumber() const
    {
        return mLineNumber;
    }

    std::string GetLocation() const
    {
        return GetFileName() + ":" + std::to_string(GetLineNumber()) + ":" + GetFunctionName();
    }

    ///@}

private:
    ///@name Member Variables
    ///@{

    std::string mFileName;
    std::string mFunctionName;
    std::size_t mLineNumber;

    ///@}
    ///@name Private Operations
    ///@{

    // static void RemoveNamespace(std::string& FunctionName, const std::string& Namespace);

    // static void ReduceTemplateArgumentsToFirstN(std::string& FunctionName, const std::string& TemplateName, std::size_t NumberOfArgumentsToKeep);

    // static std::size_t GetNextPositionSkippingWhiteSpaces(std::string const& ThisString, std::size_t Position);

    // static bool IsWhiteSpace(char C);

    // static void ReplaceAll(std::string& ThisString, const std::string& FromString, const std::string& ToString);

    ///@}

}; // Class CodeLocation

///@}
///@name Input and output
///@{

// /// output stream function
// std::ostream & operator <<(std::ostream& rOStream,
//     const CodeLocation& rLocation)
// {
//     rOStream << rLocation.GetFileName() << ":" << rLocation.GetLineNumber() << ":" << rLocation.GetFunctionName();
//     return rOStream;
// }
// ///@}

#if defined(CO_SIM_IO_CODE_LOCATION)
#undef CO_SIM_IO_CODE_LOCATION
#endif

#if defined(CO_SIM_IO_CURRENT_FUNCTION)
#undef CO_SIM_IO_CURRENT_FUNCTION
#endif

#if defined(__PRETTY_FUNCTION__)
#define CO_SIM_IO_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__GNUC__)
#define CO_SIM_IO_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCTION__)
#define CO_SIM_IO_CURRENT_FUNCTION __FUNCTION__
#elif defined(__func__)
#define CO_SIM_IO_CURRENT_FUNCTION __func__
#else
#define CO_SIM_IO_CURRENT_FUNCTION "unknown function"
#endif


#define CO_SIM_IO_CODE_LOCATION CoSimIO::CodeLocation(__FILE__, CO_SIM_IO_CURRENT_FUNCTION, __LINE__)


///@} addtogroup block

}  // namespace CoSimIO.

#endif // CO_SIM_IO_CODE_LOCATION_H_INCLUDED
