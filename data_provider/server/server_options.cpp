
#include "server_options.h"


namespace network {
namespace server {

void ServerOptions::validate() const
{
//    const auto io_threadsOpt = get<int>("net.server.io_threads");
//    if(!io_threadsOpt ){
//        int i =1;
//    }
//    if( !io_threadsOpt || (*io_threadsOpt <= 0) ) {
//        throw std::runtime_error("Invalid IOContextPool size ");
//    }
//    const auto addressOpt = get<std::string>("net.server.address");
//    if( !addressOpt || (*addressOpt).empty() ) {
//        throw std::runtime_error("Invalid server address ");
//    }
//    const auto portOpt = get<int>("net.server.port");
//    if( !portOpt || (*portOpt<= 0) ) {
//        throw std::runtime_error("Invalid server port ");
//    }
}

boost::program_options::options_description ServerOptions::getCmdLineDescription() const
{
    boost::program_options::options_description supper_description = Supper::getCmdLineDescription();
    boost::program_options::options_description this_description;
    // add to this_description here
    boost::program_options::options_description all_description;
    all_description.add(supper_description).add(this_description);
    return all_description;
}
void ServerOptions::setProperties( const boost::program_options::variables_map& cmdLine)
{
    Supper::setProperties(cmdLine);
}


}}