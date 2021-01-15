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

#ifndef CO_SIM_IO_C_MODEL_PART_INCLUDED
#define CO_SIM_IO_C_MODEL_PART_INCLUDED


typedef struct CoSimIO_ModelPart
{
    void* PtrCppInfo;
} CoSimIO_ModelPart;


CoSimIO_ModelPart CoSimIO_CreateModelPart();

int CoSimIO_FreeModelPart(CoSimIO_ModelPart I_ModelPart);

// int CoSimIO_Info_Has(const CoSimIO_ModelPart I_Info, const char* I_Key);
// void CoSimIO_Info_Erase(const CoSimIO_ModelPart I_Info, const char* I_Key);
// void CoSimIO_Info_Clear(const CoSimIO_ModelPart I_Info);
// int CoSimIO_Info_Size(const CoSimIO_ModelPart I_Info);

// int CoSimIO_Info_GetInt(const CoSimIO_ModelPart I_Info, const char* I_Key);
// double CoSimIO_Info_GetDouble(const CoSimIO_ModelPart I_Info, const char* I_Key);
// int CoSimIO_Info_GetBool(const CoSimIO_ModelPart I_Info, const char* I_Key);
// const char* CoSimIO_Info_GetString(const CoSimIO_ModelPart I_Info, const char* I_Key);

// void CoSimIO_Info_SetInt(CoSimIO_ModelPart I_Info, const char* I_Key, const int I_Value);
// void CoSimIO_Info_SetDouble(CoSimIO_ModelPart I_Info, const char* I_Key, const double I_Value);
// void CoSimIO_Info_SetBool(CoSimIO_ModelPart I_Info, const char* I_Key, const int I_Value);
// void CoSimIO_Info_SetString(CoSimIO_ModelPart I_Info, const char* I_Key, const char* I_Value);

#endif // CO_SIM_IO_C_MODEL_PART_INCLUDED
