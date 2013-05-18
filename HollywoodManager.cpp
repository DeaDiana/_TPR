#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#define NUM_OF_ACTORS 100
#define NUM_OF_EPISODES 100
#define K_CONST 1
#define CHECK_STOP 2

using namespace std;

void calc_intervals(int involved_actors_in_episodes[NUM_OF_ACTORS][NUM_OF_EPISODES], vector<int> schedule, int intervals[NUM_OF_ACTORS][2])
{
	for(int i = 0; i < NUM_OF_ACTORS; i++)
	{
		for(int j = 0; j < NUM_OF_EPISODES; j++)
		{
			if(0 != involved_actors_in_episodes[i][ schedule[j] ]) 
			{
				intervals[i][0] = j;//schedule[j];
				break;
			}
		}
		for(int j = NUM_OF_EPISODES - 1; j >= 0; j--)
		{
			if(0 != involved_actors_in_episodes[i][ schedule[j] ]) 
			{
				intervals[i][1] = j;//schedule[j];
				//if(intervals[i][0] > intervals[i][1]) { std::swap(intervals[i][0], intervals[i][1]); }
				break;
			}
		}
	}
}

long function_value_F(int salaries[], int involved_actors_in_episodes[NUM_OF_ACTORS][NUM_OF_EPISODES], int intervals[NUM_OF_ACTORS][2])
{
	long budget = 0;

	for(int i = 0; i < NUM_OF_ACTORS; i++)
	{
		budget += (intervals[i][1] - intervals[i][0] + 1) * salaries[i];
	}

	return budget;
}

struct TaskInfo 
{
	int number_of_actors;
	int number_of_episods;
	int salaries[3];
	int involed_actors_in_episods[3][3];
	void fillData(string filename);
};

void TaskInfo::fillData(string filename)
{
	ifstream input_for_task;
	input_for_task.open(filename);
	input_for_task >> number_of_actors;
	input_for_task >> number_of_episods;
	for(int i = 0; i < number_of_actors; i++) 
	{
		input_for_task >> salaries[i];
	}
	for(int i = 0; i < number_of_actors; i++) 
	{
		for(int j = 0; j < number_of_episods; j++)
		{
			input_for_task >> involed_actors_in_episods[i][j];
		}
	}
}

int main()
{
	int number_of_actors = 0;
	int number_of_episods = 0;
	vector<int> C_schedule;
	ifstream input_for_task;

	input_for_task.open("big_input.txt");
	input_for_task >> number_of_actors;
	input_for_task >> number_of_episods;

	int salaries[NUM_OF_ACTORS] = {0};
	int involed_actors_in_episods[NUM_OF_ACTORS][NUM_OF_EPISODES] = {0};
	int intervals[NUM_OF_ACTORS][2] = {0};

	for(int i = 0; i < NUM_OF_ACTORS; i++) 
	{
		input_for_task >> salaries[i];
		C_schedule.push_back(i);
	}
	for(int i = 0; i < NUM_OF_ACTORS; i++) 
	{
		for(int j = 0; j < NUM_OF_EPISODES; j++)
		{
			input_for_task >> involed_actors_in_episods[i][j];
		}
	}
	int non_progress_counter = 0;
	long result_budget = 0;
	//start loop
	while(1)
	{
		calc_intervals(involed_actors_in_episods, C_schedule, intervals);
		long C_budget = function_value_F(salaries, involed_actors_in_episods, intervals);
		vector<vector<int>> C_schedule_envir;
		for(int k = 1; k <= K_CONST; k++)
		{
			// get C environment
			for(int i = 0; i < NUM_OF_EPISODES; i++)
			{
				for(int j = i + 1; j < NUM_OF_EPISODES; j++)
				{
					C_schedule_envir.push_back(C_schedule);
					std::swap(C_schedule_envir[C_schedule_envir.size() - 1][i], C_schedule_envir[C_schedule_envir.size() - 1][j]);
					//std::swap(C_schedule_envir.pop_back[i], C_schedule_envir.pop_back()[j]);
				
				}
			}
			// get C' (random schedule from C-environmet)
			srand(time(NULL));
			int random_index = rand() % C_schedule_envir.size();
			vector<int> C1_schedule = C_schedule_envir[random_index];		
			vector<vector<int>> C1_schedule_env;
			// get C''
			while(1) 
			{
				calc_intervals(involed_actors_in_episods, C1_schedule, intervals);
				long C1_budget_value = function_value_F(salaries, involed_actors_in_episods, intervals);
				// get C' environment
				for(int i = 0; i < NUM_OF_EPISODES; i++)
				{
					for(int j = i + 1; j < NUM_OF_EPISODES; j++)
					{
						C1_schedule_env.push_back(C1_schedule);
						std::swap(C1_schedule_env[C1_schedule_env.size() - 1][i], C1_schedule_env[C1_schedule_env.size() - 1][j]);
						//std::swap(C1_schedule_env.pop_back[i], C1_schedule_env.pop_back[j]);
					}
				}
				// начинается локальный спуск
				calc_intervals(involed_actors_in_episods, C1_schedule_env[0], intervals);
				long min_neighbour = function_value_F(salaries, involed_actors_in_episods, intervals);
				int min_index = 0;
				//find best neighbour
				for(int i = 1; i < C1_schedule_env.size(); i++)
				{
					calc_intervals(involed_actors_in_episods, C1_schedule_env[i], intervals);
					long C1_budget = function_value_F(salaries, involed_actors_in_episods, intervals);
					if(min_neighbour > C1_budget)
					{
						min_neighbour = C1_budget;
						min_index = i;
					}
				}
				if(min_neighbour < C1_budget_value)
				{
					C1_schedule = C1_schedule_env[min_index];
					C1_schedule_env.clear();
					std::cout<< min_neighbour << std::endl;
				} else {
					break;
				}
			}
			vector<int> C2_schedule = C1_schedule;
			calc_intervals(involed_actors_in_episods, C2_schedule, intervals);
			long C2_budget = function_value_F(salaries, involed_actors_in_episods, intervals);
		
			if(C2_schedule == C_schedule) 
			{
				non_progress_counter++;
			}
			if(C2_budget < C_budget)			
			{
				C_schedule = C2_schedule;
				result_budget = C2_budget;
				break;
			}
		}
		if(CHECK_STOP == non_progress_counter)
		{
			break;
		}
	}

	std::cout<<"result: " << result_budget;

return 0;
}