#include <iostream>
#include <string>
#include <map>

using MM = std::map<std::string, std::string>;
MM m1
        {
                {"1" , "v11"},
                {"2", "v12"},
                {"3", "v13"},
                {"4", "v14"}
        };

MM m1c= m1;

MM m2
        {
                {"1" , "v11"},
                {"2", "v22"},
                {"5", "v23"}
        };
MM m2c= m2;
MM merge( MM&& o, MM&& n )
{
    o.merge(n);
    for(const auto&[key, newValue ]: n){
        const std::string& oldValue = o.find(key)->second; // merge usures that iterator is alwaus valid
        if(newValue == oldValue){
            std::cout <<"Debug: ignoring identical risk parameter " << key << "->" << newValue << std::endl;
        }else{
            std::cout <<"Warn: ignoring conflicting risk parameter upadate " << key << "->" << newValue <<". Using previous parameter " << key << "->" << oldValue <<  std::endl;
        }
    }
    return o;

}

MM merge2( MM&& o, MM&& n )
{

    for(auto&[key, newValue ]: n){
        const auto& [oldIter,inserted]  = o.emplace(key,std::move(newValue));
        if(!inserted){
            const std::string& oldValue = oldIter->second;
            if(newValue == oldValue){
                std::cout <<"Debug: ignoring identical risk parameter " << key << "->" << newValue << std::endl;
            }else{
                std::cout <<"Warn: ignoring conflicting risk parameter upadate " << key << "->" << newValue <<". Using previous parameter " << key << "->" << oldValue <<  std::endl;
            }
        }


    }
    return o;

}

int main()
{
    MM m = merge(std::move(m1), std::move(m2));
    MM mc = merge(std::move(m1c), std::move(m2c));
    int i =0;


    return 0;
}
/*
Debug: ignoring identical risk parameter 1->v11
Warn: ignoring conflicting risk parameter upadate 2->v22. Using previous parameter 2->v12
Debug: ignoring identical risk parameter 1->v11
Warn: ignoring conflicting risk parameter upadate 2->v22. Using previous parameter 2->v12
 */
