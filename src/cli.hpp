//
//  cli.hpp
//  Animera
//
//  Created by Indi Kernick on 23/11/19.
//  Copyright © 2019 Indi Kernick. All rights reserved.
//

#ifndef cli_hpp
#define cli_hpp

#include "docopt.h"
#include "error.hpp"

class CLI {
public:
  CLI(int &, char **);

  int exec();

private:
  int &argc;
  char **argv;
  
  Error parseArgs(std::map<std::string, docopt::value> &) const;
  int execDefault(const std::map<std::string, docopt::value> &) const;
  int execNew(const std::map<std::string, docopt::value> &) const;
  int execOpen(const std::map<std::string, docopt::value> &) const;
  int execExport(const std::map<std::string, docopt::value> &) const;
};

#endif