#include "date.cpp"
#include "category.cpp"

class activity
{
private:
    int id;
    date data;
    std::string content;
    bool completed;
    std::string _category;

public:
    // methods
    int getId();

    date getDate();
    void setDate(date d);

    std::string getContent();
    void setContent(std::string cont);

    std::string getEverything();

    std::string getCompletion();
    void changeCompletion();

    std::string getCategory();
    void setCategory();

    // consttructor & destructor
    activity();
    activity(int ID, date DATE, std::string CONTENT);
    activity(int ID, date DATE, std::string CONTENT, std::string cat);
    ~activity();
};

activity::activity()
{
}

activity::activity(int ID, date DATE, std::string CONTENT)
{
    this->id = ID;
    this->data = DATE;
    this->content = CONTENT;
    this->completed = false;
    category a = *new category();
    this->_category = a.getDefaultCategory();
}

activity::activity(int ID, date DATE, std::string CONTENT, std::string cat)
{
    this->id = ID;
    this->data = DATE;
    this->content = CONTENT;
    this->completed = false;
    this->_category = cat;
}

activity::~activity()
{
}

std::string activity::getContent()
{
    return this->content;
}

void activity::setContent(std::string cont)
{
    this->content = cont;
}

int activity::getId()
{
    return this->id;
}

date activity::getDate()
{
    return this->data;
}

void activity::setDate(date d)
{
    this->data = d;
}

std::string activity::getEverything()
{
    return "ID: " + std::to_string(this->getId()) + " | Date: " + this->getDate().getWholeDate() + " | Category: " + this->_category +
           " | Content: " + this->getContent() + " | Completed: " + this->getCompletion() + ".";
}

std::string activity::getCompletion()
{
    return this->completed ? "YES" : "NO";
}

void activity::changeCompletion()
{
    this->completed = !this->completed;
}

std::string activity::getCategory(){
    return this->_category;
}

void activity::setCategory(){

    category a = *new category();

    this->_category = a.nextCategory(this->_category.c_str());
}