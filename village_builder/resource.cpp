#include <unordered_map>
#include <string>

#include "resource.hpp"

ResourceType parseResourceType(std::string str) {
    // Table of strings to ResourceTypes
    std::unordered_map<std::string, ResourceType> table;
    table["Logs"] = ResourceType::Logs;
    table["Stone"] = ResourceType::Stone;
    table["Food"] = ResourceType::Food;
    table["Water"] = ResourceType::Water;

    return table[str];
}

void Resource::setType(ResourceType type_) {
    this->type = type_;
}

ResourceType Resource::getType() {
    return this->type;
}

int Resource::getCount() {
    return this->count;
}

void Resource::setCount(int count_) {
    this->count = count_;
}

bool Resource::changeCount(int count_) {
    // Note: this changes the count by count_ does not directly set!
    // For setting: change by negative value i.e. --  r.changeCount(-1 * r.getCount()) 
    
    int change = this->getCount() + count_;

    // Change is negative then requesting too much
    if(change < 0) {
        return false;
    }
    this->setCount(change);
    return true;
}
