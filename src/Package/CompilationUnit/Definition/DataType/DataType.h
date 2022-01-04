#pragma once
#include <memory>
using std::shared_ptr;

#include "../../../../Token/TokenIdentifier.h"
#include "../../../../Token/TokenKeyword.h"

enum DataTypeKind
{
    DATA_TYPE_PRIMITIVE,
    DATA_TYPE_IDENTIFIER
};

class DataType
{
    private:
        DataTypeKind kind;
        size_t arrayDimensions = 0;

        shared_ptr<TokenIdentifier> identifier = nullptr;
        shared_ptr<TokenKeyword> primitive_type = nullptr;

    public:
        DataType(shared_ptr<TokenIdentifier> identifier);
        DataType(shared_ptr<TokenKeyword> primitive_type);

        size_t getArrayDimensions();
        void setArrayDimensions(size_t arrayDimensions);
        size_t addArrayDimension();

        DataTypeKind getKind();
        shared_ptr<TokenIdentifier> getIdentifier();
        shared_ptr<TokenKeyword> getPrimitiveType();

        string toString();
};