#include <string>

class date
{
private:
    int day;
    int month;
    int year;
    std::string output;

public:
    std::string getWholeDate();
    int getDay();
    int getMonth();
    int getYear();

    date();
    date(int d, int m, int y);
    ~date();
};

date::date()
{
    this->day = 0;
    this->month = 0;
    this->year = 0;
}

date::date(int d, int m, int y)
{
    this->day = d;
    this->month = m;
    this->year = y;
}

date::~date()
{
}

std::string date::getWholeDate()
{
    std::string dayNum = std::to_string(this->day);
    std::string monthNum = std::to_string(this->month);
    std::string yearNum = std::to_string(this->year);

    std::string returnString = "Day: " + dayNum + ", Month: " + monthNum + ", Year: " + yearNum + ".";

    return returnString;
}

int date::getDay()
{
    return this->day;
}

int date::getMonth()
{
    return this->month;
}

int date::getYear()
{
    return this->year;
}