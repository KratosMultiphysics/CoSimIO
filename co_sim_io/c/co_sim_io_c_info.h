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

#ifndef CO_SIM_IO_C_INFO_INCLUDED
#define CO_SIM_IO_C_INFO_INCLUDED


// #define CreateNewGetValue(type) \
// type ##type,  const char* Key)

// #define DefineNewGetValue(type) \
// type GetValue##type(char* label, CoSimIO_TransferInfo);

typedef struct CoSimIO_Info
{
    void* PtrCppInfo;
} CoSimIO_Info;


CoSimIO_Info CoSimIO_CreateInfo();

int CoSimIO_FreeInfo(CoSimIO_Info I_Info);

int CoSimIO_Info_Has(const CoSimIO_Info I_Info, const char* I_Key);
void CoSimIO_Info_Erase(const CoSimIO_Info I_Info, const char* I_Key);
void CoSimIO_Info_Clear(const CoSimIO_Info I_Info);
int CoSimIO_Info_Size(const CoSimIO_Info I_Info);

int CoSimIO_Info_GetInt(const CoSimIO_Info I_Info, const char* I_Key);
double CoSimIO_Info_GetDouble(const CoSimIO_Info I_Info, const char* I_Key);
int CoSimIO_Info_GetBool(const CoSimIO_Info I_Info, const char* I_Key);
const char* CoSimIO_Info_GetString(const CoSimIO_Info I_Info, const char* I_Key);

void CoSimIO_Info_SetInt(CoSimIO_Info I_Info, const char* I_Key, const int I_Value);
void CoSimIO_Info_SetDouble(CoSimIO_Info I_Info, const char* I_Key, const double I_Value);
void CoSimIO_Info_SetBool(CoSimIO_Info I_Info, const char* I_Key, const int I_Value);
void CoSimIO_Info_SetString(CoSimIO_Info I_Info, const char* I_Key, const char* I_Value);

#endif // CO_SIM_IO_C_INFO_INCLUDED
