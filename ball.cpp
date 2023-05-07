#include	<stdio.h>
#include	<math.h>
#include	<string.h>
#include	<malloc.h>
#include	<stddef.h>
#include	<stdlib.h>
#include	<time.h>

//Constant
#define	PAI	(atan(1.e0) * 4.e0)	//Circumference
#define	DEG_RAD	(PAI / 180.e0)	//Conversion from degree to rad
#define	RAD_DEG	(180.e0 / PAI)	//Conversion from rad to degree
#define	GRAV	9.80665e0		//Gravitational Acceleration[m/s^2]

//Function
double c_L(double v0, double theta);
void euler(double *x, double *y, double *vx, double *vy);
void out_state_var(long n, double t, double x, double y, double vx, double vy);

//Variable
double	Del_t;
long	F_out_state;
long	Step_state;
FILE	*Fp_kidou;

//Main
int main(void){
	double ball_L;				//Distance[m]
	double v0, theta;
	
	//Initial State
	v0 = 10.e0;					//Initial Velocity[m/s]
	theta = 50.e0 * DEG_RAD;	//Launch Angle[rad]
	
	Del_t = 0.01e0;				//Time Step[s]
	
	//Output
	F_out_state = 1;			//Do: 1, Do not: 2
	Step_state = 2;				//How many steps to perform one output
	
	ball_L = c_L(v0, theta);
	printf("--end of program-- \n");
	printf("v0 = %.7lf, \n", ball_L);
}

//Calculate distance L[m]
double c_L(double v0, double theta){
	double t;		//Time[s]
	double x, y;	//Position in x,y direction[m]
	double vx, vy;	//Velocity in x,y direction[m/s]
	long n;			//Integration
	
	//Initialization of state quantity	
	x = 0.e0;	
	y = 0.e0;
	vx = v0 *cos (theta);
	vy = v0 *sin (theta);
	
	//Integration
	for	(n = 0; ; n++){
		if (n % 10 == 0) printf("n = %ld \n", n);
		t = n * Del_t;												//Time Update
		
		if (F_out_state == 1) out_state_var(n, t, x, y, vx, vy);	//Data Output
		euler(&x, &y, &vx, &vy);									//Eulerian Method
		
		if (y < 0.e0 && vy < 0.e0) return x;						//At the end of condition
	}
}

//Eulerian Method
void euler(double *x, double *y, double *vx, double *vy){
	*x += (*vx) * Del_t;
	*y += (*vy) * Del_t;
	*vx += 0.e0 * Del_t;
	*vy += -GRAV * Del_t;
	return;
}

//Output
void out_state_var(long n, double t, double x, double y, double vx, double vy){
	char text1[200];
	if (n == 0){
		//File Open
		if ((Fp_kidou = fopen("state_var.dat", "w")) == NULL) { printf("err fopen in out-kidou \n") ; exit(1); }
		//Fill in the first line
		fprintf(Fp_kidou, "t, x, y, vx, vy \n");
	}
	
	//Data Output
	if (n % Step_state == 0) {
		fprintf(Fp_kidou, "%.7le, %.7le, %.7le, %.7le, %.7le \n", t, x, y, vx, vy);
	}
	return;
}