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
Also it is used in order to be consistent with Kratos to reduce compatibility problems
This is a simplified version of Kratos::ModelPart
see https://github.com/KratosMultiphysics/Kratos/blob/master/kratos/includes/model_part.h
*/

// System includes
#include <vector>
#include <functional>

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
    : Node(I_Id, I_Coordinates[0], I_Coordinates[1], I_Coordinates[2])
    { }

    IdType Id() const { return mId; }
    double X() const { return mX; }
    double Y() const { return mY; }
    double Z() const { return mZ; }
    CoordinatesType Coordinates() const { return {mX, mY, mZ}; }

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
    using NodesContainerType = std::vector<std::reference_wrapper<Node>>;
    using ConnectivitiesType = std::vector<IdType>;

    Element(
        const IdType I_Id,
        const ElementType I_Type,
        const NodesContainerType& I_Nodes)
    : mId(I_Id),
      mType(I_Type),
      mNodes(I_Nodes)
    { }

    IdType Id() const { return mId; }
    ElementType Type() const { return mType; }
    std::size_t NumberOfNodes() const { return mNodes.size(); }
    NodesContainerType::const_iterator NodesBegin() const { return mNodes.begin(); }
    NodesContainerType::const_iterator NodesEnd() const { return mNodes.end(); }
    ConnectivitiesType Connectivities() const // TODO maybe rename to sth like NodeIds? Or do we need it at all?
    {
        ConnectivitiesType connectivities(mNodes.size());
        for (std::size_t i=i; i<mNodes.size(); ++i) {
            connectivities[i] = mNodes[i].get().Id();
        }
        return connectivities;
    }

private:
    IdType mId;
    ElementType mType;
    NodesContainerType mNodes;
};

class ModelPart
{
public:

    using NodesContainerType = std::vector<Node>;
    using ElementsContainerType = std::vector<Element>;

    std::size_t NumberOfNodes() const { return mNodes.size(); }
    std::size_t NumberOfElements() const { return mElements.size(); }

    Node& CreateNewNode(
        const IdType I_Id,
        const double I_X,
        const double I_Y,
        const double I_Z);

    Element& CreateNewElement(
        const IdType I_Id,
        const Element::ElementType I_Type,
        const Element::ConnectivitiesType& I_Connectivities);

    NodesContainerType::const_iterator NodesBegin() const { return mNodes.begin(); }
    ElementsContainerType::const_iterator ElementsBegin() const { return mElements.begin(); }

    NodesContainerType::const_iterator NodesEnd() const { return mNodes.end(); }
    ElementsContainerType::const_iterator ElementsEnd() const { return mElements.end(); }

    const Node& GetNode(const IdType I_Id) const;
    const Element& GetElement(const IdType I_Id) const;

private:
    NodesContainerType mNodes;
    ElementsContainerType mElements;
};

} //namespace CoSimIO

#endif // CO_SIM_IO_MODEL_PART_H_INCLUDED
