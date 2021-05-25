//
// Created by Alex Samsonov on 16/05/21.
//

#include "BasePanel.h"

void BasePanel::setParent(BasePanel* p)
{
    parent = p;
}

BasePanel* BasePanel::getParent() const
{
    return parent;
}