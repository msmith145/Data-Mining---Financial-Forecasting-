#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;

vector<double> a;
double minimum, maximum;
int main(){
	ifstream file; 
	file.open("dowjones.txt");
	//stringstream input();
	string s ;
	int i = 0;
	while(file>>s){
				double x;
				stringstream input(s);
				input>>x;
				a.push_back(x);
				//cout<<x<<endl;	
		i++;
	}
		
	minimum = *min_element(a.begin(), a.end());
	maximum = *max_element(a.begin(), a.end());
	cout<<minimum<<" "<<maximum<<endl;
	
	int n = a.size(), nninputs = 4;
	
	//cout<<"Enter number of inputs :";
	//cin>>nninputs;
	
	for(int i =0; i<n ; i++) 
		a[i] = (maximum-a[i])/ (maximum-minimum);
		
	for( int i =0 ; i+nninputs<n ; i++){
		for( int j = i; j<=i+nninputs; j++){
			printf("%.5f", a[j]);
			if(j!=i+nninputs)cout<<"\t";
			else cout<<endl;
		}
	}
	
	file.close();
	return 0;
}
