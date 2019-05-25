#include <string>

#ifndef RESOURCE_HPP_
#define RESOURCE_HPP_

// Resource Types
enum class ResourceType {
    Logs,
    Food,
    Water,
    Stone
};

// Parse string to ResourceType
ResourceType parseResourceType(std::string);

// Resouce Class
class Resource {
    public:
        // Constructors
        Resource(ResourceType type_, int count_) : type(type_), count(count_) {} 
        
        // Getter and 'Setter' for count
        int getCount();
        bool changeCount(int count_); // 'Setter' only changes the value if it is valid (i.e. when trying to consume more than available will deny) 
    
        // Getter and Setter for type
        ResourceType getType();
        void setType(ResourceType type_);
   
    private:
        // Type of Resource
        ResourceType type;

        // Count of resources
        int count;

        // Setter for count, needs to be accessed through change count
        void setCount(int count_);
};

#endif
