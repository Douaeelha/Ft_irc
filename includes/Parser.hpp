#pragma once

#include<string>
#include<vector>

class Parser{

    private:
        std::string trimTrailingCRLF(const std::string &line) const; // trim the \r\n from the end of the line
        std::vector<std::string> splitParams(const std::string &line) const;
    
    public:
        struct Command
        {
            std::string name:
            std::vector<std::string> params;
        }

        Parser();
        ~Parser();

        command parse(const std::string &line) const;
};