#include "ObjectFactory.h"
#include "../Characters/Enemy.h"
#include "../Characters/Warrior.h"
#include <iostream>

ObjectFactory* ObjectFactory::s_Instance = nullptr;

void ObjectFactory::RegisterType(std::string className, std::function<GameObject*(Properties* props)> type){
    m_TypeRegistry[className] = type;
}

GameObject* ObjectFactory::CreateObject(std::string type, Properties* props){

    GameObject* object = nullptr;
    // find the class by type from type registry map
    // it will give you back with type std::pair<X, Y>
    auto it = m_TypeRegistry.find(type);

    // first parameter is the type, the second parameter is the constructor
    if(it != m_TypeRegistry.end())
        object = it->second(props);

    return object;
}
