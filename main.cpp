#include <iostream>
#include "activity.cpp"
#include "ncurses.h"
#include "vector"
#include <stdlib.h>
#include <fstream>


using namespace std;

vector<activity> loadVectorFromFile(const std::string& filename);
void saveVectorToFile(const std::vector<activity>& vec, const std::string& filename);
void refreshScreen(WINDOW *a, WINDOW *b);
void editActivity(activity *a);
void redrawEdit(WINDOW *edit_window, WINDOW *edit_tools, activity *a);

int terminal_width, terminal_height;

int main()
{
	vector<activity> activities_pool;

	for (int i = 0; i < 15; i++)
	{
		category a = *new category();
		activities_pool.push_back(*new activity(i, *new date(69, 69, 69), "AAAAAAAAA: " + to_string(i), a.getDefaultCategory()));
	}

	// init ncurses
	initscr();
	noecho();
	keypad(stdscr, TRUE);
	start_color();

	// terminal dims
	terminal_width = getmaxx(stdscr);
	terminal_height = getmaxy(stdscr);
	int ch;

	int select_pos = 2;

	WINDOW *main_window = newwin(terminal_height - 1, terminal_width - 1, 0, 0);
	WINDOW *tools_window = newwin(5, terminal_width - 3, terminal_height - 7, 1);

	refreshScreen(main_window, tools_window);

	//int for F(4)
	int highestID = 0;
	do
	{

		switch (ch)
		{

		//edit
		case KEY_F(2):
			editActivity(&activities_pool[select_pos - 2]);
			wclear(main_window);
			refreshScreen(main_window, tools_window);
			break;

		//remove
		case KEY_F(3):
			activities_pool.erase(activities_pool.begin() + (select_pos-2));
			if(select_pos - 2 >= activities_pool.size()){
				select_pos--;
			}
			wclear(main_window);
			refreshScreen(main_window, tools_window);
			break;

		//add
		case KEY_F(4):
			highestID = 0;

			for (int i = 0; i < activities_pool.size(); i++)
			{
				activity a = activities_pool.at(i);
				if (a.getId() > highestID)
				{
					highestID = a.getId();
				}
			}
			
			activities_pool.insert(activities_pool.begin() + activities_pool.size(), *new activity(highestID + 1, *new date(0, 0, 0), "", "Low Importance"));
			editActivity(&activities_pool.at(activities_pool.size() -1));
			break;
		//save
		case KEY_F(5):
			saveVectorToFile(activities_pool, "save.bin");
			break;
		//load
		case KEY_F(6):
			activities_pool = loadVectorFromFile("save.bin");
			break;

		case KEY_DOWN:
			if(select_pos < activities_pool.size() + 1){
				mvwprintw(main_window, select_pos, 2, " ");
				select_pos += 1;
			}
			break;

		case KEY_UP:
			if (select_pos != 2)
			{
				mvwprintw(main_window, select_pos, 2, " ");
				select_pos -= 1;
			}
		default:
			break;
		}

		mvwprintw(main_window, 1, 5, "-ID-");
		mvwprintw(main_window, 1, 12, "-DATE-");
		mvwprintw(main_window, 1, 47, "-CATEGORY-");
		mvwprintw(main_window, 1, 82, "-CONTENT-");
		mvwprintw(main_window, 1, 132, "-COMPLETED-");

		for (int i = 0; i < activities_pool.size(); i++)
		{
			int offset = 5, start_height = i + 2;
			activity b = activities_pool[i];
			// mvwprintw(main_window, i + 1, 2, b.getEverything().c_str());
			mvwprintw(main_window, start_height, offset, to_string(b.getId()).c_str());
			offset += 7;
			mvwprintw(main_window, start_height, offset, b.getDate().getWholeDate().c_str());
			offset += 35;
			mvwprintw(main_window, start_height, offset, b.getCategory().c_str());
			offset += 35;
			mvwprintw(main_window, start_height, offset, b.getContent().c_str());
			offset += 50;
			mvwprintw(main_window, start_height, offset, b.getCompletion().c_str());
		}



		mvwprintw(main_window, select_pos, 2, "*");

		refreshScreen(main_window, tools_window);
	}while ((ch = getch()) != KEY_F(1));

	endwin();
	return 0;
}

void refreshScreen(WINDOW *a, WINDOW *b)
{

	box(a, 0, 0);
	wborder(b, '|', '|', '-', '-', '+', '+', '+', '+');
	mvwprintw(b, 2, 2, "     F1 - Quit     F2 - Edit     F3 - Remove Activity     F4 - Add Activity     F5 - Save     F6 - Reload (read file)");
	refresh();
	wrefresh(a);
	wrefresh(b);
}

void redrawEdit(WINDOW *edit_window, WINDOW *edit_tools, activity *a){
	edit_window = newwin(15, 60, (terminal_height / 3), (terminal_width / 2) - 30);
	edit_tools = newwin(3, 120, (terminal_height / 3) + 15, (terminal_width / 2) - 60);
	box(edit_window, 0, 0);
	box(edit_tools, 0, 0);

	string loc_id = "ID: " + to_string(a->getId());
	mvwprintw(edit_window, 1, 1, loc_id.c_str());
	mvwprintw(edit_window, 2, 1, a->getDate().getWholeDate().c_str());
	string loc_content = "Content: " + a->getContent();
	mvwprintw(edit_window, 3, 1, loc_content.c_str());
	string loc_category = "Category: " + a->getCategory();
	mvwprintw(edit_window, 4, 1, loc_category.c_str());
	string loc_completion = "Completed: " + a->getCompletion();
	mvwprintw(edit_window, 5, 1, loc_completion.c_str());
	mvwprintw(edit_tools, 1, 1, "F1 - Close window     F2 - Change date     F3 - Change Content     F4 - Change Importance     F5 - Change Completion");
	//wmove(edit_window, 0, 0);
	refresh();
	wrefresh(edit_window);
	wrefresh(edit_tools);

}

