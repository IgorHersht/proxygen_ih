#pragma once

#include <common/options.h>


namespace network {
namespace server {

struct ServerOptions : public common::Options
{
    void validate() const override;
protected:
    typedef common::Options Supper;
    boost::program_options::options_description getCmdLineDescription() const override;
    void setProperties( const boost::program_options::variables_map& cmdLine) override;
};


}}