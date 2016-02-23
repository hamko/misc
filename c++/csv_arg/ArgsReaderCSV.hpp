#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <map>
#include <cstdio> 

class ArgsReaderCSV {
  std::map<std::string, std::map<std::string, std::string> > m_args;

public:
  ArgsReaderCSV(const std::string args_csv)
  {
    std::ifstream fin(args_csv);
    if (!fin) {
      std::cerr << "No such file: " << args_csv << endl;
      exit(1);
    }
    std::string line;
    int row = 0;
    std::vector<std::string> fields;
    while (std::getline(fin, line)) {
      std::string token;
      std::istringstream stream(line);
      int col = 0;
      std::string key("");
      while (std::getline(stream, token, ',')) {
        // first row
        if (row == 0) {
          fields.push_back(token);
        } else {
          if (col == 0) // first col
            key = token;
          else 
            m_args[key][fields[col]] = token;
        }
        col++;
      }
      row++;
    }
  }

  void add(string key, string field_name, string value) {
    m_args[key][field_name] = value;
  }

  void save(string filename) {
  }

  std::string getArg(std::string key, std::string field_name) { return m_args[key][field_name]; } // TODO assert, if key is not found?

  void printArgs(void)
  {
    for (auto iti = m_args.begin(); iti != m_args.end(); iti++) {
      for (auto itj = iti->second.begin(); itj != iti->second.end(); itj++) {
        std::cout << itj->first << " " << itj->second << std::endl;
      }
      std::cout << std::endl;
    }
  }


};
