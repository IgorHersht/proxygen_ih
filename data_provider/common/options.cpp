
#include <boost/property_tree/json_parser.hpp>
#include <stdexcept>
#include <fstream>
#include "options.h"

namespace common
{

boost::program_options::variables_map Options::getCmdLine(int argc, char *argv[]) const
{
    using namespace boost::program_options;
    options_description allDescription = getCmdLineDescription();
    variables_map vm;
    store(parse_command_line(argc, argv, allDescription), vm);
    return vm;
}

boost::program_options::options_description Options::getCmdLineDescription() const
{
    using namespace boost::program_options;
    options_description config("Config files");
    config.add_options()
            ("config_default", value<std::string>(), "config_default file name")
            ("config_override", value<std::string>(), "config_override file name")
            ;

    boost::program_options::options_description extra_description;
    // add to extra_description here

    boost::program_options::options_description all_description;
    all_description.add(config).add(extra_description);
    return all_description;
}


// for now, only from config_default file assuming no duplicates
//TODO: deal with duplicats
// Remove dups from the same config file
// overrie and add from ( in order)  1.config_override file, 2. enviroment variables 3.cmd line
void Options::setProperties( const boost::program_options::variables_map& cmdLine)
{
    const std::string& config_default = cmdLine["config_default"].as<std::string>();
    if(config_default.empty()){
        return;
    }
    boost::property_tree::read_json(config_default, _properties);

}


}