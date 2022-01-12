#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include <map>
#include <string>
#include <memory>
#include "../Object/GameObject.h"

class ObjectFactory {

    public:
        GameObject* CreateObject(std::string type, Properties* props);
        // GameObject*(Properties* props) is the contructor, it is asking a function who returns GameObject contructor
        void RegisterType(std::string className, std::function<GameObject*(Properties* props)> type);
        static ObjectFactory* GetInstance(){return s_Instance = (s_Instance != nullptr)? s_Instance : new ObjectFactory();}

    private:
        ObjectFactory(){}
        static ObjectFactory* s_Instance;
        std::map<std::string, std::function<GameObject*(Properties* props)>> m_TypeRegistry;
};

template<class Type>
class Registrar {

    public:
        Registrar(std::string className){
            ObjectFactory::GetInstance()->RegisterType(className, [](Properties* props)->GameObject* { return new Type(props); });
        }
};

#endif // GAMEOBJECTFACTORY_H

