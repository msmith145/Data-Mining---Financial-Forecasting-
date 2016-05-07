#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<sstream>
#include<cstdlib>
#include<algorithm>
using namespace std;
typedef long double ld;
#define INF 1e100

long double eps = 1e-3;
vector<ld> rawdata, predicted; 
vector< vector<ld> > tuple;
vector<int> inputdata, testdata, actualtest;
int history; // The number of previous values on which a particular value is dependent
int K; // The K of the KNN algorithm

void get_input(){
	ld temp;
	while(cin >> temp) {
		rawdata.push_back(temp);
	}

}
void show_data(){
	for(int i =0 ; i<rawdata.size(); i++) cout<<rawdata[i]<<endl;
}
vector<vector<ld> > build( int pos){
	
	vector<vector<ld> > ret;
	for(int i = pos ; i+history<rawdata.size(); i++){
		vector<ld> v;
		for(int j = i; j<i+history; j++)
		v.push_back(rawdata[j] - rawdata[i]);
		ret.push_back(v);
	}
	return ret;
}
void show (vector<int> & v){
	for(int i = 0; i<5 and i <v.size(); i++){
	 for(int j = 0; j<tuple[0].size(); j ++)
	 	cout<<tuple[v[i]][j]<<" "; 
	 	cout<<endl;
	}
}
void build_input( int kfold){   // 0<= kfold <=8
	inputdata.resize(0);
	testdata.resize(0);
	actualtest.resize(0);
	tuple = build(history );
	int onetenth = tuple.size()/10;
	
	for(int i = 0; i < tuple.size(); i++){
		if( i / onetenth == kfold ){
			//if( i == kfold*onetenth + 5)
			testdata.push_back(i);
		}
		else if( i < 9*onetenth){
			inputdata.push_back(i);
		}	
		else{
			actualtest.push_back(i);
		}
	}
	/*
	cout<<rawdata.size()<< " "<< inputdata.size() << " "<<testdata.size()<<" "<<actualtest.size()<<endl;
	show (inputdata);
	cout<<endl;
	show (testdata);
	cout<<endl;
	show (actualtest);	*/
}
ld get_dist (int x, int y){
	//for(int k =0 ; k<tuple[0].size(); k++) cout<<tuple[x][k]<<" "; cout<<endl;
	//for(int k =0 ; k<tuple[0].size(); k++) cout<<tuple[y][k]<<" "; cout<<endl;
	
	ld ret = 0 ;
	for(int i = 0 ; i<tuple[0].size()-1; i++) // last column is the actual prediction 
		ret += (tuple[x][i] - tuple[y][i])*(tuple[x][i] - tuple[y][i]);
	//cout<<sqrt(ret)<<endl;
	return sqrt(ret);
}
ld knn_predict(vector<int>& current_test_data){
	//cout<<"Testing "<<current_test_data[0]<<endl;
	
	predicted.resize(current_test_data.size());
	for(int i = 0 ; i<current_test_data.size(); i ++){
		vector<pair<ld,int> > dist;
		for(int j = 0 ; j<inputdata.size(); j++){
		//	dist.push_back( make_pair(dist(testdata[i],inputdata[j]),i));
			ld cur_dist = get_dist(current_test_data[i],inputdata[j]);
			dist.push_back( make_pair(cur_dist, inputdata[j]));
		}
		sort(dist.begin(),dist.end());
		
		ld nr = 0, dr = 0;
		bool done = false;
		for(int j = 0 ; j <K ; j++){
			//cout<<"Nearest Neighbour dist = "<< dist[j].first<<" index = "<<dist[j].second <<" "; for(int k =0 ; k<tuple[0].size(); k++) cout<<tuple[dist[j].second ][k]<<" "; cout<<endl;
			if(dist[j].first < eps ){
				predicted[i] = tuple[dist[j].second].back();
				done = true;
				break;
			}
			else{
				nr += (1/dist[j].first)*tuple[dist[j].second].back();
				dr += (1/dist[j].first);
			}	
		}
		if(!done) predicted[i] = nr/dr;
	}
	
	ld nrmse = 0, mini = INF, maxi = -INF;
	for(int i =0 ; i<predicted.size(); i++){
	//	cout<<tuple[current_test_data[i]].back()<<" "<<predicted[i]<<endl;
		ld tempo = (tuple[current_test_data[i]].back() - predicted[i]);
		nrmse += tempo * tempo;
		if( mini > tuple[current_test_data[i]].back()) mini = tuple[current_test_data[i]].back();
		if( maxi < tuple[current_test_data[i]].back()) maxi = tuple[current_test_data[i]].back();
	}
	nrmse = sqrt(nrmse/predicted.size());
	nrmse /= maxi - mini;
	return nrmse;
}
int main(){
	get_input();
	//show_data();	
	ld optimal_error = INF;
	int optimal_history = -1;
	int optimal_K = -1;
	int optimal_kfold = -1;
	int steps = 0;
	for( int kfold = 0; kfold<=8 ; kfold ++)
	for( history = 10; history <= 20; history+=2)
	for( K = 40; K<=60; K+=2)
	//int kfold = 1; history = 10, K = 42;
	{
		cerr<<steps ++<<endl;
		build_input(kfold);
		ld cur_error = knn_predict(testdata);
		//cout<<cur_error<<endl;
		if( cur_error < optimal_error){
			optimal_error = cur_error;
			optimal_history = history;
			optimal_K = K;
			optimal_kfold = kfold;
			cout<<"Optimal Error = "<<optimal_error<<" ";
			cout<<"Optimal History = "<<optimal_history<<" ";
			cout<<"Optimal K = "<< optimal_K<<" ";
			cout<<"Optimal_kfold = "<< optimal_kfold<<endl;
		}
	}
	cout<<"Optimal Error = "<<optimal_error<<endl;
	cout<<"Optimal History = "<<optimal_history<<endl;
	cout<<"Optimal K = "<< optimal_K<<endl;
	cout<<"Optimal_kfold = "<< optimal_kfold<<endl;
}
