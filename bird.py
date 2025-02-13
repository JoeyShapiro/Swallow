import math

velocity_flap = 5
wing_area = 0.3
k = 0.5
Force_Total = k * velocity_flap**2 * wing_area

angle_of_attack = 0
Force_Lift = Force_Total * math.cos(angle_of_attack)
Force_Thrust = Force_Total * math.sin(angle_of_attack)

angles = [0, math.pi/4, math.pi/2, 3*math.pi/4, math.pi, 5*math.pi/4, 3*math.pi/2, 7*math.pi/4, 2*math.pi]
for a in angles:
    Force_Lift = Force_Total * math.cos(a)
    Force_Thrust = Force_Total * math.sin(a)
    print("a:", a/math.pi, "pi")
    print("\tForce Lift:", round(Force_Lift, 3))
    print("\tForce Thrust:", round(Force_Thrust, 3))

# bernoulli's equation
# ΔP=21​ρv2
# use instead of velocity_flap, i think
