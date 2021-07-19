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

// System includes
#include <algorithm>

// Project includes
#include "includes/macros.hpp"
#include "includes/model_part.hpp"
#include "includes/utilities.hpp"

namespace CoSimIO {

Node::Node(
    const IdType I_Id,
    const double I_X,
    const double I_Y,
    const double I_Z)
: mId(I_Id),
    mX(I_X),
    mY(I_Y),
    mZ(I_Z)
{
    CO_SIM_IO_ERROR_IF(I_Id < 1) << "Id must be >= 1!" << std::endl;
}

void Node::Print(std::ostream& rOStream) const
{
    rOStream << "CoSimIO-Node; Id: " << Id() << "\n";
    rOStream << "    Coordinates: [ " << X() << " | " << Y() << " | " << Z() << " ]" << std::endl;
}

void Node::save(CoSimIO::Internals::Serializer& rSerializer) const
{
    rSerializer.save("mId", mId);
    rSerializer.save("mX", mX);
    rSerializer.save("mY", mY);
    rSerializer.save("mZ", mZ);
}

void Node::load(CoSimIO::Internals::Serializer& rSerializer)
{
    rSerializer.load("mId", mId);
    rSerializer.load("mX", mX);
    rSerializer.load("mY", mY);
    rSerializer.load("mZ", mZ);
}

Element::Element(
    const IdType I_Id,
    const ElementType I_Type,
    const NodesContainerType& I_Nodes)
    : mId(I_Id),
        mType(I_Type),
        mNodes(I_Nodes)
{
    CO_SIM_IO_ERROR_IF(I_Id < 1) << "Id must be >= 1!" << std::endl;
    CO_SIM_IO_ERROR_IF(NumberOfNodes() < 1) << "No nodes were passed!" << std::endl;
    const int num_nodes_elem_type = CoSimIO::Internals::GetNumberOfNodesForElementType(I_Type);
    CO_SIM_IO_ERROR_IF_NOT(num_nodes_elem_type == static_cast<int>(NumberOfNodes())) << "Number of nodes (" << NumberOfNodes() << ") does not match expected number for element type (" << num_nodes_elem_type << ")!" << std::endl;
}


void Element::Print(std::ostream& rOStream) const
{
    rOStream << "CoSimIO-Element; Id: " << Id() << "\n";
    rOStream << "    Number of Nodes: " << NumberOfNodes() << "\n";
    rOStream << "    Node Ids: ";
    if (NumberOfNodes() > 0) {
        rOStream << mNodes[0]->Id();
    }
    for (std::size_t i=1; i<NumberOfNodes(); ++i) {
        rOStream << ", " << mNodes[i]->Id();
    }
    rOStream << std::endl;
}

void Element::save(CoSimIO::Internals::Serializer& rSerializer) const
{
    rSerializer.save("mId", mId);
    rSerializer.save("mType", static_cast<int>(mType));
    rSerializer.save("mNodes", mNodes);
}

void Element::load(CoSimIO::Internals::Serializer& rSerializer)
{
    rSerializer.load("mId", mId);
    int tmp;
    rSerializer.load("mType", tmp);
    mType = static_cast<ElementType>(tmp);
    rSerializer.load("mNodes", mNodes);
}


ModelPart::ModelPart(const std::string& I_Name) : mName(I_Name)
{
    CO_SIM_IO_ERROR_IF(I_Name.empty()) << "Please don't use empty names (\"\") when creating a ModelPart" << std::endl;
    CO_SIM_IO_ERROR_IF_NOT(I_Name.find(".") == std::string::npos) << "Please don't use names containing (\".\") when creating a ModelPart (used in \"" << I_Name << "\")" << std::endl;
}

Node& ModelPart::CreateNewNode(
    const IdType I_Id,
    const double I_X,
    const double I_Y,
    const double I_Z)
{
    CO_SIM_IO_ERROR_IF(HasNode(I_Id)) << "The Node with Id " << I_Id << " exists already!" << std::endl;

    mNodes.push_back(CoSimIO::make_intrusive<Node>(I_Id, I_X, I_Y, I_Z));
    return *(mNodes.back());
}

Element& ModelPart::CreateNewElement(
    const IdType I_Id,
    const ElementType I_Type,
    const ConnectivitiesType& I_Connectivities)
{
    CO_SIM_IO_ERROR_IF(HasElement(I_Id)) << "The Element with Id " << I_Id << " exists already!" << std::endl;

    Element::NodesContainerType nodes;
    nodes.reserve(I_Connectivities.size());
    for (const IdType node_id : I_Connectivities) {
        nodes.push_back(pGetNode(node_id));
    }
    mElements.push_back(CoSimIO::make_intrusive<Element>(I_Id, I_Type, nodes));
    return *(mElements.back());
}

Node& ModelPart::GetNode(const IdType I_Id)
{
    auto it_node = FindNode(I_Id);
    CO_SIM_IO_ERROR_IF(it_node == mNodes.end()) << "Node with Id " << I_Id << " does not exist!" << std::endl;
    return **it_node;
}

const Node& ModelPart::GetNode(const IdType I_Id) const
{
    auto it_node = FindNode(I_Id);
    CO_SIM_IO_ERROR_IF(it_node == mNodes.end()) << "Node with Id " << I_Id << " does not exist!" << std::endl;
    return **it_node;
}

ModelPart::NodePointerType ModelPart::pGetNode(const IdType I_Id)
{
    auto it_node = FindNode(I_Id);
    CO_SIM_IO_ERROR_IF(it_node == mNodes.end()) << "Node with Id " << I_Id << " does not exist!" << std::endl;
    return *it_node;
}

Element& ModelPart::GetElement(const IdType I_Id)
{
    auto it_elem = FindElement(I_Id);
    CO_SIM_IO_ERROR_IF(it_elem == mElements.end()) << "Element with Id " << I_Id << " does not exist!" << std::endl;
    return **it_elem;
}

const Element& ModelPart::GetElement(const IdType I_Id) const
{
    auto it_elem = FindElement(I_Id);
    CO_SIM_IO_ERROR_IF(it_elem == mElements.end()) << "Element with Id " << I_Id << " does not exist!" << std::endl;
    return **it_elem;
}

ModelPart::ElementPointerType ModelPart::pGetElement(const IdType I_Id)
{
    auto it_elem = FindElement(I_Id);
    CO_SIM_IO_ERROR_IF(it_elem == mElements.end()) << "Element with Id " << I_Id << " does not exist!" << std::endl;
    return *it_elem;
}

void ModelPart::Print(std::ostream& rOStream) const
{
    rOStream << "CoSimIO-ModelPart \"" << mName << "\"\n";
    rOStream << "    Number of Nodes: " << NumberOfNodes() << "\n";
    rOStream << "    Number of Elements: " << NumberOfElements() << std::endl;
}

void ModelPart::Clear()
{
    mElements.clear();
    mElements.shrink_to_fit();

    mNodes.clear();
    mNodes.shrink_to_fit();
}

ModelPart::NodesContainerType::const_iterator ModelPart::FindNode(const IdType I_Id) const
{
    return std::find_if(
        mNodes.begin(), mNodes.end(),
        [I_Id](const NodePointerType& rp_node) { return rp_node->Id() == I_Id;});
}

ModelPart::NodesContainerType::iterator ModelPart::FindNode(const IdType I_Id)
{
    return std::find_if(
        mNodes.begin(), mNodes.end(),
        [I_Id](const NodePointerType& rp_node) { return rp_node->Id() == I_Id;});
}

ModelPart::ElementsContainerType::const_iterator ModelPart::FindElement(const IdType I_Id) const
{
    return std::find_if(
        mElements.begin(), mElements.end(),
        [I_Id](const ElementPointerType& rp_elem) { return rp_elem->Id() == I_Id;});
}

ModelPart::ElementsContainerType::iterator ModelPart::FindElement(const IdType I_Id)
{
    return std::find_if(
        mElements.begin(), mElements.end(),
        [I_Id](const ElementPointerType& rp_elem) { return rp_elem->Id() == I_Id;});
}

bool ModelPart::HasNode(const IdType I_Id) const
{
    return FindNode(I_Id) != mNodes.end();
}

bool ModelPart::HasElement(const IdType I_Id) const
{
    return FindElement(I_Id) != mElements.end();
}

void ModelPart::save(CoSimIO::Internals::Serializer& rSerializer) const
{
    rSerializer.save("mName", mName);
    rSerializer.save("mNodes", mNodes);
    rSerializer.save("mElements", mElements);
}

void ModelPart::load(CoSimIO::Internals::Serializer& rSerializer)
{
    rSerializer.load("mName", mName);
    rSerializer.load("mNodes", mNodes);
    rSerializer.load("mElements", mElements);
}

} //namespace CoSimIO
