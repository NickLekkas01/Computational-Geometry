import enum
import random


class Orientation(enum.Enum):
	COLINEAR = 0
	CLOCKWISE = 1
	COUNTER_CLOCKWISE = 2

class Edge:
	def __init__(self,p1,p2):
		self.p1 = p1
		self.p2 = p2

	def find_intersection(self, other):
		if self.p1 == other.p1 or self.p1 == other.p2:
			return self.p1
		elif self.p2 == other.p1 or self.p2 == other.p2:
			return self.p2
		else:
			None

	def __eq__(self, other):
		return (self.p1 == other.p1 and self.p2 == other.p2) or\
				(self.p1 == other.p2 and self.p2 == other.p1)

def generate_random_points(points_num):
	max_x = 10000
	max_y = 10000

	saw_x = set()
	points = []

	for _ in range(0,points_num):
		x = random.randint(-max_x,max_x)
		y = random.randint(-max_y,max_y)

		if x in saw_x:
			while x in saw_x:
				if x == max_x:
					x = -max_x
				else:
					x = x + 1
		
		saw_x.add(x)

		points = points + [(x,y)]

	return points

def orientation(p0,p1,p2):
	v1 = p1 - p0
	v2 = p2 - p0

	orient_num = v1.cross_product(v2)
	if orient_num == 0:
		return Orientation.COLINEAR
	elif orient_num > 0:
		return Orientation.COUNTER_CLOCKWISE
	else:
		return Orientation.CLOCKWISE

# The 2 functions below just apply the formulas in order to resolve the predicates
def on_edge_point(edge,point):
	matrix = Matrix([[edge.p1[0], edge.p1[1], 1],[edge.p2[0], edge.p2[1], 1],[point[0], point[1], 1]])

	if matrix.determinant() == 0:
		return True
	else:
		return False

def color_edge(edge, new_point, convex_hull_point):
	new_point_matrix  = Matrix([[edge.p1[0], edge.p1[1], 1],[edge.p2[0], edge.p2[1], 1],[new_point[0], new_point[1], 1]])
	convex_hull_matrix = Matrix([[edge.p1[0], edge.p1[1], 1],[edge.p2[0], edge.p2[1], 1],[convex_hull_point[0], convex_hull_point[1], 1]])

	sign_new_point = 1 if new_point_matrix.determinant() > 0 else -1
	sign_convex_hull_matrix = 1 if convex_hull_matrix.determinant() > 0 else -1

	if sign_new_point != sign_convex_hull_matrix:
		return 'red'
	else:
		return 'blue'

# We assume that the first 3 points are not colinear 
# and that every x - coordinate has at most 1 point
def beaneath_beyond(points):
	points = sorted(points, key=lambda e: e[0],reverse=True) # Sorting elements at decreasing order at x axis
	points_num = len(points)

	if points_num == 2:
		return [Edge(points[0], points[1])]

	convex_hull = [Edge(points[0],points[1]), Edge(points[1],points[2]), Edge(points[2],points[0])]
	curr_ch_points = set([points[0],points[1],points[2]])

	for index in range(3, points_num):			# For each point
		curr_point = points[index]
		prev_point = points[index - 1]

		curr_ch_points.add(prev_point)			# We're adding to convex hull points the previous point

		if curr_point == prev_point:
			continue
		
		# current point is outside convex hull so we need to find the first red edge, so we
		# check the edges that have previous point as vertex
		edges_to_examine = [edge for edge in convex_hull if edge.p1 == prev_point or edge.p2 == prev_point]

		for curr_edge in edges_to_examine:
			# Searching for a convex hull point that is not on the vertex
			# that we want to check its color

			points_to_choose = curr_ch_points.symmetric_difference(set([curr_edge.p1,curr_edge.p2]))
			find_color_ch_point = points_to_choose.pop()

			while on_edge_point(curr_edge, find_color_ch_point) == True:
				find_color_ch_point = points_to_choose.pop()
			
			# If we found a red edge we stop the searching because we found where to begin
			if color_edge(curr_edge, curr_point, find_color_ch_point) == 'red':
				edges_to_examine = [tuple([None,curr_edge])]					# we keep the previous red edge
				break
		
		# edges_to_examine works as queue in order to keep the edges
		# that needs coloring
		while edges_to_examine != []:
			curr_element = edges_to_examine.pop(0)
			curr_edge = curr_element[1]
			prev_red_edge = curr_element[0]
			
			intersection = curr_edge.find_intersection(prev_red_edge) if prev_red_edge != None else None

			if prev_red_edge is None:			# Now we find the points that need to expand the color
				next_points = [curr_edge.p1, curr_edge.p2]
			else:
				next_points = [curr_edge.p1] if curr_edge.p1 != intersection else [curr_edge.p2]

			# Coloring current edge
			points_to_choose = curr_ch_points.symmetric_difference(set([curr_edge.p1,curr_edge.p2]))
			find_color_ch_point = points_to_choose.pop()

			while on_edge_point(curr_edge, find_color_ch_point) == True:
				find_color_ch_point = points_to_choose.pop()

			# If current edge is red we should find the color of the tangent edges 
			# of the vertex(or vertexes) and we delete this edge from the convex hull

			if color_edge(curr_edge, curr_point, find_color_ch_point) == 'red':
				convex_hull.remove(curr_edge)

				edges_to_examine = edges_to_examine + [(curr_edge,edge) for edge in convex_hull if edge.p1 in next_points or edge.p2 in next_points]
			else:
				# If the edge is blue we create an edge between the new point
				# and the intersection of this edge and its previous one
				# which is definetely red

				convex_hull = convex_hull + [Edge(intersection,curr_point)]
	
	return convex_hull


points = generate_random_points(30)

scatter_plot(points).save('ex3_random_points.png')

convex_hull = beaneath_beyond(points)

plot_list = [line([e.p1,e.p2]) for e in convex_hull]
(sum(plot_list) + scatter_plot(points)).save("ex3_ch2.png")