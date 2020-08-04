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

#ifndef CO_SIM_IO_MODEL_PART_H_INCLUDED
#define CO_SIM_IO_MODEL_PART_H_INCLUDED

/* This file contains the implementation of th  CoSimIO::ModelPart
It serves as a data container when exchanging data
Also it is used in order to be consitent with Kratos to reduce compatibility problems
This is a simplified version of Kratos::ModelPart
see https://github.com/KratosMultiphysics/Kratos/blob/master/kratos/includes/model_part.h
*/

// System includes
#include <array>
#include <vector>

// Project includes
#include "macros.hpp"

class Node
{
public:
    using CoordinatesType = std::array<double,3>;

    Node(
        const int I_Id,
        const double I_X,
        const double I_Y,
        const double I_Z)
    : mId(I_Id),
      mX(I_X),
      mY(I_Y),
      mZ(I_Z)
    { }

    Node(
        const int I_Id,
        const CoordinatesType& I_Coordinates)
    : mId(I_Id),
      mX(I_Coordinates[0]),
      mY(I_Coordinates[1]),
      mZ(I_Coordinates[2])
    { }

    int GetId() const;
    double GetX() const;
    double GetY() const;
    double GetZ() const;

private:
    int mId;
    double mX;
    double mY;
    double mZ;
};

class Element
{
public:
    using ElementType = int;
    using ConnectivitiesType = std::vector<int>;

    Element(
        const int I_Id,
        const ElementType I_Type,
        const ConnectivitiesType& I_Connectivities)
    : mId(I_Id),
      mType(I_Type),
      mConnectivities(I_Connectivities)
    { }

    int GetId() const;
    ElementType GetType() const;
    const ConnectivitiesType& GetConnectivities() const;

private:
    int mId;
    int mType;
    std::vector<int> mConnectivities;
};

class ModelPart
{
private:
    // does it make sense to have it private to prevent its usage?
    // => the iterators are in the interface anyway and one can probably get the type somehow...
    using NodesContainerType = std::vector<Node>;
    using ElementsContainerType = std::vector<Element>;

public:
    int NumberOfNodes() const
    {
        return mNodes.size();
    }

    int NumberOfElements() const
    {
        return mElements.size();
    }

    Node& CreateNewNode(
        const int I_Id,
        const double I_X,
        const double I_Y,
        const double I_Z);

    Element& CreateNewElement(
        const int I_Id,
        const Element::ElementType I_Type,
        const Element::ConnectivitiesType& I_Connectivities);

    NodesContainerType::iterator NodesBegin();
    ElementsContainerType::iterator ElementsBegin();

    NodesContainerType::const_iterator NodesBegin() const;
    ElementsContainerType::const_iterator ElementsBegin() const;

    Node& GetNode(const int Id);
    Element& GetElement(const int Id);

    const Node& GetNode(const int Id) const;
    const Element& GetElement(const int Id) const;

private:
    NodesContainerType mNodes;
    ElementsContainerType mElements;
};

#endif // CO_SIM_IO_MODEL_PART_H_INCLUDED
