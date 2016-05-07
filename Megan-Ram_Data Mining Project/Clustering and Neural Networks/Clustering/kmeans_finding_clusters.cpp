#include<iostream>
#include<vector>
#include<string>
#include<cmath>
#include<sstream>
#include<cstdlib>
using namespace std;
typedef double ld;
ld eps = 1e-3;
ld maxi = 1e20;
vector<vector<ld> > inputdata, center, dist, next_center;
vector<int> nearest_center, next_nearest_center;
int input_rows, input_cols;
int K; // Number of clusters;

vector<string> names;
void get_inputs(){
	//cin >> input_rows >> input_cols;
	input_rows = 0; int good_rows = 0;
	string t ;
	while( getline(cin , t) ){
	//	if(input_rows == 1 ) break;
		inputdata.resize( input_rows + 1);
	//	cout<<t<<endl;
		istringstream in(t);
		//cout<<t.size()<<endl;
		if(t.size() < 10) continue;
		string s; 
		in >> s;
	//	cout<<s<<endl; // Name of currency
		
		string temp; int  missing = 0, y = 0;
		while(in >> temp) {
			
			if( temp.size() == 1) {   //Found missng value
				missing ++; 
			}
			else {   // Normal value
				istringstream in1(temp);
				ld tt;
				in1 >> tt;
				inputdata[input_rows].push_back(tt);
			}
		}
	//	cout<<inputdata[input_rows].size()<<endl;
		if(missing) inputdata[input_rows].resize(0);
		else {
			if(inputdata[input_rows].size() !=1001 ) y++;  // checking if there are 1001 valid data points in the row
			input_rows++;
			names.push_back(s);
		}
		//cout<<y<<endl;
	}
	input_rows = inputdata.size();
	input_cols = inputdata[0].size();
	cout<<input_rows<<" "<<input_cols<<endl;
	
	return ;
	
	for(int i =0 ; i< input_rows; i++) inputdata[i].resize(input_cols);
		 
	for(int i =0 ; i< input_rows; i++){
	 
	 for(int j = 0; j<input_cols; j++){
		 cin  >> inputdata[i][j];
	 }
	}
}
void show_inputs(){
	for(int i =0 ; i< input_rows; i++){
	 for(int j = 0; j<input_cols; j++){
		 cout<<inputdata[i][j]<<" ";
	 }
	 cout<<endl;
	}
}
void init(){
	center.resize(K);
	for(int i =0 ; i<K ; i++) center[i] = inputdata[i];
	next_center = center;
	
	dist.resize(K);
	for(int i =0 ; i<K ; i++) dist[i].resize(input_rows);

	nearest_center.resize(input_rows);
	next_nearest_center.resize(input_rows);
}
ld computedist(int cindex, int iindex){
	ld d = 0;
	for(int i =0 ; i<input_cols; i++){
		if(center[cindex][i] - inputdata[iindex][i] < eps) continue;
		if(d > maxi )continue;
		d += (center[cindex][i] - inputdata[iindex][i])*(center[cindex][i] - inputdata[iindex][i]);
	}
	return sqrt(d);	
}
bool iterate (){
	
	  for(int j = 0; j<input_rows; j++){
	  	long double min_dist = -1;
		for(int i = 0 ; i<K ; i++){
		 	dist[i][j] = computedist( i, j);
	 		if(min_dist < 0 or min_dist > dist[i][j]) {
	 			min_dist = dist[i][j];
	 			next_nearest_center[j] = i;
	 		}
		}
	 }
}
void show_dist(){
//cout<<K<<" " << input_rows<<endl;
//cout<<dist.size()<< " "<<dist[0].size();
	for(int i = 0 ; i<K ; i++) {
	  for(int j = 0; j<input_rows; j++)
	  	cout<<dist[i][j]<<" ";
	   cout<<endl; 
	}
}
void update_center(){
	
	vector<int> cluster_size(K);
	
	for(int i = 0; i<next_center.size(); i++)
	 for(int j = 0 ; j<next_center[0].size(); j++)
	 	next_center[i][j] = 0;
	 	
	for(int i =0 ; i<input_rows; i++){
		cluster_size[ next_nearest_center[i]] ++;
		for(int j = 0; j<input_cols; j++)
		next_center[next_nearest_center[i]][j] += min(inputdata[i][j],10.0);
	}
	
	for(int i =0 ; i<K; i++){
		if(cluster_size[i] == 0) continue;
		for(int j =0 ;j<input_cols; j++){
				next_center[i][j] /= cluster_size[i];
				center[i][j] = next_center[i][j];
		}
	}
}
void show_center(){
	cout<<"Centers ";
	for(int i =0 ; i<K; i++){
	 for(int j = 0; j <input_cols; j++)
	 	cout<<center[i][j]<<" ";
	 	cout<<endl<<endl;
	}
}
ld intracluster(){
	ld sum  = 0;
	 for(int i = 0; i <input_rows; i++){
	 	if(  isnan(computedist(nearest_center[i], i))) continue; 
	 	sum += computedist(nearest_center[i], i)*computedist(nearest_center[i], i);
	 	//cout<< i<<" "<<nearest_center[i]<<" "<<isnan(computedist(nearest_center[i], i))<<endl;
	}
	return sum;		
}
int main (){
	
		get_inputs();
		//show_inputs();
	for( K = 1; K <= 30; K++){
		init();	
		int iteration = 1;
		while(iteration < 50){
			iteration++;
			//cout<<"Iteration "<<iteration++<<endl;
			nearest_center = next_nearest_center;
			iterate(); 
			//cout<<"nearest centers ";for(int i =0 ;i<input_rows; i++) cout<<next_nearest_center[i]<<" "; cout<<endl;
			//show_dist();
			update_center();
				
			if(nearest_center == next_nearest_center) break;
		}
//		show_center();
		//cout<<"nearest centers ";for(int i =0 ;i<input_rows; i++) cout<<next_nearest_center[i]<<" "; cout<<endl;
		cout<< K<<"\t"<<intracluster()<<endl;
	}
}
