#ifndef __DEBUG__

#define __DEBUG__ 

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector> 
#include <queue>
#include <chrono>
#include <climits>
#include <cassert>
#include <tuple>
#include <ctime> 
#include <iomanip>
#include <set>
#include <unistd.h>
#include <unordered_map>
#include <mpi.h>
#include <string.h>
#include <unordered_set>

using namespace std;

typedef int ll;
typedef vector<ll> vi;
typedef vector<vi> vvi ;
typedef pair<ll,ll> ii ;
typedef vector<ii> vii ;
typedef vector<vvi> vvii ;
typedef queue<ll> q ;

#define FORWARD 0 
#define BACKWARD 1 


const int no_debug = 0 ;
const int log_emergency=1 ;
const int evaluate_mode = 0 ;


void log_graph (vvii &a) {
    if (evaluate_mode) return ;
    if (no_debug) return ;
    for (int i=0; i<a.size (); i++) {
        for (auto &it1:a[i]) {
            cerr <<i << " : " << it1[0] << " " << it1[1] << " " << it1[2] << " " << it1[3] << endl;
        }
    }
}

void log_arr (vi &a) {
    if (evaluate_mode) return ;
    if (no_debug) return ;
    for (auto &it:a) {
        cerr << it << " " ;
    }
    cerr << endl ;
}

void log_message (const string &msg) {
    if (evaluate_mode) return ;
     if (no_debug) return ;
    cerr << msg << endl ;
}

void log_message_push (const string &msg) {
    return ;
    //  if (no_debug) return ;
    if (evaluate_mode) return ;
    cerr << msg << endl ;
}

void log_sos (const string &msg) {
    if (evaluate_mode) return ;
	if (!log_emergency) return ;
	cout << msg << endl ;
}

#endif
