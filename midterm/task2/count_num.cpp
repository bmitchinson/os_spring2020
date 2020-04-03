#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <map>
#include <bits/stdc++.h> 
#include <string>
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
	lines_in_last_process = line_count % lines_per_process;

	if (!lines_in_last_process)
		lines_in_last_process = lines_per_process;

	printf("line count: %d\n", line_count);
	printf("lines per process: %d\n", lines_per_process);
	printf("lines in last: %d\n", lines_in_last_process);


	for(int i=0;i<n;i++)
	{ 
		if(fork() == 0) 
		{ 
			string out_name = to_string(i) + ".txt";
			ofstream out_file;
			out_file.open(out_name);
			map<int, int> counts;
			int lines_to_process;

			lines_to_process = lines_per_process;
			if (i == n-1)
				lines_to_process = lines_in_last_process;

			printf("son %d: lines to process: %d\n", i, lines_to_process);
			for(int j=0; j < lines_to_process; j++){
				printf("i:%d j:%d\n", i, j);
				x = stoi(fin_lines.at(i * lines_per_process + j));
				if (counts.find(x) == counts.end()){
					counts[x] = 1;
				} else {
					counts[x]++;
				}
			}
			
			for (auto &c: counts) {
				out_file << c.first << " " << c.second << endl;
			}
			exit(0); 
		}
	} 
	for(int i=0;i<n;i++){
		wait(NULL);
	}

	// while (fin >> x) {
	// 	if (counts.find(x) == counts.end()) counts[x] = 0;
	// 	counts[x]++;
	// }



	return 0;
}