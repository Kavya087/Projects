#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat April 11 20:47:16 2022

@author: kavya
"""

from rocketpy import Environment, Rocket, SolidMotor, Flight
import datetime

def drogueTrigger(p, y):
    return True if y[5] < 0 else False


def mainTrigger(p, y):
    return True if y[5] < 0 and y[2] < 800 + 1400 else False

#All Distances Are In Meters

#Setting up the environment of the launch 
Env = Environment(
    railLength=4.9, # length of the rail the rocket is launched off of
    latitude=42.728413, #latitude of the launch site
    longitude=-73.691788, #longitude of the launch site
    elevation=0, #elevation of the alunch site
)
#setting the time of the launch... preset to tommorrow 
tomorrow = datetime.date.today() + datetime.timedelta(days=1)
Env.setDate((tomorrow.year, tomorrow.month, tomorrow.day, 12))  # Hour given in UTC time

#Getting the atmospheric conditions for the specific date
Env.setAtmosphericModel(type='Forecast', file='GFS')
Env.info()


#Setting up the motor used in the rocket
motor = SolidMotor(
    thrustSource="thrustCurve.csv", #thrust curve of the motor you're using
    burnOut=4.2,
    grainNumber=5,
    grainSeparation=5 / 1000,
    grainDensity=1820,
    grainOuterRadius=25 / 1000,
    grainInitialInnerRadius=10 / 1000,
    grainInitialHeight=100 / 1000,
    nozzleRadius=34 / 1000,
    throatRadius=8 / 1000,
    interpolationMethod="linear",
)

motor.info()

#Creating a Rocket
Rocket1 = Rocket(
    motor=motor,
    radius=127 / 2000,
    mass=19.197 - 2.956,
    inertiaI=7.90,
    inertiaZ=0.1351,
    distanceRocketNozzle=-1.8712,
    distanceRocketPropellant=-0.864207,
    powerOffDrag="powerOffDragCurve.csv", #Rocket’s drag coefficient as a function of Mach number when the motor is off
    powerOnDrag="powerOnDragCurve.csv", #Rocket’s drag coefficient as a function of Mach number when the motor is on
)

Rocket1.setRailButtons([0.3, -0.6]) #distance to center of mass in meters 

#Adding Nose Cone 
NoseCone = Rocket1.addNose(length=0.55829, kind="vonKarman", distanceToCM=0.835801)

#Adding Fins
FinSet = Rocket1.addTrapezoidalFins(
    n=4,
    rootChord=0.120,
    tipChord=0.040,
    span=0.100,
    distanceToCM=-1.04956,
    cantAngle=0,
    radius=None,
    airfoil=None,
)

#Adding Tail
Tail = Rocket1.addTail(
    topRadius=0.05478, bottomRadius=0.0345, length=0.075, distanceToCM=-1.184626
)


Main = Rocket1.addParachute("Main",
    CdS=10.0,
    trigger=mainTrigger,
    samplingRate=105,
    lag=1.5,
    noise=(0, 8.3, 0.5),
)

Drogue = Rocket1.addParachute("Drogue",
    CdS=1.0,
    trigger=drogueTrigger,
    samplingRate=105,
    lag=1.5,
    noise=(0, 8.3, 0.5),
)

TestFlight = Flight(rocket=Rocket1, environment=Env, inclination=75, heading=0)
TestFlight.allInfo()
















