import enum
import random

class Orientation(enum.Enum):
	COLINEAR = 0
	CLOCKWISE = 1
	COUNTER_CLOCKWISE = 2

def orientation(p0,p1,p2):
    # Adding 3rd dimension in order to do cross product
    p0_vec = vector(p0 + tuple([0]))
    p1_vec = vector(p1 + tuple([0]))
    p2_vec = vector(p2 + tuple([0]))

    v1 = p1_vec - p0_vec
    v2 = p2_vec - p0_vec

    orient_num = v1.cross_product(v2)
    if orient_num == 0:
    	return Orientation.COLINEAR
    elif orient_num > 0:
    	return Orientation.COUNTER_CLOCKWISE
    else:
    	return Orientation.CLOCKWISE

def generate_random_points(points_num):
	max_x = 10000
	max_y = 10000

	points = []

	for _ in range(0,points_num):
		x = random.randint(-max_x,max_x)
		y = random.randint(-max_y,max_y)

		points = points + [(x,y)]

	return points

def euclidean(p1, p2):
    return ((p1[0] - p2[0])**2 + (p1[0] - p2[0])**2)**(0.5)

def gift_wrapping(points):
    if len(points) == 2:       # If we have two points we return the line
        return points

    min_x = min([p[0] for p in points])
    min_points = [p for p in points if p[0] == min_x]

    convex_hull_points = [sorted(min_points, key = lambda p: p[1])[0]] # Finding the left-most point

    curr_point = convex_hull_points[0]

    while 1:
        curr_next_point = points[0]

        for i in range(1, len(points)):         # Finding the next point to add in the convex hull
            next_cand_point = points[i]

            if orientation(curr_point,curr_next_point,next_cand_point) == Orientation.CLOCKWISE:   
                curr_next_point = next_cand_point
            
            # If the vectors are colinear we add the vector that corresponds to the most far point
            if orientation(curr_point,curr_next_point,next_cand_point) == Orientation.COLINEAR:     
                if euclidean(curr_point, next_cand_point) > euclidean(curr_point, curr_next_point):
                    curr_next_point = next_cand_point
        
        # We add that point to the convex hull and we remove it from the point list
        # because we do not want to check it again 
        convex_hull_points.append(curr_next_point)
        points.remove(curr_next_point)

        if curr_next_point == convex_hull_points[0]:
            break

        curr_point = curr_next_point
    
    return convex_hull_points

points = generate_random_points(100)

scatter_plot(points).save('ex4_random_points.png')

convex_hull = gift_wrapping(list(points))

plot_list = [line([convex_hull[i],convex_hull[i+1]]) for i in range(0,len(convex_hull) - 1)]
(sum(plot_list) + scatter_plot(points)).save("ex4_ch2.png")