#include <string>
#include<string.h>
#include <list>

#define CAT_ARR_SIZE 5

std::string categories[CAT_ARR_SIZE] = {"Immediate", "Urgent", "High Importance", "Normal Importance", "Low Importance"};

class category
{
public:
    std::string getCategoryString();
    std::string getDefaultCategory();
    std::string nextCategory(const char* str);


    category();
    ~category();

private:
    std::string picked__cat;
};

category::category()
{
    this->picked__cat = categories[4];
}

category::~category()
{
}

std::string category::getDefaultCategory(){
    return "Low Importance";
}

std::string category::getCategoryString()
{
    return picked__cat;
}

std::string category::nextCategory(const char* str){
    int catnum = 0;

    for (auto i = 0; i < CAT_ARR_SIZE - 1; i++)
    {
        if(strcmp(str, categories[i].c_str()) == 0){
            catnum = i;
        } 
    }
    
    if(catnum == CAT_ARR_SIZE -1){
        catnum = 0;
    }

    return categories[catnum + 1];
}
