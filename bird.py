import math

velocity_flap = 0.0
wing_area = 0.0
k = 0.0
Force_Total = k * velocity_flap**2 * wing_area

angle_of_attack = 0.0
Force_Lift = Force_Total * math.cos(angle_of_attack)
Force_Thrust = Force_Total * math.sin(angle_of_attack)

# bernoulli's equation
# ΔP=21​ρv2
# use instead of velocity_flap, i think