void editActivity(activity *a){
	WINDOW *edit_window = newwin(15, 60, (terminal_height / 3), (terminal_width / 2) - 30);
	WINDOW *edit_tools = newwin(3, 120, (terminal_height / 3) + 15, (terminal_width / 2) - 60);
	redrawEdit(edit_window, edit_tools, a);
	int ch, term_width = terminal_width, term_height = terminal_height;
	while ((ch = getch()) != KEY_F(1))
	{
			if(ch == KEY_F(2)){ 
				WINDOW *date_edit = newwin(15, 60, (term_height / 3), (term_width / 2) - 30);
				WINDOW *date_tools = newwin(3, 120, (term_height / 3) + 15, (term_width / 2) - 60);
				box(date_edit, 0, 0);
				box(date_tools, 0, 0);
				int date_day, date_month, date_year;
				mvwprintw(date_edit, 1, 1, "Please Input Day Number:");
				wmove(date_edit, 2, 1);
				refresh();
				wrefresh(date_edit);
				do{
					int ch3;
					string input;
					while ((ch3 = getch()) != '\n')
					{
						if(isdigit(ch3)){
							input.push_back(ch3);
							mvwprintw(date_edit, 2, 1, input.c_str());
							wrefresh(date_edit);
						}
					}
					date_day = stoi(input.c_str());

					if(date_day < 1 || date_day > 31){
						int ch2;
						mvwprintw(date_edit, 2, 1, "Wrong Number! Press F1 to retry.");
						wrefresh(date_edit);
						while ((ch2 = getch()) != KEY_F(1)){}
						mvwprintw(date_edit, 2, 1, "							");
						wmove(date_edit, 2, 1);
						wrefresh(date_edit);
					}
				}while(date_day < 1 || date_day > 31);



				mvwprintw(date_edit, 3, 1, "Please Input Month Number:");
				wmove(date_edit, 4, 1);
				refresh();
				wrefresh(date_edit);
				do{
					int ch3;
					string input;
					while ((ch3 = getch()) != '\n')
					{
						if (isdigit(ch3))
						{
							input.push_back(ch3);
							mvwprintw(date_edit, 4, 1, input.c_str());
							wrefresh(date_edit);
						}
					}
					date_month = stoi(input.c_str());

					if (date_month < 1 || date_month > 12)
					{
						int ch2;
						mvwprintw(date_edit, 4, 1, "Wrong Number! Press F1 to retry.");
						wrefresh(date_edit);
						while ((ch2 = getch()) != KEY_F(1)){}
						mvwprintw(date_edit, 4, 1, "							");
						wmove(date_edit, 4, 1);
						wrefresh(date_edit);
					}
				}while(date_month < 1 || date_month > 12);



				mvwprintw(date_edit, 5, 1, "Please Input Year Number:");
				wmove(date_edit, 6, 1);
				refresh();
				wrefresh(date_edit);
				do{
					int ch3;
					string input;
					while ((ch3 = getch()) != '\n')
					{
						if (isdigit(ch3))
						{
							input.push_back(ch3);
							mvwprintw(date_edit, 6, 1, input.c_str());
							wrefresh(date_edit);
						}
					}
					date_year = stoi(input.c_str());


					if (date_year < 2024 || date_year > 2050)
					{
						int ch2;
						mvwprintw(date_edit, 6, 1, "Wrong Number! Press F1 to retry.");
						wrefresh(date_edit);
						while ((ch2 = getch()) != KEY_F(1)){}
						mvwprintw(date_edit, 6, 1, "							");
						wmove(date_edit, 6, 1);
						wrefresh(date_edit);
					}
				}while (date_year < 2024 || date_year > 2050);

				a->setDate(*new date(date_day, date_month, date_year));

				wclear(date_edit);
				wclear(date_tools);
				refresh();
				wrefresh(date_edit);
				wrefresh(date_tools);
				}


			if(ch == KEY_F(3)){
				int ch_loc;
				WINDOW *content_edit = newwin(3, 60, (term_height / 2) - 2, (term_width / 2) - 30);
				box(content_edit, 0, 0);
				wmove(content_edit, 1, 1);
				string input = a->getContent();
				mvwprintw(content_edit, 1, 1, input.c_str());
				wrefresh(content_edit);
				while ((ch_loc = getch()) != '\n')
				{
					if(ch_loc == KEY_BACKSPACE && input.size() > 0){
						input = input.substr(0, input.size()-1);
						wclear(content_edit);
						box(content_edit, 0, 0);
						mvwprintw(content_edit, 1, 1, input.c_str());
						wrefresh(content_edit);
					}
					else 
					{
						input.push_back(ch_loc);
						mvwprintw(content_edit, 1, 1, input.c_str());
						wrefresh(content_edit);
					}
				}
				a->setContent(input);
				redrawEdit(edit_window, edit_tools, a);
			}

			if(ch == KEY_F(4)){
				a->setCategory();
				redrawEdit(edit_window, edit_tools, a);
			}

			if (ch == KEY_F(5))
			{
				a->changeCompletion();
				redrawEdit(edit_window, edit_tools, a);
			}

	redrawEdit(edit_window, edit_tools, a);
	}
}

void saveVectorToFile(const std::vector<activity>& vec, const std::string& filename) {
	ofstream outFile(filename, ios::binary);
	
	
}


vector<activity> loadVectorFromFile(const std::string& filename){

}

