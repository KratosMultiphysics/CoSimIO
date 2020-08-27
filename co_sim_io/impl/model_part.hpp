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
#include "define.hpp"
#include "macros.hpp"

namespace CoSimIO {
class Node
{
public:
    Node(
        const IdType I_Id,
        const double I_X,
        const double I_Y,
        const double I_Z)
    : mId(I_Id),
      mX(I_X),
      mY(I_Y),
      mZ(I_Z)
    { }

    Node(
        const IdType I_Id,
        const CoordinatesType& I_Coordinates)
    : mId(I_Id),
      mX(I_Coordinates[0]),
      mY(I_Coordinates[1]),
      mZ(I_Coordinates[2])
    { }

    IdType Id() const;
    double X() const;
    double Y() const;
    double Z() const;

private:
    IdType mId;
    double mX;
    double mY;
    double mZ;
};

class Element
{
public:
    using ElementType = std::size_t;
    using ConnectivitiesType = std::vector<std::size_t>;

    Element(
        const IdType I_Id,
        const ElementType I_Type,
        const ConnectivitiesType& I_Connectivities)
    : mId(I_Id),
      mType(I_Type),
      mConnectivities(I_Connectivities)
    { }

    IdType Id() const;
    ElementType Type() const;
    const ConnectivitiesType& Connectivities() const;

private:
    IdType mId;
    std::size_t mType;
    ConnectivitiesType mConnectivities;
};

class ModelPart
{
public:

    using NodesContainerType = std::vector<Node>;
    using ElementsContainerType = std::vector<Element>;

    std::size_t NumberOfNodes() const
    {
        return mNodes.size();
    }

    std::size_t NumberOfElements() const
    {
        return mElements.size();
    }

    Node& CreateNewNode(
        const IdType I_Id,
        const double I_X,
        const double I_Y,
        const double I_Z);

    Element& CreateNewElement(
        const IdType I_Id,
        const Element::ElementType I_Type,
        const Element::ConnectivitiesType& I_Connectivities);

    NodesContainerType::const_iterator NodesBegin() const;
    ElementsContainerType::const_iterator ElementsBegin() const;

    NodesContainerType::const_iterator NodesEnd() const;
    ElementsContainerType::const_iterator ElementsEnd() const;

    const Node& GetNode(const IdType I_Id) const;
    const Element& GetElement(const IdType I_Id) const;

private:
    NodesContainerType mNodes;
    ElementsContainerType mElements;
};

} //namespace CoSimIO

#endif // CO_SIM_IO_MODEL_PART_H_INCLUDED
