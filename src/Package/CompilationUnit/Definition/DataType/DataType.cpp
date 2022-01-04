#include "DataType.h"

DataType::DataType(shared_ptr<TokenIdentifier> identifier)
{
    kind = DATA_TYPE_IDENTIFIER;
    this->identifier = identifier;
}

DataType::DataType(shared_ptr<TokenKeyword> primitive_type)
{
    kind = DATA_TYPE_PRIMITIVE;
    this->primitive_type = primitive_type;
}

size_t DataType::getArrayDimensions()
{
    return arrayDimensions;
}

void DataType::setArrayDimensions(size_t arrayDimensions)
{
    this->arrayDimensions = arrayDimensions;
}

size_t DataType::addArrayDimension()
{
    return ++arrayDimensions;
}

DataTypeKind DataType::getKind()
{
    return kind;
}

shared_ptr<TokenIdentifier> DataType::getIdentifier()
{
    return
        (getKind() == DATA_TYPE_IDENTIFIER) ? identifier : nullptr;
}

shared_ptr<TokenKeyword> DataType::getPrimitiveType()
{
    return
        (getKind() == DATA_TYPE_PRIMITIVE) ? primitive_type : nullptr;
}

string DataType::toString()
{
    string typeName;

    switch (kind)
    {
        case DATA_TYPE_IDENTIFIER:
            typeName = identifier->getName();
            break;

        case DATA_TYPE_PRIMITIVE:
            typeName = primitive_type->getPrintableKindString();
            break;

        default:
            typeName = "(null)";
            break;
    }

    size_t dim = arrayDimensions;
    while (dim > 0)
    {
        typeName += "[]";
        dim--;
    }

    return typeName;
}