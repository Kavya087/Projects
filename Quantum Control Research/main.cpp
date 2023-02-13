//
//  main.cpp
//  Quantum Control
//
//  Created by Kavya on 11/11/22.
//

#include <iostream>
#include<array>
#include <fstream>
#include <complex>
#include <string>

using namespace std;
const int Nt=1000000; //Number of time stamps double
double max_t=70.0; //total time the simulation runs
double dt=max_t/Nt;
double omega1, omega_one=50;
double omega2=0.10;
double gamma2=2.0;
double gamma3=0.3;
double gamma32=gamma2+gamma3; double delta1=15.0;
double delta2;
float time_instances[Nt];

std::complex<double> imaginery(0, 1);
std::complex<double> rho11[Nt];
std::complex<double> rho22[Nt];
std::complex<double> rho33[Nt];
std::complex<double> rho21[Nt];
std::complex<double> rho32[Nt];
std::complex<double> rho31[Nt];
std::complex<double> deltaChange_rho33[700];

double tao=0.5;
double time_pi=M_PI/omega_one;
int pulse_timesteps=time_pi/dt;

int nopulse_timesteps=(tao-time_pi)/dt;
int pulse=12;
int total_pulse_timesteps=pulse* (pulse_timesteps+nopulse_timesteps);
std::complex<double> rho_oneone(int i) { std::complex<double> rho_11;
rho_11=gamma2*rho22[i]-imaginery*0.5*omega1*rho21[i]+imaginery*0.5*omega1*conj(rho21[i]) ;
 return rho_11; }
std::complex<double> rho_twotwo(int i) { std::complex<double> rho_22;
rho_22=-gamma2*rho22[i]+gamma3*rho33[i] +imaginery*0.5*omega1*rho21[i] -imaginery*0.5*omega1*conj(rho21[i]) -imaginery*0.5*omega2*rho32[i] +imaginery*0.5*omega2*conj(rho32[i]);
return rho_22; }
std::complex<double> rho_threethree(int i) { std::complex<double> rho_33;
rho_33=-gamma3*rho33[i]+imaginery*0.5*omega2*rho32[i]-imaginery*omega2*0.5*conj(rho32[i] );
return rho_33; }
std::complex<double> rho_twoone(int i) { std::complex<double> rho_21;
rho_21=-imaginery*0.5*omega1*rho11[i] +imaginery*omega1*0.5*rho22[i] +(imaginery*delta1-0.5*gamma2)*rho21[i] -imaginery*0.5*omega2*rho31[i];
return rho_21; }
std::complex<double> rho_threetwo(int i) { std::complex<double> rho_32;
rho_32=-imaginery*0.5*omega2*rho22[i] +imaginery*omega2*0.5*rho33[i] +(imaginery*delta2-0.5*gamma32)*rho32[i] +imaginery*0.5*omega1*rho31[i];
return rho_32; }
std::complex<double> rho_threeone(int i) { std::complex<double> rho_31;

rho_31=-imaginery*0.5*omega2*rho21[i] +imaginery*omega1*0.5*rho32[i] +(imaginery*(delta2+delta1)-0.5*gamma3)*rho31[i];
return rho_31; }


int main(int argc, const char * argv[]) {
    
    ofstream fout;
    
    for (int j=-350; j<350; j++){
        delta2=j;
        //inital conditions
        rho11[0]={0,0};
        rho22[0]={1,0};
        rho33[0]={0,0};
        rho21[0]={0,0};
        rho32[0]={0,0};
        rho31[0]={0,0};
        time_instances[0]=0;
        
        int ki=0;
        for (int k=0;k<pulse;k++){
            int iteration=0;
            
            while (iteration<pulse_timesteps){
                omega1=omega_one;
                time_instances[ki+iteration+1]=time_instances[ki+iteration]+dt;
                rho11[ki+iteration+1]=rho11[ki+iteration]+dt*rho_oneone(ki+iteration); rho22[ki+iteration+1]=rho22[ki+iteration]+dt*rho_twotwo(ki+iteration); rho33[ki+iteration+1]=rho33[ki+iteration]+dt*rho_threethree(ki+iteration); rho21[ki+iteration+1]=rho21[ki+iteration]+dt*rho_twoone(ki+iteration); rho32[ki+iteration+1]=rho32[ki+iteration]+dt*rho_threetwo(ki+iteration); rho31[ki+iteration+1]=rho31[ki+iteration]+dt*rho_threeone(ki+iteration); iteration++;
            }
            
            while (iteration<nopulse_timesteps+ pulse_timesteps){
                omega1=0;
                time_instances[ki+iteration+1]=time_instances[ki+iteration]+dt;
                rho11[ki+iteration+1]=rho11[ki+iteration]+dt*rho_oneone(ki+iteration);
                rho22[ki+iteration+1]=rho22[ki+iteration]+dt*rho_twotwo(ki+iteration);
                rho33[ki+iteration+1]=rho33[ki+iteration]+dt*rho_threethree(ki+iteration);
                rho21[ki+iteration+1]=rho21[ki+iteration]+dt*rho_twoone(ki+iteration);
                rho32[ki+iteration+1]=rho32[ki+iteration]+dt*rho_threetwo(ki+iteration);
                rho31[ki+iteration+1]=rho31[ki+iteration]+dt*rho_threeone(ki+iteration);
                iteration++;
            }
            
            ki+=nopulse_timesteps+pulse_timesteps;
        }
        
        for (int i = total_pulse_timesteps; i < Nt-1; i++) { time_instances[i+1]=time_instances[i]+dt;
            rho11[i+1]=rho11[i]+dt*rho_oneone(i);
            rho22[i+1]=rho22[i]+dt*rho_twotwo(i);
            rho33[i+1]=rho33[i]+dt*rho_threethree(i);
            rho21[i+1]=rho21[i]+dt*rho_twoone(i);
            rho32[i+1]=rho32[i]+dt*rho_threetwo(i);
            rho31[i+1]=rho31[i]+dt*rho_threeone(i);
        }
        
        deltaChange_rho33[j+100]=rho33[Nt-1];
        
        //fout.open("Delta2_" +to_string(j)+".txt");
        //
        //for (int i=0;i<Nt-1;i++){
        //
        //fout<<time_instance[i]<<","<<real(rho11[i])<<","<<imag(rho11[i])<<","<<real(rho22[i])<<","<<imag(rho22[i]) <<","<<real(rho33[i])<<","<<imag(rho33[i]) <<","<<real(rho21[i])<<","<<imag(rho21[i]) <<","<<real(rho32[i])<<","<<imag(rho32[i]) <<","<<real(rho31[i])<<","<<imag(rho31[i]) <<endl;
        // }
        //fout.close();
    
    fout.open("Delta2_"+to_string(j)+".txt");
    
    for (int i = 0; i < Nt-1; i++) { fout<<time_instances[i]<<","<<real(rho11[i])<<","<<imag(rho11[i]);
    }
    fout.close();
    }
        return 0;
            
        }
