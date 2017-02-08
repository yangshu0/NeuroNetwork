#include <opencv2/opencv.hpp>
#include<math.h>

using namespace std;
using namespace cv;

class Perceptron {
public:
	double* weights;
	double c = 0.01;

	Perceptron() {};
	Perceptron(int n) {
		weights = new double[n];
		for (int i = 0; i < n; i++) {
			weights[i] = 1.0 - (double)rand() / RAND_MAX;
			}
	}

	int feedforward(double* inputs, int n) {
		double sum = 0;
		for (int i = 0; i < n; i++) {
			sum += inputs[i] * weights[i];
		}
		return activate(sum);
	}

	int activate(double sum) {
		if (sum > 0) 
			return 1;
		else 
			return -1;
	}

	int train(double* inputs, int n, int desired) {
		int guess = feedforward(inputs, n);
		double error = desired - guess;
		for (int i = 0; i < n; i++) {
			weights[i] += c * error * inputs[i];
		}
		return guess;
	}
};

class Trainer {
public:
	double* inputs;
	int answer;
	Trainer() {};
	Trainer(double x, double y, int a) {
		inputs = new double[3];
		inputs[0] = x;
		inputs[1] = y;
		inputs[2] = 1;
		answer = a;
	}
};

Perceptron ptron;
Trainer training[2000];
int inputlength = 3;
int trainlength = 2000;
int count = 0;
int w = 640;
int h = 380;

double f(double x) {
	return  x/4 + 1;
}

void setup() {
	ptron = Perceptron(3);

	for (int i = 0; i < trainlength; i++) {
		double x = w/2 - (double)rand() / RAND_MAX * w;
		double y = h/2 - (double)rand() / RAND_MAX * h;
		int answer = 1;
		if (y < f(x)) 
			answer = -1;
		training[i] = Trainer(x, y, answer);
	}
}

int main(int argn, char* argc[])
{
	Mat image = Mat::zeros(h, w, CV_8UC3);
	namedWindow("image");

	int count = 0;
	setup();
	ptron.train(training[count].inputs, 3, training[count].answer);
	count = (count + 1) % 2000;
	int noFill;
	for (int i = 0; i < 2000; i++) {
		int guess = ptron.train(training[i].inputs, 3, training[i].answer);
   		if (guess > 0)
			circle(image, Point(training[i].inputs[0] + w/2, training[i].inputs[1]+h/2), 8.0, Scalar(0, 0, 255), 1, 8);
		else
			circle(image, Point(training[i].inputs[0] + w/2, training[i].inputs[1] + h/2), 8.0, Scalar(255, 0, 0), 1, 8);
		
		cout << ptron.weights[0] << ' ' << ptron.weights[1] << ' ' << ptron.weights[2] << endl;
		imshow("image", image);
		waitKey(20);
	}
	waitKey(0);
}