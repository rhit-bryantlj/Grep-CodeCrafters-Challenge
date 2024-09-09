#include <iostream>
#include <string>
#include <algorithm>

bool positive_pattern(const std::string& input_line, const std::string& pattern){
    return input_line.find_first_of(pattern) != std::string::npos;
}

bool alpha_num(const std::string& input_line, const std::string& pattern){
    for(int i = 0; i < input_line.length(); i++){
            if(isalnum(input_line[i])){
                return true;
            }   
        }
        return false;
}

bool digit_check(const std::string& input_line, const std::string& pattern){
    return input_line.find_first_of("0123456789") != std::string::npos;
}

bool match_pattern(const std::string& input_line, const std::string& pattern) {
    if(pattern.size() == 0) // pattern ended, line not ended, match found
        return 1;
    if(input_line.length() == 0) // line ended, match not found for pattern
        return 0;
    if (pattern.length() == 1) { // 
        return input_line.find(pattern) != std::string::npos;
    } else if(pattern[0] == '.'){
        return match_pattern(input_line.substr(1), pattern.substr(1));
    }else if(pattern[1] == '+') {
        if(pattern[0] != input_line[0])
            return 0;
        int count = 1;
        while(pattern[0] == input_line[count]){
            count++;
        }
        return match_pattern(input_line.substr(count), pattern.substr(2));
    } else if(pattern.substr(0,2) == "\\d"){
        if(isdigit(input_line[0])){
            return match_pattern(input_line.substr(1), pattern.substr(2));
        }
        return match_pattern(input_line.substr(1), pattern);
    } else if(pattern.substr(0,2) == "\\w"){
        // alpha_num(input_line, pattern);
        if(isalnum(input_line[0]))
            return match_pattern(input_line.substr(1), pattern.substr(2));
        return match_pattern(input_line.substr(1), pattern);
    } else if (pattern[0] == '['){
        int end = pattern.find(']');
        if(pattern[1] == '^'){
            if(!positive_pattern(input_line, pattern.substr(2,end-1))){
                return match_pattern(input_line.substr(1), pattern.substr(end+1));
            }
            return 0;
        } else{
            if(positive_pattern(input_line, pattern.substr(1,end-1))){
                return match_pattern(input_line.substr(1), pattern.substr(end+1));
            }
            return 0;
        }
    } else if (pattern.length() > 1 && pattern[1] == '?'){
        if(pattern[0] == input_line[0]){
            return match_pattern(input_line.substr(1), pattern.substr(2)) || match_pattern(input_line, pattern.substr(2));
        }
        return match_pattern(input_line, pattern.substr(2));
    } 

    if(pattern[0] == input_line[0])
        return match_pattern(input_line.substr(1), pattern.substr(1));

    return 0;
}

bool match_full(std::string input_line, std::string pattern){
    if(pattern[0] == '^')
        return match_pattern(input_line, pattern.substr(1));
    if(pattern[pattern.length()-1] == '$'){
        std::reverse(input_line.begin(), input_line.end());
        std::reverse(pattern.begin(), pattern.end());
        return match_pattern(input_line, pattern.substr(1));
    }
    int found = pattern.find('|');
    if(found != std::string::npos){
        int start = pattern.find('(');
        int end = pattern.find(')');
        std::string pattern1 = pattern.substr(0,start) + pattern.substr(start+1, found-start-1) + pattern.substr(end+1);
        std::string pattern2 = pattern.substr(0,start) + pattern.substr(found+1,end-found-1) + pattern.substr(end+1);
        return match_full(input_line, pattern1) || match_full(input_line, pattern2);
    }
    do{
        if(match_pattern(input_line, pattern)){
            return 1;
        }
        input_line = input_line.substr(1); // go to next char on the input
    } while(input_line!="");
    return 0;
}

int main(int argc, char* argv[]) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc != 3) {
        std::cerr << "Expected two arguments" << std::endl;
        return 1;
    }

    std::string flag = argv[1];
    std::string pattern = argv[2];

    if (flag != "-E") {
        std::cerr << "Expected first argument to be '-E'" << std::endl;
        return 1;
    }

    std::string input_line;
    std::getline(std::cin, input_line);
    
    try {
        if (match_full(input_line, pattern)) {
            return 0;
        } else {
            return 1;
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
