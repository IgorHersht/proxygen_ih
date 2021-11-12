#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/program_options.hpp>
#include <boost/optional.hpp>
#include <string>
#include <memory>

namespace common
{

struct Options {
    void set(int argc, char *argv[] ){
        boost::program_options::variables_map cmdLine = getCmdLine(argc, argv);
        setProperties(cmdLine);
    }
    virtual void validate() const = 0;
// assumes no dups
    template <typename T> boost::optional<T> get(const std::string key, bool mandatory = true) const{
        using namespace boost::property_tree;
        boost::optional<T> optValue;
        auto child = _properties.get_child_optional(key);
        if(child){
            optValue = child->get_value<T>();
        }else if(mandatory){
            std::string msg("Configuration error. Cannot find value for key="); msg += key;
            throw std::runtime_error(msg);
        }
       return optValue;
    }
protected:
    virtual boost::program_options::variables_map getCmdLine(int argc, char *argv[]) const ;
    virtual boost::program_options::options_description getCmdLineDescription() const;
    virtual void setProperties( const boost::program_options::variables_map& cmdLine);
protected:
    boost::property_tree::ptree                 _properties;
};

}//namespace common