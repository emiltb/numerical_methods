#include <functional>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <iostream>
using namespace std;

function<double(double, int)> qspline(vector<double>&x, vector<double>&y) {
//if(y.size() != x.size()) throw logic_error("x.size != y.size");

int n = x.size();
vector<double> p(n-1), dx(n-1), c(n-1), b(n-1);

for(int i=0; i<n-1; i++) {
	dx[i] = x[i+1] - x[i];
	p[i] = (y[i+1] - y[i]) / dx[i];
}

c[0]=0;
for(int i=0; i<n-2; i++) {
	c[i+1] = (1 / dx[i+1]) * (p[i+1] - p[i] - c[i]*dx[i]);
}

c[n-2] = c[n-2]/2;
for(int i=n-3; i >= 0; i--) {
	c[i] = (1 / dx[i]) * (p[i+1] - p[i] - c[i+1]*dx[i+1]);
}

for(int i=0; i<n-1; i++) {
	b[i] = p[i] - c[i]*dx[i];
}


return [x,y,c,b,n](double z, int flag = 0)->double {

	int i = 0, j = n - 1;

	while(j - i > 1) {
		int m = (i + j) / 2;
		if(z > x[m]) {
			i = m;
		}
		else {
			j = m;
		}
	};

	double dx = z - x[i];
	switch(flag) {
	case 0: {
		return y[i] + dx * (b[i] + dx * c[i]);
	}
	case 1: {// Return integral
		double S = 0;
		for (int k = 0; k<i; k++) {
			double dx = x[k+1] - x[k];
			S+= dx*(y[k] + dx*(b[k]/2 + dx*c[k]/3));
		}
		return S + dx*(y[i] + dx*(b[i]/2 + dx*c[i]/3));
	}
	case -1: { // Return derivative
		return b[i] + 2*dx*c[i];
	}
	}
};
}
