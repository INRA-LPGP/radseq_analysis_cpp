#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <string>
#include <vector>
#include <algorithm>
#include <map>

class ArgParser {

    public:

        ArgParser(int &argc, char **argv) {

            for (auto i=1; i < argc; ++i) this->fields.push_back(std::string(argv[i]));
        }

        const std::string get_value(const std::string& setting) const {

            std::vector<std::string>::const_iterator itr =  std::find(this->fields.begin(), this->fields.end(), setting);

            if (itr != this->fields.end() && ++itr != this->fields.end()) {

                return *itr;
            }

            return "";
        }

        bool contains(const std::string &option) const {

            return std::find(this->fields.begin(), this->fields.end(), option) != this->fields.end();
        }

        const std::string set_value(const std::string& field, const std::map<std::string, std::vector<std::string>>& options) {

            if (this->contains(field)) return this->get_value("-n");
            else  return options.at(std::string("-n"))[0];
        }

    private:

        std::vector<std::string> fields;
};

#endif // ARGPARSER_H
