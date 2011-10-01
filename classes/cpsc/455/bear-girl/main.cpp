#include <iostream.h>
#include <fstream.h>
#include <math.h>

// Enumerated type for the state of the system
typedef enum {
	X_G, Y_G, R_G,
	X_B, Y_B, R_B,
	X_T, Y_T,
	COS_THETA_G, SIN_THETA_G,
	COS_THETA_B, SIN_THETA_B,
	STATE_SIZE
} BG_STATENAMES;
typedef double BG_STATE[STATE_SIZE];

// A function to initialize a double
double init(char *s) {
	cout << s;
	double d;
	cin >> d;
	return d;
}

// between tests to see if a <= b <= c or if c <= b <= a
int between(double a, double b, double c) {
	if(((a <= b) && (b <= c)) || ((c <= b) && (b <= a))) return 1;
	return 0;
}

// main routine
int main(int argc, char *argv[]) {
	// state and change in state
	BG_STATE state, dstate;
	// output files
	ofstream bear(argv[1]), girl(argv[2]);
	// return value
	int ret = 0;

	// Get initial state from the user
	state[X_G] = init("Girl's x position: ");
	state[Y_G] = init("Girl's y position: ");
	state[R_G] = init("Girl's speed: ");
	state[X_B] = init("Bear's x position: ");
	state[Y_B] = init("Bear's y position: ");
	state[R_B] = init("Bear's speed: ");
	state[X_T] = init("Telephone booth's x position: ");
	state[Y_T] = init("Telephone booth's y position: ");

	// Find the angle the girl is heading
	double opp = state[Y_T] - state[Y_G];
	double adj = state[X_T] - state[X_G];
	double hyp = sqrt(adj * adj + opp * opp);
	state[COS_THETA_G] = adj / hyp;
	state[SIN_THETA_G] = opp / hyp;

	// Print a heading
	cout << "\tX_G\tY_G\tX_B\tY_B" << endl;

	// Clear dstate
	for(int i = 0; i < STATE_SIZE; i++) dstate[i] = 0;

	// Loop until a break statement is reached
	for(;;) {

		// Print the new positions
		cout << '\t' << state[X_G] << '\t' << state[Y_G]
		     << '\t' << state[X_B] << '\t' << state[Y_B] << endl;

		if(argc>2) girl << state[X_G] << ' ' << state[Y_G] << endl;
		if(argc>1) bear << state[X_B] << ' ' << state[Y_B] << endl;

		// Find the girl's change in position
		dstate[X_G] = state[R_G] * state[COS_THETA_G];
		dstate[Y_G] = state[R_G] * state[SIN_THETA_G];

		// Update the girl's position
		state[X_G] += dstate[X_G];
		state[Y_G] += dstate[Y_G];

		// See if the girl reached the phone booth
		if(between(state[X_G], state[X_T], state[X_G] - dstate[X_G]) &&
		   between(state[Y_G], state[Y_T], state[Y_G] - dstate[Y_G])) {
			cout << "The girl won." << endl;
			cout << "Distance from bear to girl: ";
			cout << sqrt(pow(state[X_G] - state[X_B], 2) +
			             pow(state[Y_G] - state[Y_B], 2)) << endl;
			ret = 1;
			break;
		}

		// Find the angle the bear is heading
		opp = state[Y_G] - state[Y_B];
		adj = state[X_G] - state[X_B];
		hyp = sqrt(adj * adj + opp * opp);
		state[COS_THETA_B] = adj / hyp;
		state[SIN_THETA_B] = opp / hyp;

		// Find the bear's change in position
		dstate[X_B] = state[R_B] * state[COS_THETA_B];
		dstate[Y_B] = state[R_B] * state[SIN_THETA_B];

		// Update the bear's position
		state[X_B] += dstate[X_B];
		state[Y_B] += dstate[Y_B];

		// See if the bear reached the girl
		if(between(state[X_B], state[X_G], state[X_B] - dstate[X_B]) &&
		   between(state[Y_B], state[Y_G], state[Y_B] - dstate[Y_B])) {
			cout << "The bear won." << endl;
				cout << "Distance from girl to phone booth: ";
			cout << sqrt(pow(state[X_G] - state[X_T], 2) +
			             pow(state[Y_G] - state[Y_T], 2)) << endl;
			ret = 2;
			break;
		}

	}

	// Output the final position
	cout << "Final positions: " << endl;
	cout << '\t' << state[X_G] << '\t' << state[Y_G]
	     << '\t' << state[X_B] << '\t' << state[Y_B] << endl;

	if(argc>2) girl << state[X_G] << ' ' << state[Y_G] << endl;
	if(argc>1) bear << state[X_B] << ' ' << state[Y_B] << endl;

	return ret;
}
