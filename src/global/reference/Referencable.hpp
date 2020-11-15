#ifndef REFERENCABLE_HPP
#define REFERENCABLE_HPP

#include <QtCore/QList>

#include "src/global/reference/OptionalReference.hpp"
#include "src/global/reference/Reference.hpp"
#include "src/defines.hpp"

class Referencable
{
    private:
        QList<Reference<Referencable>*> references;
        QList<OptionalReference<Referencable>*> optionalReferences;

    public:
        virtual ~Referencable();

        template<typename T>
        Reference<T> getReference();
        template<typename T>
        OptionalReference<T> getOptionalReference();
};

#endif // REFERENCABLE_HPP
