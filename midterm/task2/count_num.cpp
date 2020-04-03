#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <map>
#include <bits/stdc++.h> 
using namespace std;

void clean_exit_on_sig(int sig_num)
{
	cout << "Segmentation fault :(\nDid you give both arguments?\n";
  exit(1);
}

int main(int argc, char *argv[]) {
	signal(11, clean_exit_on_sig);
  ifstream fin(argv[1]);
	vector<string> fin_lines;
	string line;
	int line_count;
	int lines_per_process;
	int lines_in_last_process;
	map<int, int> counts;
	int x;

  char* process_count_str = argv[2];
  int n = atoi(process_count_str); // n = process count

	while (fin >> line) {
		fin_lines.push_back(line);
	}
	// for (int i = 0; i < fin_lines.size(); i++) 
	// 	cout << fin_lines[i] << "\n"; 
	line_count = fin_lines.size();
	lines_per_process = line_count/n + (line_count % n != 0);
	lines_in_last_process = line_count % n;

	if (!lines_in_last_process)
		lines_in_last_process = lines_per_process;

	for(int i=0;i<n;i++)
	{ 
		if(fork() == 0) 
		{ 
			// open file
			// "pass" 
			// write to file "3.txt"
			printf("[son%d:] pid %d from [parent] pid %d\n",i, getpid(),getppid()); 
			exit(0); 
		}
	} 
	for(int i=0;i<n;i++){
		wait(NULL);
	}
		// loop will run n times (n=5) 
      
    // for(int i=0;i<5;i++){
      // open "i.txt" file
      // input all entries into map
    //}

    // output map into file.

	// while (fin >> x) {
	// 	if (counts.find(x) == counts.end()) counts[x] = 0;
	// 	counts[x]++;
	// }

	// for (auto &c: counts) {
	// 	cout << c.first << " " << c.second << endl;
	// }

	return 0;
}