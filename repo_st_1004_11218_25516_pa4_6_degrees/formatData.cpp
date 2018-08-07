#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <iostream>

using namespace std;


/* 
 * Arg[1] is unformatted input
 * Arg[2] is formatted output
 *
 */
int main(int argc, char* argv[]) {


  string s(argv[1]);
  string s2(argv[2]);

  ifstream in(s);
  ofstream out(s2);

  while(in) {

    string s1;

    if(!getline(in,s1)) break;

    istringstream ss(s1);
    vector<string> record;

    while(ss) {
      string next;
      if(!getline( ss, next, '\t')) break;
      record.push_back(next);
    }

    auto it = record.begin();
    it++;
    if(record.size() < 8) continue;
    record.erase(it, it+8);

    for(it = record.begin();it != record.end(); it++) {
  
      out << *it << '\t';
    }
    out << endl;
  }
return 0;
}
    
