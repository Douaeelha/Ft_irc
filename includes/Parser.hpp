<<<<<<< HEAD
#pragma once

#include<string>
#include<vector>

class Parser{

    private:
        std::string trimTrailingCRLF(const std::string &line) const; // trim the \r\n from the end of the line
    
    public:
        struct Command
        {
            std::string name;
            std::vector<std::string> params;
        };

        Parser();
        ~Parser();

        Command parse(const std::string &line) const;
};
=======
#pragma once 
#include <vector>
#include <string>
class ParsedMessage {
public:
    std::string command;
    std::vector<std::string> params;
    std::string trailing;
};  
>>>>>>> main
